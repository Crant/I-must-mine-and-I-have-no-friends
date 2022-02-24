#include "Player.h"
#include "Globals.h"
#include "World.h"
#include "GraphicsRenderer.h"
#include "EntityManager.h"

using namespace IMM;

IMM::Player::Player(const olc::vf2d& vInitPos, const olc::vf2d& vSize, SpriteType sModel, float fHitpoints) 
	: PhysUnit(vInitPos, vSize, sModel, fHitpoints)
{
	mInventory = std::make_shared<Inventory>();
	cInventoryMenu = std::make_shared<DynamicMenu>(olc::vf2d(0, 0), mInventory);
	//vMenus.push_back(cInventoryMenu);
	World::Main()->AddObserver(this);
}

std::shared_ptr<Inventory> IMM::Player::GetInventory()
{
	return mInventory;
}

void IMM::Player::UpdateUI()
{
	UpdateAllMenus();
	if (!IsInInventory())
	{
		cActiveItem = nullptr;
	}
}
void Player::UpdateAllMenus()
{
	olc::PixelGameEngine* pge = GlobalState::GetEngine();
	nLastIndex = 0;
	bHasReleasedItem = false;

	for (auto menu : vMenus)
	{
		if (std::abs(menu->vPos.x - vPos.x) < nPickupDistance && std::abs(menu->vPos.y - vPos.y) < nPickupDistance)
		{
			UpdateMenu(menu->cInventoryMenu);
		}
		else
		{
			menu->cInventoryMenu->bMenuActive = false;
		}
	}
	UpdateMenu(cInventoryMenu);
	for (auto menu : vMenus)
	{
		if (menu != nullptr && menu->cInventoryMenu->bMenuActive)
		{
			menu->cInventoryMenu->Render(pge);
		}
	}

	if (cInventoryMenu != nullptr && cInventoryMenu->bMenuActive)
	{
		cInventoryMenu->Render(pge);
	}
}
bool IMM::Player::IsInInventory()
{
	for (auto it = mInventory->mStorage.begin(); it != mInventory->mStorage.end(); ++it)
	{
		if (*it == cActiveItem)
		{
			return true;
		}
	}
	//cActiveItem == nullptr;
	return false;
}
bool Player::UpdateMenu(std::shared_ptr<DynamicMenu> menu)
{
	if (menu != nullptr && menu->bMenuActive)
	{
		olc::PixelGameEngine* pge = GlobalState::GetEngine();
		if (pge->GetMouse(1).bPressed)
		{
			menu->vStartDragPos = pge->GetMousePos();
			menu->vDragOffset = menu->vPos;
		}
		if (pge->GetMouse(1).bHeld)
		{
			if (menu->vPos.x <= menu->vStartDragPos.x && menu->vStartDragPos.x <= menu->vPos.x + menu->vMenuSize.x &&
				menu->vPos.y <= menu->vStartDragPos.y && menu->vStartDragPos.y <= menu->vPos.y + menu->vMenuSize.y)
			{
				menu->vDragOffset += (pge->GetMousePos() - menu->vStartDragPos);
				menu->vStartDragPos = pge->GetMousePos();


				for (int x = 0; x < menu->nMenuPanels; x++)
				{
					for (int y = 0; y < menu->nMenuPanels; y++)
					{
						int nIndex = y * menu->nMenuPanels + x;
						olc::vf2d vPanelPos = olc::vf2d((menu->vPanelSize.x * x) + (menu->vPanelOffset.x * (x)), (menu->vPanelSize.y * y) + (menu->vPanelOffset.y * (y))) + menu->vDragOffset;

						menu->mButtons[nIndex]->UpdatePos(vPanelPos);
					}
				}
				menu->vPos = menu->vDragOffset;
			}
		}
		for (int x = 0; x < menu->nMenuPanels; x++)
		{
			for (int y = 0; y < menu->nMenuPanels; y++)
			{
				int nIndex = y * menu->nMenuPanels + x;
				if (menu->mButtons[nIndex]->Hovered(pge->GetMouseX(), pge->GetMouseY()))
				{
					if (pge->GetMouse(0).bPressed && menu->cInventory->mStorage.at(nIndex) != nullptr && !menu->bItemHeld)
					{
						menu->bItemHeld = true;
						menu->nItemPressed = nIndex;
						nItemPressedIndex = nIndex;
						cFirstInvCheck = menu->cInventory;
					}
					else if (pge->GetMouse(0).bReleased)
					{
						cSecondInvCheck = menu->cInventory;

						nLastIndex = nIndex;
						bHasReleasedItem = true;
					}

				}
			}
		}
		if (bHasReleasedItem)
		{
			Inventory::AddItemFromInventoryToOther(cFirstInvCheck, cSecondInvCheck, nItemPressedIndex, nLastIndex); //�NDRA DETTA TILL ETT EVENT SOM SEDAN OMVANDLAS TILL MSG F�R SERVER
			bHasReleasedItem = false;
		}
		if (!pge->GetMouse(0).bHeld)
		{
			menu->bItemHeld = false;
		}

		//menu->Render(pge);
	}
	return true;
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
		if (GlobalState::GetEngine()->GetKey(olc::Key::I).bPressed)
		{
			//if (!vMenus[0]->bMenuActive)
			//{
			//	vMenus[0]->bMenuActive = true;
			//}
			//else
			//{
			//	vMenus[0]->bMenuActive = false;
			//}
			if (!cInventoryMenu->bMenuActive)
			{
				cInventoryMenu->bMenuActive = true;
			}
			else
			{
				cInventoryMenu->bMenuActive = false;
			}
		}
		if (GlobalState::GetEngine()->GetMouse(0).bHeld && cActiveItem != nullptr)
		{
			vTarget = GlobalState::GetMouseWorld();
			//cActiveItem->Use(this);
			UseItem();
		}
		if (GlobalState::GetEngine()->GetMouse(1).bPressed)
		{
			olc::vf2d vMouse = GlobalState::GetMouseWorld();
			if (std::abs(vMouse.x - vPos.x) < nPickupDistance && std::abs(vMouse.y - vPos.y) < nPickupDistance)
			{
				auto cNewStorage = EntityManager::GetStorageAtPos(vMouse);

				if (cNewStorage != nullptr)
				{
					bool bIsAlreadyInInv = false;
					for (auto& storage : vMenus)
					{
						if (storage == cNewStorage)
						{
							bIsAlreadyInInv = true;

							break;
						}
					}
					if (bIsAlreadyInInv)
					{
						cNewStorage->cInventoryMenu->bMenuActive = false;
						vMenus.remove(cNewStorage);
					}
					else
					{
						cNewStorage->cInventoryMenu->bMenuActive = true;
						vMenus.push_back(cNewStorage);
					}

				}

				// ->bMenuActive = true;
			}
		}
		//World::Main()->CheckWrapping(vPos.x, vPos.x);
		//World::Main()->CheckWrapping(GlobalState::GetMouseWorld().x,);

		/*GlobalState::Update(vPos);*/
	}
}

void Player::UseItem()
{
	if (!cActiveItem->Use(this))
	{
		mInventory->RemoveItemFromInventory(cActiveItem);
	}

}

void IMM::Player::OnEvent(Event* e)
{
	if (e->type == EventType::TileRemoved)
	{
		auto TRE = reinterpret_cast<IMM::Events::TileRemovedEvent*>(e);
		olc::vf2d coords = World::Main()->Coordinates(TRE->nTilePos);


		if (std::abs(coords.x - vPos.x) < nPickupDistance && std::abs(coords.y - vPos.y) < nPickupDistance && !TRE->bHasBeenPickedUp)
		{
			int nInvIndex = 0;
			if (mInventory->HasBlockInInventory(TRE->tOldTile, nInvIndex))
			{
				mInventory->AddBlockToStack(TRE->tOldTile, TRE->nTilePos, nInvIndex);
			}
			else
			{
				mInventory->AddItemToInventory(std::make_shared<Block>(TRE->tOldTile, TRE->nTilePos));
			}
			TRE->bHasBeenPickedUp = true;
		}
	}
}

//bool IMM::Player::ListenToEvents(Event* e)
//{
//	if (e->type == EventType::TileRemoved)
//	{
//		auto TRE = reinterpret_cast<IMM::Events::TileRemovedEvent*>(e);
//		olc::vf2d coords = World::Main()->Coordinates(TRE->nTilePos);
//
//
//		if (std::abs(coords.x - vPos.x) < nPickupDistance && std::abs(coords.y - vPos.y) < nPickupDistance)
//		{
//			int nInvIndex = 0;
//			if (mInventory->HasBlockInInventory(TRE->tOldTile, nInvIndex))
//			{
//				mInventory->AddBlockToStack(TRE->tOldTile, TRE->nTilePos, nInvIndex);
//			}
//			else
//			{
//				mInventory->AddItemToInventory(std::make_shared<Block>(TRE->tOldTile, TRE->nTilePos));
//			}
//			return true;
//		}
//	}
//	return false;
//}
