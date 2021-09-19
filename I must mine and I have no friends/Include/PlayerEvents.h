#pragma once

#include "Observer.h"
#include "olcPixelGameEngine.h"

namespace IMM
{
	namespace Events
	{
		class PlayerMoveEvent : public Event
		{
		public:
			olc::vf2d direction;
		};
	}
}