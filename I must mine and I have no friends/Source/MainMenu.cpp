#include "MainMenu.h"
#include "MenuEvents.h"
#include "Globals.h"
#include "AssetsManager.h"
#include "../Maths.h"

#define DESIGN_SCREEN_WIDTH 1920.0f
#define DESIGN_SCREEN_HEIGHT 1080.0f



IMM::Menu::Menu(const olc::vf2d& vStartPos, const olc::vf2d& vSize) :
	vPos(vStartPos), vSize(vSize)
{
	vEndPos = { vPos + vSize };

	int nMidX = ((int)vEndPos.x + (int)this->vPos.x) / 2;
	int nMidY = ((int)vEndPos.y + (int)this->vPos.y) / 2;

	vMid = olc::vi2d(nMidX, nMidY);

	mButtons[0] = std::make_unique<Button>(
		"Assets/hostgameimg.png",
		this->vMid,
		olc::vf2d(GlobalState::GetTileScale(), GlobalState::GetTileScale()));

}
void IMM::Menu::Update(olc::PixelGameEngine* pge)
{
	//vPos = vNewPos;
	//vEndPos = { vPos + vSize };

	//int nMidX = ((int)vEndPos.x + (int)this->vPos.x) / 2;
	//int nMidY = ((int)vEndPos.y + (int)this->vPos.y) / 2;

	//vMid = olc::vi2d(nMidX, nMidY);
	//olc::vf2d vOffsetMid = olc::vf2d((vMid.x - mButtons[0]->GetScaledWidth() / 2), vMid.y - mButtons[0]->GetScaledHeight() / 2);

	//vMid = vOffsetMid;

	//mButtons[0]->UpdatePos(vMid);
}
//void IMM::Menu::Render(olc::PixelGameEngine* pge) // Göra fönster som i windows
//{
//	mButtons[0]->Render(pge);
//
//	pge->DrawLineDecal(vPos, olc::vf2d(vPos.x, vSize.y));
//	pge->DrawLineDecal(olc::vf2d(vPos.x, vSize.y), vSize);
//	pge->DrawLineDecal(vSize, olc::vf2d(vSize.x, vPos.y));
//	pge->DrawLineDecal(olc::vf2d(vSize.x, vPos.y), vPos);
//}
void IMM::Menu::Render(olc::PixelGameEngine* pge)
{
	mButtons[0]->Render(pge);

	pge->DrawLineDecal(vPos, olc::vf2d(vPos.x, vEndPos.y));
	pge->DrawLineDecal(olc::vf2d(vPos.x, vEndPos.y), vEndPos);
	pge->DrawLineDecal(vEndPos, olc::vf2d(vEndPos.x, vPos.y));
	pge->DrawLineDecal(olc::vf2d(vEndPos.x, vPos.y), vPos);
}
IMM::DynamicMenu::DynamicMenu(const olc::vf2d& vStartPos, std::shared_ptr<Inventory> cInventory) :
	vPos(vStartPos), vEndPos(olc::vf2d(cInventory->mStorage.size(), cInventory->mStorage.size()))
{
	nMenuPanels = sqrt(cInventory->mStorage.size());
	this->cInventory = cInventory;

	vPanelSize = olc::vf2d(1 * GlobalState::GetPixelSize(), 1 * GlobalState::GetPixelSize());
	vPanelOffset = vPanelSize / 2;
	vMenuSize = (vPanelSize * nMenuPanels) + (vPanelOffset * nMenuPanels);

	for (int x = 0; x < nMenuPanels; x++)
	{
		for (int y = 0; y < nMenuPanels; y++)
		{
			olc::vf2d vPanelPos = olc::vf2d((vPanelSize.x * x) + (vPanelOffset.x * (x)), (vPanelSize.y * y) + (vPanelOffset.y * (y))) + vStartPos;

			mButtons[y * nMenuPanels + x] = std::make_unique<Button>("Assets/rutor.png", vPanelPos, olc::vf2d(1, 1));
		}
	}
}

void IMM::DynamicMenu::Update(olc::PixelGameEngine* pge)
{
	if (pge->GetMouse(1).bPressed)
	{
		vStartDragPos = pge->GetMousePos();
		vDragOffset = vPos;
	}
	if (pge->GetMouse(1).bHeld)
	{
		if (vPos.x <= vStartDragPos.x && vStartDragPos.x <= vPos.x + vMenuSize.x &&
			vPos.y <= vStartDragPos.y && vStartDragPos.y <= vPos.y + vMenuSize.y)
		{
			vDragOffset += (pge->GetMousePos() - vStartDragPos);
			vStartDragPos = pge->GetMousePos();


			for (int x = 0; x < nMenuPanels; x++)
			{
				for (int y = 0; y < nMenuPanels; y++)
				{
					int nIndex = y * nMenuPanels + x;
					olc::vf2d vPanelPos = olc::vf2d((vPanelSize.x * x) + (vPanelOffset.x * (x)), (vPanelSize.y * y) + (vPanelOffset.y * (y))) + vDragOffset;

					mButtons[nIndex]->UpdatePos(vPanelPos);					
				}
			}
			vPos = vDragOffset;
		}
	}
	for (int x = 0; x < nMenuPanels; x++)
	{
		for (int y = 0; y < nMenuPanels; y++)
		{
			int nIndex = y * nMenuPanels + x;
			if (mButtons[nIndex]->Hovered(pge->GetMouseX(), pge->GetMouseY()))
			{
				if (pge->GetMouse(0).bPressed && cInventory->mStorage.at(nIndex) != nullptr && !bItemHeld)
				{
					bItemHeld = true;
					nItemPressed = nIndex;
				}
				else if (pge->GetMouse(0).bReleased && bItemHeld)
				{
					cInventory->AddItemToInventoryAtPos(cInventory->mStorage.at(nIndex), nItemPressed, nIndex);
					//IMM::Events::InventoryButtonReleasedEvent()
				}
			}
		}
	}
	if (!pge->GetMouse(0).bHeld)
	{
		bItemHeld = false;
	}

}

void IMM::DynamicMenu::Render(olc::PixelGameEngine* pge)
{
	olc::vf2d vStartPos = vPos - vPanelOffset;
	olc::vf2d vEndPos = vPos + vMenuSize;
	olc::vf2d vScale = olc::vf2d(vPanelSize.x / GlobalState::GetPixelSize(), vPanelSize.y / GlobalState::GetPixelSize());

	//Ritar ut ramen för menyn
	pge->DrawLineDecal(vStartPos, olc::vf2d(vStartPos.x, vEndPos.y), olc::DARK_GREY);
	pge->DrawLineDecal(olc::vf2d(vStartPos.x, vEndPos.y), vEndPos, olc::DARK_GREY);
	pge->DrawLineDecal(vEndPos, olc::vf2d(vEndPos.x, vStartPos.y), olc::DARK_GREY);
	pge->DrawLineDecal(olc::vf2d(vEndPos.x, vStartPos.y), vStartPos, olc::DARK_GREY);

	olc::Decal* mDecal = nullptr;

	for (int x = 0; x < nMenuPanels; x++)
	{
		for (int y = 0; y < nMenuPanels; y++)
		{
			int nIndex = y * nMenuPanels + x;
			mButtons[nIndex]->UpdateScale(vScale);
			mButtons[nIndex]->Render(pge); //Bara en rad kod
			mDecal = nullptr;

			//Fixar värdena och ritar ut ramen för inventory slotsen
			olc::vf2d vStartXStartY = olc::vf2d((vPanelSize.x * x) + (vPanelOffset.x * (x + 1)), (vPanelSize.y * y) + (vPanelOffset.y * (y + 1))) + vStartPos;
			olc::vf2d vEndXStartY = olc::vf2d((vPanelSize.x * (x + 1)) + (vPanelOffset.x * (x + 1)), (vPanelSize.y * y) + (vPanelOffset.y * (y + 1))) + vStartPos;
			olc::vf2d vEndXEndY = olc::vf2d((vPanelSize.x * (x + 1)) + (vPanelOffset.x * (x + 1)), (vPanelSize.y * (y + 1)) + (vPanelOffset.y * (y + 1))) + vStartPos;
			olc::vf2d vStartXEndY = olc::vf2d((vPanelSize.x * x) + (vPanelOffset.x * (x + 1)), (vPanelSize.y * (y + 1)) + (vPanelOffset.y * (y + 1))) + vStartPos;

			pge->DrawLineDecal(vStartXStartY, vEndXStartY);
			pge->DrawLineDecal(vEndXStartY, vEndXEndY);
			pge->DrawLineDecal(vEndXEndY, vStartXEndY);
			pge->DrawLineDecal(vStartXEndY, vStartXStartY);

			//Hittar rätt sprite för inventory
			if (cInventory->mStorage.at(nIndex) != nullptr)
			{
				if (cInventory->mStorage.at(nIndex)->GetType() == ItemType::Block)
				{
					mDecal = Assets::Main()->GetSpriteDecal(cInventory->mStorage.at(nIndex)->GetTile());
				}
				else
				{
					mDecal = Assets::Main()->GetSpriteDecal(cInventory->mStorage.at(nIndex)->GetType());
				}
			}
			//Kollar ifall den ska rita vid muspekaren eller inte
			if (mDecal != nullptr)
			{
				olc::vi2d vDrawPos;
				if (bItemHeld && cInventory->mStorage.at(nIndex) == cInventory->mStorage.at(nItemPressed))
				{
					vDrawPos = pge->GetMousePos();
				}
				else
				{
					vDrawPos = vStartXStartY;
				}
				if (cInventory->mStorage.at(nIndex)->GetType() == ItemType::Block)
				{
					pge->DrawPartialDecal(vDrawPos, mDecal, olc::vf2d(0, 0),
						olc::vi2d(GlobalState::GetPixelSize(), GlobalState::GetPixelSize()),
						olc::vf2d(vScale.x, vScale.y));

					auto eBlock = std::reinterpret_pointer_cast<Block>(cInventory->mStorage.at(nIndex));

					pge->DrawStringDecal(vDrawPos, std::to_string(eBlock->nBlockList.size()));
				}
				else
				{
					pge->DrawDecal(vDrawPos,
						mDecal);
					pge->DrawStringDecal(vDrawPos, std::to_string(1));
				}
			}			
		}
	}
}























IMM::MainMenu::MainMenu(int screenWidth, int screenHeight)
{
	float screenWidthScale = (float)screenWidth / DESIGN_SCREEN_WIDTH;
	float screenHeightScale = (float)screenHeight / DESIGN_SCREEN_HEIGHT;

	olc::vf2d startPosition = olc::vf2d(
		810 * screenWidthScale,
		200 * screenHeightScale);

	mHostGameButton = std::make_unique<Button>(
		"Assets/hostgameimg.png", 
		startPosition, 
		olc::vf2d(screenWidthScale, screenHeightScale));

	startPosition += olc::vi2d(0, mHostGameButton->GetScaledHeight() * 1.1f);

	mJoinGameButton = std::make_unique<Button>(
		"Assets/joingameimg.png",
		startPosition,
		olc::vf2d(screenWidthScale, screenHeightScale));
}

void IMM::MainMenu::Update(olc::PixelGameEngine* pge)
{
	//Hovered

	if (mHostGameButton->Hovered(pge->GetMouseX(), pge->GetMouseY()))
	{
		if (pge->GetMouse(0).bPressed)
		{
			//Send an Event

			IMM::Events::HostButtonPressedEvent HBPE;
			NotifyObservers(&HBPE);
		}
	}
	if (mJoinGameButton->Hovered(pge->GetMouseX(), pge->GetMouseY()))
	{
		if (pge->GetMouse(0).bPressed)
		{
			//Send an Event
			IMM::Events::JoinButtonPressedEvent JBPE;
			NotifyObservers(&JBPE);
		}
	}
}

void IMM::MainMenu::Render(olc::PixelGameEngine* pge)
{
	mHostGameButton->Render(pge);
	mJoinGameButton->Render(pge);
}


