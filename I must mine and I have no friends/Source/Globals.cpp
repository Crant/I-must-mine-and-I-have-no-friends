#include "../Include/Globals.h"


using namespace IMM;

void GlobalState::Init(const olc::vf2d& vCameraInit, int nScreenWidth, int nScreenHeight, olc::PixelGameEngine* pge)
{
	data = new GlobalData();
	data->nPixelSize = 32;
	data->nTileSize = 8;
	data->vCamAdjust.x = (nScreenWidth / data->nTileSize) / 2;
	data->vCamAdjust.y = (nScreenHeight / data->nTileSize) / 2;
	data->vCamera = vCameraInit;
	data->vVisibleTiles.x = nScreenWidth / data->nTileSize;
	data->vVisibleTiles.y = nScreenHeight / data->nTileSize;
	data->vCamOff.x = vCameraInit.x - (float)data->vVisibleTiles.x;
	data->vCamOff.y = vCameraInit.y - (float)data->vVisibleTiles.y;
	data->vMiddleOfScreen = { (float)nScreenWidth / 2, (float)nScreenHeight / 2 };
	data->fTileScale = ((float)data->nTileSize / (float)data->nPixelSize);
	data->pge = pge;
}
void GlobalState::Update(const olc::vf2d& vPlayerPos)
{
	data->vCamera = vPlayerPos;
	data->vCamOff.x = data->vCamera.x - (float)data->vCamAdjust.x;
	data->vCamOff.y = data->vCamera.y - (float)data->vCamAdjust.y;
}
olc::vf2d GlobalState::LocalToWorld(const olc::vf2d& vLocalPos)
{

	olc::vf2d vWorldPos = { (vLocalPos.x / data->nTileSize) + data->vCamOff.x, (vLocalPos.y / data->nTileSize) + data->vCamOff.y };
	return vWorldPos;
}
olc::vf2d GlobalState::WorldToLocal(const olc::vf2d& vWorldPos)
{
	return vWorldPos - data->vCamOff;
}
olc::vf2d GlobalState::WorldToScreen(const olc::vf2d& vWorldPos)
{
	return (vWorldPos - data->vCamOff) * data->nTileSize;
}
olc::vf2d GlobalState::GetMouseWorld()
{
	olc::vf2d vMousePos;
	vMousePos.x = (float)data->pge->GetMouseX() / data->nTileSize + data->vCamOff.x;
	vMousePos.y = (float)data->pge->GetMouseY() / data->nTileSize + data->vCamOff.y;

	return vMousePos;

	//mMousePos.x = ((float)GetMouseX() / mTileSize) + mOffsetX; //HUR MAN FÅR TAG I MUSPEKAREN I WORLDSPACE FRÅN SKÄRM
	//mMousePos.y = ((float)GetMouseY() / mTileSize) + mOffsetY;
}
int GlobalState::GetTileSize()
{
	return data->nTileSize;
}
int GlobalState::GetPixelSize()
{
	return data->nPixelSize;
}
float GlobalState::GetTileScale()
{
	return data->fTileScale;
}
olc::vf2d GlobalState::GetScreenMid()
{
	return data->vMiddleOfScreen;
}
olc::vf2d GlobalState::GetCamera()
{
	return data->vCamera;
}
olc::vf2d GlobalState::GetCamOffset()
{
	return data->vCamOff;
}
olc::vi2d GlobalState::GetVisTiles()
{
	return data->vVisibleTiles;
}
olc::PixelGameEngine* GlobalState::GetEngine()
{
	return data->pge;
}
bool GlobalState::IsInView(const olc::vf2d& vWorldPos)
{
	olc::vf2d vScreenPos = WorldToScreen(vWorldPos);
	if (vScreenPos.x > - data->nPixelSize && vScreenPos.y > -data->nPixelSize && vScreenPos.x < data->pge->ScreenWidth() + data->nPixelSize && vScreenPos.y < data->pge->ScreenHeight() + data->nPixelSize)
	{
		return true;
	}
	else
	{
		return false;
	}
}
GlobalState::GlobalState()
{
}
GlobalState::~GlobalState()
{

}