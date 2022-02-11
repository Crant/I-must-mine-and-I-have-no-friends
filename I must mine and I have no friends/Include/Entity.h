#pragma once
#include "olcPixelGameEngine.h"

//#include "GraphicsRenderer.h"

namespace IMM
{
	enum class SpriteType : unsigned char
	{
		Default,
		Player
	};

	class Updated
	{
	public:
		virtual bool UpdateSelf() = 0;
	};

	class Entity : public Updated, public std::enable_shared_from_this<Entity> //Entity representerar basklassen för alla spelvärldens objekt
	{
	public:
		Entity(const olc::vf2d& vPos, const olc::vf2d& vSize, SpriteType sModel);
		~Entity();

		olc::vf2d vPos;
		olc::vf2d vSize;
		SpriteType sModel;
		bool UpdateSelf() override; //Return false för att döda objektet

	protected:


	private:

	};
}


