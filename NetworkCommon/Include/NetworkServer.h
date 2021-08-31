#include "NetworkCommon.h"
#include "NetworkTSQueue.h"
#include "NetworkMessage.h"
#include "NetworkConnection.h"

namespace IMM
{
	namespace Network
	{
		template<typename T>
		class ServerInterface
		{
		public:
			// Create a server, ready to listen on specified port
			ServerInterface(uint16_t port)
				: mAsioAcceptor(mAsioContext, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
			{

			}

			virtual ~ServerInterface()
			{
				// May as well try and tidy up
				Stop();
			}

			bool Start()
			{
				try
				{
					// Issue a task to the asio context - This is important
					// as it will prime the context with "work", and stop it
					// from exiting immediately. Since this is a server, we 
					// want it primed ready to handle clients trying to
					// connect.
					WaitForClientConnection();

					// Launch the asio context in its own thread
					mThreadContext = std::thread([this]() { mAsioContext.run(); });
				}
				catch (std::exception& e)
				{
					// Something prohibited the server from listening
					std::cerr << "[SERVER] Exception: " << e.what() << "\n";
					return false;
				}

				std::cout << "[SERVER] Started!\n";
				return true;
			}

			// Stops the server!
			void Stop()
			{
				// Request the context to close
				mAsioContext.stop();

				// Tidy up the context thread
				if (mThreadContext.joinable()) 
					mThreadContext.join();

				// Inform someone, anybody, if they care...
				std::cout << "[SERVER] Stopped!\n";
			}

			// ASYNC - Instruct asio to wait for a connection
			void WaitForClientConnection()
			{
				// Prime context with an instruction to wait until a socket connects. This
				// is the purpose of an "acceptor" object. It will provide a unique socket
				// for each incoming connection attempt
				mAsioAcceptor.async_accept(
					[this](std::error_code ec, asio::ip::tcp::socket socket)
					{
						// Triggered by incoming connection request
						if (!ec)
						{
							// Display some useful(?) information
							std::cout << "[SERVER] New Connection: " << socket.remote_endpoint() << "\n";

							// Create a new connection to handle this client 
							std::shared_ptr<Connection<T>> newconn =
								std::make_shared<Connection<T>>(Connection<T>::Owner::Server,
									mAsioContext, std::move(socket), mQMessagesIn);

							// Give the user server a chance to deny connection
							if (OnClientConnect(newconn))
							{
								// Connection allowed, so add to container of new connections
								mDeqConnections.push_back(std::move(newconn));

								// And very important! Issue a task to the connection's
								// asio context to sit and wait for bytes to arrive!
								mDeqConnections.back()->ConnectToClient(this, nIDCounter++);

								std::cout << "[" << mDeqConnections.back()->GetID() << "] Connection Approved\n";
							}
							else
							{
								std::cout << "[-----] Connection Denied\n";

								// Connection will go out of scope with no pending tasks, so will
								// get destroyed automagically due to the wonder of smart pointers
							}
						}
						else
						{
							// Error has occurred during acceptance
							std::cout << "[SERVER] New Connection Error: " << ec.message() << "\n";
						}

						// Prime the asio context with more work - again simply wait for
						// another connection...
						WaitForClientConnection();
					});
			}

			// Send a message to a specific client
			void MessageClient(std::shared_ptr<Connection<T>> client, const Message<T>& msg)
			{
				// Check client is legitimate...
				if (client && client->IsConnected())
				{
					// ...and post the message via the connection
					client->Send(msg);
				}
				else
				{
					// If we cant communicate with client then we may as 
					// well remove the client - let the server know, it may
					// be tracking it somehow
					OnClientDisconnect(client);

					// Off you go now, bye bye!
					client.reset();

					// Then physically remove it from the container
					mDeqConnections.erase(
						std::remove(mDeqConnections.begin(), mDeqConnections.end(), client), mDeqConnections.end());
				}
			}

			// Send a message to all clients
			void MessageAllClients(const Message<T>& msg, std::shared_ptr<Connection<T>> pIgnoreClient = nullptr)
			{
				bool bInvalidClientExists = false;

				// Iterate through all clients in container
				for (auto& client : mDeqConnections)
				{
					// Check client is connected...
					if (client && client->IsConnected())
					{
						// ..it is!
						if (client != pIgnoreClient)
							client->Send(msg);
					}
					else
					{
						// The client couldnt be contacted, so assume it has
						// disconnected.
						OnClientDisconnect(client);
						client.reset();

						// Set this flag to then remove dead clients from container
						bInvalidClientExists = true;
					}
				}

				// Remove dead clients, all in one go - this way, we dont invalidate the
				// container as we iterated through it.
				if (bInvalidClientExists)
					mDeqConnections.erase(
						std::remove(mDeqConnections.begin(), mDeqConnections.end(), nullptr), mDeqConnections.end());
			}

			// Force server to respond to incoming messages
			void Update(size_t nMaxMessages = -1, bool bWait = false)
			{
				if (bWait) 
					mQMessagesIn.wait();

				// Process as many messages as you can up to the value
				// specified
				size_t nMessageCount = 0;
				while (nMessageCount < nMaxMessages && !mQMessagesIn.empty())
				{
					// Grab the front message
					auto msg = mQMessagesIn.pop_front();

					// Pass to message handler
					OnMessage(msg.remote, msg.msg);

					nMessageCount++;
				}
			}

			// Called when a client is validated
			virtual void OnClientValidated(std::shared_ptr<Connection<T>> client)
			{

			}

		protected:
			// Called when a client connects, you can veto the connection by returning false
			virtual bool OnClientConnect(std::shared_ptr<Connection<T>> client)
			{
				return false;
			}

			// Called when a client appears to have disconnected
			virtual void OnClientDisconnect(std::shared_ptr<Connection<T>> client)
			{

			}

			// Called when a message arrives
			virtual void OnMessage(std::shared_ptr<Connection<T>> client, Message<T>& msg)
			{

			}

		protected:
			// Thread Safe Queue for incoming message packets
			TsQueue<OwnedMessage<T>> mQMessagesIn;

			// Container of active validated connections
			std::deque<std::shared_ptr<Connection<T>>> mDeqConnections;

			// Order of declaration is important - it is also the order of initialisation
			asio::io_context mAsioContext;
			std::thread mThreadContext;

			// These things need an asio context
			asio::ip::tcp::acceptor mAsioAcceptor; // Handles new incoming connection attempts...

			// Clients will be identified in the "wider system" via an ID
			uint32_t nIDCounter = 10000;
		};
	}
}