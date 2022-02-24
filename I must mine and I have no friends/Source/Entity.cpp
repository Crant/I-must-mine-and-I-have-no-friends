#include "Entity.h"
#include "Globals.h"
#include "GraphicsRenderer.h"


using namespace IMM;

Entity::Entity(const olc::vf2d& vPos, const olc::vf2d& vSize, SpriteType sModel)
{
	this->vPos = vPos;
	this->vSize = vSize;
	this->sModel = sModel;
}

Entity::~Entity()
{
}
bool Entity::UpdateSelf()
{
	if (GlobalState::IsInView(vPos))
	{
		GFX::RenderSprites(this);
	}
	return true;
}


bool IMM::Updated::ListenToEvents(Event* e)
{
	return false;
}

