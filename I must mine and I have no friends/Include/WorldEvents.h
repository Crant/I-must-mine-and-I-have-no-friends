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
			WorldCreatedEvent(std::shared_ptr<IMM::World> pWorld, std::shared_ptr<std::vector<float>> pPerlins) : world(pWorld), perlins(pPerlins){ }
			std::shared_ptr<IMM::World> world;
			std::shared_ptr<std::vector<float>> perlins;
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
				tOldTile(tOldTile),
				bHasBeenPickedUp(false)
				
			{
				type = EventType::TileRemoved;
			}
			int nTilePos;
			IMM::TileType tOldTile;
			bool bHasBeenPickedUp;
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
