#pragma once

#include "Observer.h"
#include "olcPixelGameEngine.h"

namespace IMM
{
	namespace Events
	{
		class ClientDirectionChangeEvent : public Event
		{
		public:
			uint32_t clientID;
			olc::vf2d direction;
		};
	}
}