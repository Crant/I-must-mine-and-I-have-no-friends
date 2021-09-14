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
			TileChangedEvent(olc::vi2d tilePos, IMM::TileType tt) :
				tilePos(tilePos),
				tileType(tt)
			{}

			olc::vi2d tilePos;
			IMM::TileType tileType;
		};
	}
	
}
