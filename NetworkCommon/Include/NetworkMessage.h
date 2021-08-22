#pragma once

#include "NetworkCommon.h"

namespace IMM
{
	namespace Network
	{
		// Message Header is sent at start of all messages. The template allows us
		// to use "enum class" to ensure that the messages are valid at compile time
		template <typename T>
		struct Message_Header
		{
			T mID{};
			uint32_t size = 0;
		};

		// Message Body contains a header and a std::vector, containing raw bytes
		// of infomation. This way the message can be variable length, but the size
		// in the header must be updated.
		template <typename T>
		struct Message
		{
			// Header & Body vector
			Message_Header<T> mHeader{};
			std::vector<uint8_t> mBody;

			//returns size of entire message packet in bytes
			size_t size() const
			{
				return sizeof(Message_Header<T>) + mBody.size();
			}

			// Override for std::cout compatibility
			friend std::ostream& operator << (std::ostream& os, const Message<T>& msg)
			{
				os << "ID: " << int(msg.mHeader.mID) << " Size: " << msg.mHeader.size;
				return os;
			}

			// Convenience Operator overloads - These allow us to add and remove stuff from
			// the body vector as if it were a stack, so First in, Last Out. These are a 
			// template in itself, because we dont know what data type the user is pushing or 
			// popping, so lets allow them all. NOTE: It assumes the data type is fundamentally
			// Plain Old Data (POD). TLDR: Serialise & Deserialise into/from a vector

			// Pushes any POD-like data into the message buffer
			template<typename DataType>
			friend Message<T>& operator <<(Message<T>& msg, const DataType& data)
			{
				// Check that the type of the data being pushed is trivially copyable
				static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into a vector.");

				// Cache current size of vector, as this will be the point we insert the data
				size_t i = msg.mBody.size();

				// Resize the vector by the size of the data being pushed
				msg.mBody.resize(msg.mBody.size() + sizeof(DataType));

				// Physically copy the data into the newly allocated vector space
				std::memcpy(msg.mBody.data() + i, &data, sizeof(DataType));

				// Recalculate the message size
				msg.mHeader.size = msg.size();

				// Return the target message so it can be "chained"
				return msg;
			}

			// Pulls any POD-like data from the message buffer
			template<typename DataType>
			friend Message<T>& operator >> (Message<T>& msg, DataType& data)
			{
				// Check that the type of the data being pushed is trivially copyable
				static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into a vector.");

				// Cache the location towards the end of the vector where the pulled data starts
				size_t i = msg.mBody.size() - sizeof(DataType);

				// Physically copy the data from the vector into the user variable
				std::memcpy(&data, msg.mBody.data() + i, sizeof(DataType));

				// Shrink the vector to remove read bytes, and reset end position
				msg.mBody.resize(i);

				// Recalculate the message size
				msg.mHeader.size = msg.size();

				// Return the target message so it can be "chained"
				return msg;
			}

			//Forward declare the connection
			template <typename T>
			class Connection;

			template <typename T>
			struct OwnedMessage
			{
				std::shared_ptr<Connection<T>> remote = nullptr;
				Message<T> msg;

				friend std::ostream& operator<<(std::ostream& os, const owned_message<T>& msg)
				{
					os << msg.msg;
					return os;
				}
			};
		};
	}
}