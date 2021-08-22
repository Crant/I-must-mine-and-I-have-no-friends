#pragma once

#include "NetworkCommon.h"
#include "NetworkTSQueue.h"
#include "NetworkMessage.h"

namespace IMM
{
	namespace Network
	{
		template<typename T>
		class Connection : public std::enable_shared_from_this<Connection<T>>
		{
		public:

			// A connection is "owned" by either a server or a client, and its
			// behaviour is slightly different bewteen the two.
			enum class Owner
			{
				Server,
				Host,
				Client
			};

			Connection()
			{}

			virtual ~Connection()
			{}

			bool ConnectToServer();
			bool Disconnect();
			bool IsConnected() const;

			bool Send(const Message<T>& msg);

		protected:
			// Each connection has a unique socket to a remote
			asio::ip::tcp::socket mSocket;

			// This context is shared with the whole asio instance
			asio::io_context& mAsioContext;

			// This queue holds all messages to be sent to the remote side
			// of this connection
			TsQueue<Message<T>> mQueueMessagesOut;

			// This queue holds all messages that have been recieved from
			// the remote side of this connection. Note it is a reference
			// as the "owner" of this connection is expected to provide a queue
			TsQueue<OwnedMessage>& mQueueMessagesIn;
		};
	}
}