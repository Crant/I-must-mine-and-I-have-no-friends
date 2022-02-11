#pragma once

#include "World.h"
#include "Tiles.h"
#include "Observer.h"

namespace IMM
{
	namespace Events
	{

		class WorldCreatedEvent : public Event
		{
		public:
			WorldCreatedEvent(std::shared_ptr<IMM::World> pWorld) : world(pWorld) { }
			std::shared_ptr<IMM::World> world;
		};

		class TileChangedEvent : public Event
		{
		public:
			TileChangedEvent(olc::vi2d tilePos, IMM::TileType tNewTile, TileType tOldTile) :
				tilePos(tilePos),
				tNewtileType(tNewTile),
				tOldTileType(tOldTile)
			{}

			olc::vi2d tilePos;
			IMM::TileType tNewtileType;
			IMM::TileType tOldTileType;
		};
		class TileRemovedEvent : public Event
		{
		public:
			TileRemovedEvent(int nTilePos, IMM::TileType tOldTile) :
				nTilePos(nTilePos),
				tOldTile(tOldTile)
				
			{
				type = EventType::TileRemoved;
			}

			int nTilePos;
			IMM::TileType tOldTile;
		};
		class TilePlacedEvent : public Event
		{
		public:
			TilePlacedEvent(int nTilePos, TileType tPlacedTile) :
				nTilePos(nTilePos),
				tPlacedTile(tPlacedTile)
			{
				type = EventType::TilePlaced;
			}

			int nTilePos;
			IMM::TileType tPlacedTile;
		};

	}
}
