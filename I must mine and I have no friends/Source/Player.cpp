#include "Player.h"
#include "Globals.h"
#include "World.h"

using namespace IMM;

IMM::Player::Player(const olc::vf2d& vInitPos, const olc::vf2d& vSize, SpriteType sModel, float fHitpoints) 
	: PhysUnit(vInitPos, vSize, sModel, fHitpoints)
{
	mInventory = std::make_shared<Inventory>();
	World::Main()->AddObserver(this);
}

std::shared_ptr<Inventory> IMM::Player::GetInventory()
{
	return mInventory;
}

void Player::MoveUnit()
{
	if (GlobalState::GetEngine()->IsFocused())
	{
		if (GlobalState::GetEngine()->GetKey(olc::Key::W).bHeld)
		{
			vVel.y -= 30.5f * GlobalState::GetEngine()->GetElapsedTime();
		}
		if (GlobalState::GetEngine()->GetKey(olc::Key::A).bHeld)
		{
			vVel.x -= 30.5f * GlobalState::GetEngine()->GetElapsedTime();
		}
		if (GlobalState::GetEngine()->GetKey(olc::Key::D).bHeld)
		{
			vVel.x += 30.5f * GlobalState::GetEngine()->GetElapsedTime();
		}
		if (GlobalState::GetEngine()->GetKey(olc::Key::S).bHeld)
		{
			vVel.y += 30.5f * GlobalState::GetEngine()->GetElapsedTime();
		}
		if (GlobalState::GetEngine()->GetKey(olc::Key::SPACE).bHeld && bIsGrounded)
		{
			ApplyForce(olc::vf2d(0, -50.f));
		}
		if (GlobalState::GetEngine()->GetKey(olc::Key::K1).bHeld)
		{
			cActiveItem = mInventory->mStorage[0];
		}
		if (GlobalState::GetEngine()->GetKey(olc::Key::K2).bHeld)
		{
			cActiveItem = mInventory->mStorage[1];
		}
		if (GlobalState::GetEngine()->GetKey(olc::Key::K3).bHeld)
		{
			cActiveItem = mInventory->mStorage[2];
		}
		if (GlobalState::GetEngine()->GetKey(olc::Key::K4).bHeld)
		{
			cActiveItem = mInventory->mStorage[3];
		}
		if (GlobalState::GetEngine()->GetKey(olc::Key::K5).bHeld)
		{
			cActiveItem = mInventory->mStorage[4];
		}
		if (GlobalState::GetEngine()->GetKey(olc::Key::K6).bHeld)
		{
			cActiveItem = mInventory->mStorage[5];
		}
		if (GlobalState::GetEngine()->GetKey(olc::Key::K7).bHeld)
		{
			cActiveItem = mInventory->mStorage[6];
		}
		if (GlobalState::GetEngine()->GetKey(olc::Key::K8).bHeld)
		{
			cActiveItem = mInventory->mStorage[7];
		}
		if (GlobalState::GetEngine()->GetKey(olc::Key::K9).bHeld)
		{
			cActiveItem = mInventory->mStorage[8];
		}
		if (GlobalState::GetEngine()->GetMouse(0).bHeld && cActiveItem != nullptr)
		{
			vTarget = GlobalState::GetMouseWorld();
			cActiveItem->Use(this);
		}
		//World::Main()->CheckWrapping(vPos.x, vPos.x);
		//World::Main()->CheckWrapping(GlobalState::GetMouseWorld().x,);

		GlobalState::Update(vPos);
	}
}

void Player::UseItem()
{
	cActiveItem->Use(this);
}

void IMM::Player::OnEvent(Event* e)
{
	if (e->type == EventType::TileRemoved)
	{
		auto TRE = reinterpret_cast<IMM::Events::TileRemovedEvent*>(e);
		olc::vf2d coords = World::Main()->Coordinates(TRE->nTilePos);


		if (std::abs(coords.x - vPos.x) < nPickupDistance && std::abs(coords.y - vPos.y) < nPickupDistance)
		{
			mInventory->AddItemToInventory(std::make_shared<Block>(TRE->tOldTile));
		}
	}
}