#pragma once

#include "NetworkCommon.h"
#include "NetworkTSQueue.h"
#include "NetworkMessage.h"
#include "NetworkConnection.h"

namespace IMM
{
	namespace Network
	{
		template <typename T>
		class ClientInterface
		{
		public:
			ClientInterface() : mSocket(mContext)
			{
				// Initialise the socket with the io context, so it can do stuff.

			}

			virtual ~ClientInterface()
			{
				// If the client is destroyed, always try and disconnect from the server
				Disconnect();
			}


			// Connect to server with hostname / ip-address and port
			bool Connect(const std::string& host, const uint16_t port)
			{
				try
				{
					// Create connection
					mConnection = std::make_unique<Connection<T>>(); //TODO

					// Resolve hostname / ip-address into tangiable physical address
					asio::ip::tcp::resolver resolver(mContext);
					mEndpoints = resolver.resolve(host, std::to_string(port));

					// Tell the connection object to connect to the server
					mConnection->ConnectToServer(mEndpoints);

					// Start the context thread
					mThreadContext = std::thread([this]() { mContext.run(); });
				}
				catch (std::exception& e)
				{
					std::cerr << "Client Exception: " << e.what() << "\n";
					return false;
				}
				return true;
			}

			//Disconnect from the server
			void Disconnect()
			{
				// if connection exists, and it's connected then...
				if (IsConnected())
				{
					// ...disconnect from the server
					mConnection->Disconnect();
				}

				// Either way, we're also done with the asio context...
				mContext.stop();
				// ...and its thread
				if (mThreadContext.joinable())
					mThreadContext.join();

				// Destroy the connection object
				mConnection.release();
			}

			// Check if the client is actually connected to a server
			bool IsConnected()
			{
				if (mConnection)
					return mConnection.IsConnected();
				else
					return false;
			}

			// Retrieve the queue of messages from the server
			TsQueue<OwnedMessage<T>>& Incoming()
			{
				return mQueueMessagesIn;
			}

		protected:
			// Asio context handles the data transfer
			asio::io_context mContext;

			// ...but needs a thread of its own to execute its work commands
			std::thread mThreadContext;

			// This is the hardware socket that is connected to the server
			asio::ip::tcp::socket mSocket;

			// The client has a single instance of a "Connection object, which handles data transfer
			std::unique_ptr<Connection<T>> mConnection;

		private:
			// This is the thread safe queue of incoming messages from server
			TsQueue<OwnedMessage<T>> mQueueMessagesIn;
		};
	}
}