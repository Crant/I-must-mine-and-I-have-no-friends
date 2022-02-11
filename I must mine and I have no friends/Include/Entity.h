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

	class Entity : public Updated, public std::enable_shared_from_this<Entity> //Entity representerar basklassen f�r alla spelv�rldens objekt
	{
	public:
		Entity(const olc::vf2d& vPos, const olc::vf2d& vSize, SpriteType sModel);
		~Entity();

		olc::vf2d vPos;
		olc::vf2d vSize;
		SpriteType sModel;
		bool UpdateSelf() override; //Return false f�r att d�da objektet

	protected:


	private:

	};
}


