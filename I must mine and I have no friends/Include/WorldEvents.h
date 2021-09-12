#pragma once

#include "World.h"
#include "Observer.h"

class WorldCreatedEvent : public Event
{
public:
	WorldCreatedEvent(std::shared_ptr<IMM::World> pWorld) : world(pWorld) { }
	std::shared_ptr<IMM::World> world;
};