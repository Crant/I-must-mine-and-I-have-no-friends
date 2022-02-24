#include "Maths.h"
#include "Include/World.h"

using namespace Maths;
using namespace IMM;


#pragma region Generation



unsigned int* Maths::GetWorldBlockSeeds(const std::string& seed, int nWidth, int nHeight)
{
	std::hash<std::string> seedHasher;
	auto seedHash = seedHasher(seed);

	unsigned int* WorldSeeds = new unsigned int[nWidth * nHeight];

	for (int i = 0; i < nWidth * nHeight; i++)
	{
		if (World::Main()->IsBlock(i))
		{
			WorldSeeds[i] = RandNumber(i, seedHash);
		}
	}
	return WorldSeeds;
}
unsigned int Maths::RandNumber(int pos, unsigned int seed) //Grundfunktionen f�r randomisering
{
	constexpr unsigned int BIT_NOISE1 = 0xB5297A4D;
	constexpr unsigned int BIT_NOISE2 = 0x68E31DA4;
	constexpr unsigned int BIT_NOISE3 = 0x1B56C4E9;

	unsigned int mangled = (unsigned int)pos;
	mangled *= BIT_NOISE1;
	mangled += seed;
	mangled ^= (mangled >> 8);
	mangled += BIT_NOISE2;
	mangled ^= (mangled << 8);
	mangled *= BIT_NOISE3;
	mangled ^= (mangled >> 8);
	return mangled;
	//STOLEN FROM SQUIRREL EISERLOH
}
//void Maths::Srand(const std::string& seed)
//{
//	nPosInNoise = 0;
//
//	std::hash<std::string> seedHasher;
//	auto seedHash = seedHasher(seed);
//	nSeed = seedHash;
//}
float* Maths::Get1DPerlinNoise(const std::string& seed, int nWidth, int nHeight)
{
	//Width = Count
	//Height = H�jden p� v�rlden oftast
	//Octaves �r 8 per default

	float* nPerlinWalk = new float[nWidth];
	float* nRandomWalk = new float[nWidth];

	std::hash<std::string> seedHasher; //Hashen f�r seed till randomisering
	auto seedHash = seedHasher(seed);

	for (size_t i = 0; i < nWidth; i++)
	{
		nRandomWalk[i] = (float)RandNumber(i, seedHash) / UINT_MAX;
		//nRandomWalk[i] = RandZeroToOne();
	}

	nRandomWalk[0] = 0.5f; //ser till att den alltid hamnar i mitten

	for (int x = 0; x < nWidth; x++)
	{
		float fScale = 1.0f;
		float fNoise = 0.0f;
		float fScaleAcc = 0.0f;
		float fBias = 2.0f;
		for (int y = 0; y < 8; y++)
		{
			int nPitch = nWidth >> y;
			int nSample1 = (x / nPitch) * nPitch;
			int nSample2 = (nSample1 + nPitch) % nWidth;

			float fBlend = (float)(x - nSample1) / (float)nPitch;
			float fSample = (1.0f - fBlend) * nRandomWalk[nSample1] + fBlend * nRandomWalk[nSample2];
			fNoise += fSample * fScale;
			fScaleAcc += fScale;

			fScale = fScale / fBias;
		}
		nPerlinWalk[x] = (fNoise / fScaleAcc) * nHeight;
	}
	delete[] nRandomWalk;
	return nPerlinWalk;
}
std::shared_ptr<std::vector<float>> Maths::Get2DPerlinNoise(const std::string& seed, int nWidth, int nHeight)
{
	//Width = Count
	//Height = H�jden p� v�rlden oftast
	//Octaves �r 8 per default

	std::shared_ptr<std::vector<float>> fPerlinNoise2D = std::make_shared<std::vector<float>>(nWidth * nHeight);
	float* nRandomNoise2D = new float[nWidth * nHeight];


	std::hash<std::string> seedHasher; //Hashen f�r seed till randomisering
	auto seedHash = seedHasher(seed);

	for (size_t i = 0; i < nWidth * nHeight; i++)
	{
		nRandomNoise2D[i] = (float)RandNumber(i, seedHash) / UINT_MAX;
	}
	//return nRandomNoise2D;
	nRandomNoise2D[0] = 0.5f; //se till att den alltid hamnar i mitten

	for (int x = 0; x < nWidth; x++)
	{
		for (int y = 0; y < nHeight; y++)
		{
			float fScale = 1.0f;
			float fNoise = 0.0f;
			float fScaleAcc = 0.0f;
			float fBias = 1.f;
			for (int o = 0; o < 8; o++)
			{
				int nPitch = nWidth >> o;
				int nSampleX1 = (x / nPitch) * nPitch;
				int nSampleY1 = (y / nPitch) * nPitch;

				int nSampleX2 = (nSampleX1 + nPitch) % nWidth;
				int nSampleY2 = (nSampleY1 + nPitch) % nWidth;

				float fBlendX = (float)(x - nSampleX1) / (float)nPitch;
				float fBlendY = (float)(y - nSampleY1) / (float)nPitch;

				float fSampleT = (1.0f - fBlendX) * nRandomNoise2D[nSampleY1 * nWidth + nSampleX1] + fBlendX * nRandomNoise2D[nSampleY1 * nWidth + nSampleX2];
				float fSampleB = (1.0f - fBlendX) * nRandomNoise2D[nSampleY2 * nWidth + nSampleX1] + fBlendX * nRandomNoise2D[nSampleY2 * nWidth + nSampleX2];
				fNoise += (fBlendY * (fSampleB - fSampleT) + fSampleT) * fScale;
				fScaleAcc += fScale;

				fScale = fScale / fBias;
			} 
			fPerlinNoise2D->at(x * nHeight + y) = fNoise / fScaleAcc; // kanske m�ste �ndra till x * nheight + y ist�llet
			//nPerlinNoise2D[y * nWidth + x] = (fNoise / fScaleAcc) * nHeight; // kanske m�ste �ndra till x * nheight + y ist�llet
		}

	}
	delete[] nRandomNoise2D;
	return fPerlinNoise2D;
}
#pragma endregion
#pragma region Smoothing



float Maths::RangeMap(float input, float inputStart, float inputEnd, float outputStart, float outputEnd)
{
	float output = input - inputStart;
	output /= (inputEnd - inputStart);
	output = SmoothStep(output);
	output *= (outputEnd - outputStart);
	return output + outputStart;
}
float Maths::SmoothStep(float t) //Default 3 g�nger g�ngrad
{
	float inputSquared = (t * t);
	return (3.0f * inputSquared) - (2.0f * inputSquared * inputSquared);
}
float Maths::SmoothStart(float t) //Default 3 g�nger g�ngrad
{
	return t * t * t;
}
float Maths::SmoothStop(float t) //Default 3 g�nger g�ngrad
{
	float flipped = Flip(t);
	return 1.f - (flipped * flipped * flipped);
}
float Maths::Flip(float t)
{
	return 1.f - t;
}
float Maths::Mix(float smoothStart, float smoothStop, float blendPercentage, float t)
{
	return (1.f - blendPercentage) * smoothStart + (blendPercentage)*smoothStop;
}
float Maths::Scale(float(*func)(float), float t)
{
	return t * func(t);
}
float Maths::ReverseScale(float(*func)(float), float t)
{
	return (1.f - t) * func(t);
}
float Maths::Arch(float t)
{
	return (t * (1.f - t)) * 4; //G�ngra med 4 f�r att normalisera mellan 0 och 1
}
float Maths::BellCurve(float t)
{
	return ((1.f - t) * t) * ((1.f - t) * t) * 16; //G�ngra med 16 f�r att normalisera mellan 0 och 1
}
float Maths::BellCurve2(float t, float fDesiredStart, float fDesiredEnd)
{
	float fUpdated = UpdateRange(t, fDesiredStart, fDesiredEnd);
	return ((1.f - fUpdated) * fUpdated) * ((1.f - fUpdated) * fUpdated) * 16;
}
float Maths::Bezier(float b, float c, float t)
{
	float s = 1.f - t;
	float t2 = t * t;
	float s2 = s * s;
	float t3 = t2 * t;
	return (3.f * b * s2 * t) + (3.f * c * s * t2) + (t3);
}
float Maths::CalculateRange(float input, float inputStart, float inputEnd, float outputStart, float outputEnd, float(*func)(float))
{
	float output = input - inputStart;
	output /= (inputEnd - inputStart);
	output = func(output);
	output *= (outputEnd - outputStart);
	return output + outputStart;
}
float Maths::UpdateRange(float fInput, float fDesiredStart, float fDesiredEnd)
{
	float output = fInput - fDesiredStart;
	output /= (fDesiredEnd - fDesiredStart), 0.f, 1.f;
	output = std::clamp(output, 0.f, 1.f);
	return output;
}
#pragma endregion
olc::vf2d Maths::LocalToWorld(const olc::vf2d& vLocalPos, int nTileSize, const olc::vf2d& vCameraOffset)
{
	
	olc::vf2d vWorldPos = { (vLocalPos.x / nTileSize) + vCameraOffset.x, (vLocalPos.y / nTileSize) + vCameraOffset.y };
	return vWorldPos;
}
olc::vf2d Maths::WorldToLocal(const olc::vf2d& vWorldPos, const olc::vf2d& vCameraOffset)
{
	return vWorldPos - vCameraOffset;
}
olc::vf2d Maths::WorldToScreen(const olc::vf2d& vWorldPos, int nTileSize, const olc::vf2d& vCameraOffset)
{
	return (vWorldPos - vCameraOffset) * nTileSize;
}
float Maths::Distance(const olc::vf2d& vPos1, const olc::vf2d& vPos2)
{
	olc::vf2d vDist = vPos2 - vPos1;

	return sqrt((vDist.x * vDist.x) + (vDist.y * vDist.y));
}