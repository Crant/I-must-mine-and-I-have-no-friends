#pragma once
#include "olcPixelGameEngine.h"

namespace IMM
{
	struct GlobalData
	{
		int nTileSize;
		int nPixelSize;
		float fTileScale; //för cool parallax effekt bara gångra med när du renderar ut det
		olc::vf2d vCamera;
		olc::vf2d vCamOff; //Short for camera with offset
		olc::vi2d vVisibleTiles;
		olc::vf2d vMiddleOfScreen;
		olc::vi2d vCamAdjust; // för att få kameran till mitten av spelaren. Hälften av offset
		olc::PixelGameEngine* pge;
	};

	static class GlobalState
	{
	public:
		GlobalState();
		~GlobalState();

		static void Init(const olc::vf2d& vCameraInit, int nScreenWidth, int nScreenHeight, olc::PixelGameEngine* pge);
		static void Update(const olc::vf2d& vPlayerPos);



		static olc::vf2d LocalToWorld(const olc::vf2d& vLocalPos);
		static olc::vf2d WorldToLocal(const olc::vf2d& vWorldPos);
		static olc::vf2d WorldToScreen(const olc::vf2d& vWorldPos);
		static int GetTileSize();
		static int GetPixelSize();
		static float GetTileScale();
		static olc::vf2d GetScreenMid();
		static olc::vf2d GetCamera(); 
		static olc::vf2d GetCamOffset();
		static olc::vi2d GetVisTiles();
		static olc::vf2d GetMouseWorld();
		static olc::PixelGameEngine* GetEngine();
		static bool IsInView(const olc::vf2d& vWorldPos);

	private:
		static std::unique_ptr<olc::vf2d> vPlayerPos;
		static inline GlobalData* data;

	};
}

