#pragma once
#include "Include/olcPixelGameEngine.h"

namespace Maths
{
	struct GlobalConstants
	{
		olc::vf2d vScreenMiddle;
		int nTileSize;
	};
	static int nRandPos = 0;
	void GenerateBlock(unsigned int seed);
	unsigned int RandNumber(int pos, unsigned int seed); //Grundfunktionen för randomisering
	void Srand(const std::string& seed);
	static unsigned int Rand() { return RandNumber(nRandPos++, 13579); } //För helt random värden. Använd inte när du vill seeda planeter etc
	static float RandZeroToOne() { return (float)Rand() / UINT_MAX; }
	//static float RandZeroToOne(unsigned int seed) { return (float)RandNumber(seed, nSeed) / UINT_MAX; }
	//static void ResetRand() { nPosInNoise = 0; }
	unsigned int* GetWorldBlockSeeds(const std::string& seed, int nWidth, int nHeight);
	float RangeMap(float input, float inputStart, float inputEnd, float outputStart, float outputEnd);
	float CalculateRange(float input, float inputStart, float inputEnd, float outputStart, float outputEnd, float(*func)(float));
	float SmoothStep(float t);
	float SmoothStop(float t);
	float SmoothStart(float t);
	float Flip(float t);
	float Mix(float a, float b, float weightB, float t);
	float Scale(float(*func)(float), float t);
	float ReverseScale(float(*func)(float), float t);
	float Arch(float t);
	float BellCurve(float t);
	float BellCurve2(float t, float fDesiredStart, float fDesiredEnd);
	float Bezier(float b, float c, float t);
	float UpdateRange(float fInput, float fDesiredStart, float fDesiredEnd);

	float* Get1DPerlinNoise(const std::string& seed, int nWidth, int nHeight);
	float* Get2DPerlinNoise(const std::string& seed, int nWidth, int nHeight);

	olc::vf2d LocalToWorld(const olc::vf2d& vLocalPos, int nTileSize, const olc::vf2d& vCameraOffset);
	olc::vf2d WorldToLocal(const olc::vf2d& vWorldPos, const olc::vf2d& vCameraOffset);
	olc::vf2d WorldToScreen(const olc::vf2d& vWorldPos, int nTileSize, const olc::vf2d& vCameraOffset);
	float Distance(const olc::vf2d& vPos1, const olc::vf2d& vPos2);

}
