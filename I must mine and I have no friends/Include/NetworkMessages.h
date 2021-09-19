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
	ServerSendWorldWidth,
	ServerSendTileChange,
	ServerSendUnitUpdates,
	ClientRequestWorldInfo,
	ClientRequestWorldData,
	ClientRequestWorldChanges,
	ClientChangeTileRequest,
	ClientDirectionChange,
	
};