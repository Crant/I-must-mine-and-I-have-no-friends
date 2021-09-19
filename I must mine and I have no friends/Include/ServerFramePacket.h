#pragma once

#include "NetworkMessages.h"
#include "olcPixelGameEngine.h"

namespace IMM
{
	class ServerFramePacket
	{
	public:
		ServerFramePacket() {}
		ServerFramePacket(olc::vf2d position, olc::vf2d direction, uint32_t id) :
			mDirection(direction),
			mPosition(position),
			mID(id) { }

		const olc::vf2d& GetDirection() const { return mDirection; }
		const olc::vf2d& GetPosition() const { return mPosition; }
		const uint32_t& GetID() const { return mID; }
	private:
		olc::vf2d mDirection;
		olc::vf2d mPosition;
		uint32_t mID;
	};
}
