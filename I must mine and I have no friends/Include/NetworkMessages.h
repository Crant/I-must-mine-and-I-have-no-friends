#pragma once
#include <Network.h>

enum class NetworkMessageTypes : uint32_t
{
	ServerAccept,
	ServerDeny,
	ServerPing,
	MessageAll,
	ServerMessage,
	ServerSendWorldSeed,
	ServerSendWorldFull,
	ServerSendWorldChanges,
	ClientRequestWorld,
	ClientRequestWorldChanges,
	ServerSendWorldWidth,
};