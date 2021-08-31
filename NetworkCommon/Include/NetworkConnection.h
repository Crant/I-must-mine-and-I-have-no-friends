#pragma once

#include "NetworkCommon.h"
#include "NetworkTSQueue.h"
#include "NetworkMessage.h"

namespace IMM
{
	namespace Network
	{
		//Forward declare
		template<typename T>
		class ServerInterface;

		template<typename T>
		class Connection : public std::enable_shared_from_this<Connection<T>>
		{
		public:

			// A connection is "owned" by either a server or a client, and its
			// behaviour is slightly different between the two.
			enum class Owner
			{
				Server,
				Host,
				Client
			};

			// Constructor: Specify Owner, connect to context, transfer the socket
			//				Provide reference to incoming message queue
			Connection(Owner parent, asio::io_context& asioContext, asio::ip::tcp::socket socket, TsQueue<OwnedMessage<T>>& qIn)
				: mAsioContext(asioContext), mSocket(std::move(socket)), mQMessagesIn(qIn)
			{
				mOwnerType = parent;

				// Construct validation check data
				if (mOwnerType == Owner::Server)
				{
					// Connection is Server -> Client, construct random data for the client
					// to transform and send back for validation
					mHandshakeOut = uint64_t(std::chrono::system_clock::now().time_since_epoch().count());

					// Pre-calculate the result for checking when the client responds
					mHandshakeCheck = Scramble(mHandshakeOut);
				}
				else
				{
					// Connection is Client -> Server, so we have nothing to define, 
					mHandshakeIn = 0;
					mHandshakeOut = 0;
				}
			}

			virtual ~Connection()
			{}

			// This ID is used system wide - its how clients will understand other clients
			// exist across the whole system.
			uint32_t GetID() const
			{
				return mId;
			}

			void ConnectToClient(IMM::Network::ServerInterface<T>* server, uint32_t uId = 0)
			{
				if (mOwnerType == Owner::Server)
				{
					if (mSocket.is_open())
					{
						mId = uId;

						// A client has attempted to connect to the server, but we wish
						// the client to first validate itself, so first write out the
						// handshake data to be validated
						WriteValidation();

						// Next, issue a task to sit and wait asynchronously for precisely
						// the validation data sent back from the client
						ReadValidation(server);
					}
				}
			}

			void ConnectToServer(const asio::ip::tcp::resolver::results_type& endpoints)
			{
				// Only clients can connect to servers
				if (mOwnerType == Owner::Client)
				{
					// Request asio attempts to connect to an endpoint
					asio::async_connect(mSocket, endpoints,
						[this](std::error_code ec, asio::ip::tcp::endpoint endpoint)
						{
							if (!ec)
							{
								// First thing server will do is send packet to be validated
								// so wait for that and respond
								ReadValidation();
							}
						});
				}
			}

			void Disconnect()
			{
				if (IsConnected())
					asio::post(mAsioContext, [this]() { mSocket.close(); });
			}
			bool IsConnected() const
			{
				return mSocket.is_open();
			}


			// ASYNC - Send a message, connections are one-to-one so no need to specifiy
			// the target, for a client, the target is the server and vice versa
			void Send(const Message<T>& msg)
			{
				asio::post(mAsioContext,
					[this, msg]()
					{
						// If the queue has a message in it, then we must 
						// assume that it is in the process of asynchronously being written.
						// Either way add the message to the queue to be output. If no messages
						// were available to be written, then start the process of writing the
						// message at the front of the queue.
						bool bWritingMessage = !mQMessagesOut.empty();

						mQMessagesOut.push_back(msg);
						if (!bWritingMessage)
						{
							WriteHeader();
						}
					});
			}

		private:
			// ASYNC - Prime context to write a message header
			void WriteHeader()
			{
				// If this function is called, we know the outgoing message queue must have 
				// at least one message to send. So allocate a transmission buffer to hold
				// the message, and issue the work - asio, send these bytes
				asio::async_write(mSocket, asio::buffer(&mQMessagesOut.front().mHeader, sizeof(Message_Header<T>)),
					[this](std::error_code ec, std::size_t length)
					{
						// asio has now sent the bytes - if there was a problem
						// an error would be available...
						if (!ec)
						{
							// ... no error, so check if the message header just sent also
							// has a message body...
							if (mQMessagesOut.front().mBody.size() > 0)
							{
								// ...it does, so issue the task to write the body bytes
								WriteBody();
							}
							else
							{
								// ...it didnt, so we are done with this message. Remove it from 
								// the outgoing message queue
								mQMessagesOut.pop_front();

								// If the queue is not empty, there are more messages to send, so
								// make this happen by issuing the task to send the next header.
								if (!mQMessagesOut.empty())
								{
									WriteHeader();
								}
							}
						}
						else
						{
							// ...asio failed to write the message, we could analyse why but 
							// for now simply assume the connection has died by closing the
							// socket. When a future attempt to write to this client fails due
							// to the closed socket, it will be tidied up.
							std::cout << "[" << mId << "] Write Header Fail.\n";
							mSocket.close();
						}
					});
			}

			// ASYNC - Prime context to write a message body
			void WriteBody()
			{
				// If this function is called, a header has just been sent, and that header
				// indicated a body existed for this message. Fill a transmission buffer
				// with the body data, and send it!
				asio::async_write(mSocket, asio::buffer(mQMessagesOut.front().mBody.data(), mQMessagesOut.front().mBody.size()),
					[this](std::error_code ec, std::size_t length)
					{
						if (!ec)
						{
							// Sending was successful, so we are done with the message
							// and remove it from the queue
							mQMessagesOut.pop_front();

							// If the queue still has messages in it, then issue the task to 
							// send the next messages' header.
							if (!mQMessagesOut.empty())
							{
								WriteHeader();
							}
						}
						else
						{
							// Sending failed, see WriteHeader() equivalent for description :P
							std::cout << "[" << mId << "] Write Body Fail.\n";
							mSocket.close();
						}
					});
			}

			// ASYNC - Prime context ready to read a message header
			void ReadHeader()
			{
				// If this function is called, we are expecting asio to wait until it receives
				// enough bytes to form a header of a message. We know the headers are a fixed
				// size, so allocate a transmission buffer large enough to store it. In fact, 
				// we will construct the message in a "temporary" message object as it's 
				// convenient to work with.
				asio::async_read(mSocket, asio::buffer(&mMsgTemporaryIn.mHeader, sizeof(Message_Header<T>)),
					[this](std::error_code ec, std::size_t length)
					{
						if (!ec)
						{
							// A complete message header has been read, check if this message
							// has a body to follow...
							if (mMsgTemporaryIn.mHeader.size > 0)
							{
								// ...it does, so allocate enough space in the messages' body
								// vector, and issue asio with the task to read the body.
								mMsgTemporaryIn.mBody.resize(mMsgTemporaryIn.mHeader.size);
								ReadBody();
							}
							else
							{
								// it doesn't, so add this bodyless message to the connections
								// incoming message queue
								AddToIncomingMessageQueue();
							}
						}
						else
						{
							// Reading from the client went wrong, most likely a disconnect
							// has occurred. Close the socket and let the system tidy it up later.
							std::cout << "[" << mId << "] Read Header Fail.\n";
							mSocket.close();
						}
					});
			}

			// ASYNC - Prime context ready to read a message body
			void ReadBody()
			{
				// If this function is called, a header has already been read, and that header
				// request we read a body, The space for that body has already been allocated
				// in the temporary message object, so just wait for the bytes to arrive...
				asio::async_read(mSocket, asio::buffer(mMsgTemporaryIn.mBody.data(), mMsgTemporaryIn.mBody.size()),
					[this](std::error_code ec, std::size_t length)
					{
						if (!ec)
						{
							// ...and they have! The message is now complete, so add
							// the whole message to incoming queue
							AddToIncomingMessageQueue();
						}
						else
						{
							// As above!
							std::cout << "[" << mId << "] Read Body Fail.\n";
							mSocket.close();
						}
					});
			}

			// Once a full message is received, add it to the incoming queue
			void AddToIncomingMessageQueue()
			{
				// Shove it in queue, converting it to an "owned message", by initialising
				// with the a shared pointer from this connection object
				if (mOwnerType == Owner::Server)
					mQMessagesIn.push_back({ this->shared_from_this(), mMsgTemporaryIn });
				else
					mQMessagesIn.push_back({ nullptr, mMsgTemporaryIn });

				// We must now prime the asio context to receive the next message. It 
				// wil just sit and wait for bytes to arrive, and the message construction
				// process repeats itself. Clever huh?
				ReadHeader();
			}

			//Validation
			
			// "Encrypt" data
			uint64_t Scramble(uint64_t nInput)
			{
				uint64_t out = nInput ^ 0xAEADBEEFB0DEEAFE;
				out = (out & 0xF0F0F0F0F0F0F0) >> 4 | (out & 0x0F0F0F0F0F0F0F) << 4;
				return out ^ 0xC0DEFACE12345678;
			}

			// ASYNC - Used by both client and server to write validation packet
			void WriteValidation()
			{
				asio::async_write(mSocket, asio::buffer(&mHandshakeOut, sizeof(uint64_t)),
					[this](std::error_code ec, std::size_t length)
					{
						if (!ec)
						{
							// Validation data sent, clients should sit and wait
							// for a response (or a closure)
							if (mOwnerType == Owner::Client)
								ReadHeader();
						}
						else
						{
							mSocket.close();
						}
					});
			}

			void ReadValidation(IMM::Network::ServerInterface<T>* server = nullptr)
			{
				asio::async_read(mSocket, asio::buffer(&mHandshakeIn, sizeof(uint64_t)),
					[this, server](std::error_code ec, std::size_t length)
					{
						if (!ec)
						{
							if (mOwnerType == Owner::Server)
							{
								// Connection is a server, so check response from client

								// Compare sent data to actual solution
								if (mHandshakeIn == mHandshakeCheck)
								{
									// Client has provided valid solution, so allow it to connect properly
									std::cout << "Client Validated" << std::endl;
									server->OnClientValidated(this->shared_from_this());

									// Sit waiting to receive data now
									ReadHeader();
								}
								else
								{
									// Client gave incorrect data, so disconnect
									std::cout << "Client Disconnected (Fail Validation)" << std::endl;
									mSocket.close();
								}
							}
							else
							{
								// Connection is a client, so solve puzzle
								mHandshakeOut = Scramble(mHandshakeIn);

								// Write the result
								WriteValidation();
							}
						}
						else
						{
							// Some biggerfailure occured
							std::cout << "Client Disconnected (ReadValidation)" << std::endl;
							mSocket.close();
						}
					});
			}

		protected:
			// Each connection has a unique socket to a remote
			asio::ip::tcp::socket mSocket;

			// This context is shared with the whole asio instance
			asio::io_context& mAsioContext;

			// This queue holds all messages to be sent to the remote side
			// of this connection
			TsQueue<Message<T>> mQMessagesOut;

			// This queue holds all messages that have been recieved from
			// the remote side of this connection. Note it is a reference
			// as the "owner" of this connection is expected to provide a queue
			TsQueue<OwnedMessage<T>>& mQMessagesIn;

			// Incoming messages are constructed asynchronously, so we will
			// store the part assembled message here, until it is ready
			Message<T> mMsgTemporaryIn;

			// The "owner" decides how some of the connection behaves
			Owner mOwnerType = Owner::Server;

			// Handshake Validation			
			uint64_t mHandshakeOut = 0;
			uint64_t mHandshakeIn = 0;
			uint64_t mHandshakeCheck = 0;


			bool mValidHandshake = false;
			bool mConnectionEstablished = false;


			uint32_t mId = 0;
		};
	}
}