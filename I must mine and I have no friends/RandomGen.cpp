//#include "Include/RandomGen.h"
//#include "Include/World.h"
//#include "Include/Tiles.h"
//
//using namespace IMM;
//
//Block::Block(unsigned int seed)
//{
//	std::hash<std::string> hasher;
//	auto oreHash = hasher("Iron");
//
//	nIronAmount = (float)Random::RandNumber(seed, oreHash) / UINT_MAX;
//
//	oreHash = hasher("Copper");
//
//	nCopperAmount = (float)Random::RandNumber(seed, oreHash) / UINT_MAX;
//
//	oreHash = hasher("Coal");
//
//	nCoalAmount = (float)Random::RandNumber(seed, oreHash) / UINT_MAX;
//
//	oreHash = hasher("Quartz");
//
//	nQuartzAmount = (float)Random::RandNumber(seed, oreHash) / UINT_MAX;
//}
//Block::~Block()
//{
//
//}
//void Random::GenerateBlock(unsigned int seed)
//{
//	Block* meme = new Block(seed);
//
//
//}
//unsigned int* Random::GetWorldBlockSeeds(const std::string& seed, int nWidth, int nHeight)
//{
//	std::hash<std::string> seedHasher;
//	auto seedHash = seedHasher(seed);
//
//	unsigned int* WorldSeeds = new unsigned int[nWidth * nHeight];
//
//	for (int i = 0; i < nWidth * nHeight; i++)
//	{
//		if (World::Main()->IsBlock(i))
//		{
//			WorldSeeds[i] = RandNumber(i, seedHash);
//		}
//	}
//	return WorldSeeds;
//}
//unsigned int Random::RandNumber(int pos, unsigned int seed) //Grundfunktionen för randomisering
//{
//	constexpr unsigned int BIT_NOISE1 = 0xB5297A4D;
//	constexpr unsigned int BIT_NOISE2 = 0x68E31DA4;
//	constexpr unsigned int BIT_NOISE3 = 0x1B56C4E9;
//
//	unsigned int mangled = (unsigned int)pos;
//	mangled *= BIT_NOISE1;
//	mangled += seed;
//	mangled ^= (mangled >> 8);
//	mangled += BIT_NOISE2;
//	mangled ^= (mangled << 8);
//	mangled *= BIT_NOISE3;
//	mangled ^= (mangled >> 8);
//	return mangled;
//	//STOLEN FROM SQUIRREL EISERLOH
//}
//void Random::Srand(const std::string& seed)
//{
//	nPosInNoise = 0;
//
//	std::hash<std::string> seedHasher;
//	auto seedHash = seedHasher(seed);
//	nSeed = seedHash;
//}
//float* Random::Get1DPerlinNoise(const std::string& seed, int nWidth, int nHeight)
//{
//	//Width = Count
//	//Height = Höjden på världen oftast
//	//Octaves är 8 per default
//
//	float* nPerlinWalk = new float[nWidth];
//	float* nRandomWalk = new float[nWidth];
//
//	std::hash<std::string> seedHasher; //Hashen för seed till randomisering
//	auto seedHash = seedHasher(seed);
//
//	for (size_t i = 0; i < nWidth; i++)
//	{
//		nRandomWalk[i] = (float)RandNumber(i, seedHash) / UINT_MAX;
//		//nRandomWalk[i] = RandZeroToOne();
//	}
//
//	nRandomWalk[0] = 0.5f; //ser till att den alltid hamnar i mitten
//
//	for (int x = 0; x < nWidth; x++)
//	{
//		float fScale = 1.0f;
//		float fNoise = 0.0f;
//		float fScaleAcc = 0.0f;
//		float fBias = 2.0f;
//		for (int y = 0; y < 8; y++)
//		{
//			int nPitch = nWidth >> y;
//			int nSample1 = (x / nPitch) * nPitch;
//			int nSample2 = (nSample1 + nPitch) % nWidth;
//
//			float fBlend = (float)(x - nSample1) / (float)nPitch;
//			float fSample = (1.0f - fBlend) * nRandomWalk[nSample1] + fBlend * nRandomWalk[nSample2];
//			fNoise += fSample * fScale;
//			fScaleAcc += fScale;
//
//			fScale = fScale / fBias;
//		}
//		nPerlinWalk[x] = (fNoise / fScaleAcc) * nHeight;
//	}
//	delete[] nRandomWalk;
//	return nPerlinWalk;
//}
//float* Random::Get2DPerlinNoise(const std::string& seed, int nWidth, int nHeight)
//{
//	//Width = Count
//	//Height = Höjden på världen oftast
//	//Octaves är 8 per default
//
//	float* nPerlinNoise2D = new float[nWidth * nHeight];
//	float* nRandomNoise2D = new float[nWidth * nHeight];
//
//	std::hash<std::string> seedHasher; //Hashen för seed till randomisering
//	auto seedHash = seedHasher(seed);
//
//	for (size_t i = 0; i < nWidth * nHeight; i++)
//	{
//		nRandomNoise2D[i] = (float)RandNumber(i, seedHash) / UINT_MAX;
//	}
//	//return nRandomNoise2D;
//	nRandomNoise2D[0] = 0.5f; //se till att den alltid hamnar i mitten
//
//	for (int x = 0; x < nWidth; x++)
//	{
//		for (int y = 0; y < nHeight; y++)
//		{
//			float fScale = 1.0f;
//			float fNoise = 0.0f;
//			float fScaleAcc = 0.0f;
//			float fBias = 1.f;
//			for (int o = 0; o < 8; o++)
//			{
//				int nPitch = nWidth >> o;
//				int nSampleX1 = (x / nPitch) * nPitch;
//				int nSampleY1 = (y / nPitch) * nPitch;
//
//				int nSampleX2 = (nSampleX1 + nPitch) % nWidth;
//				int nSampleY2 = (nSampleY1 + nPitch) % nWidth;
//
//				float fBlendX = (float)(x - nSampleX1) / (float)nPitch;
//				float fBlendY = (float)(y - nSampleY1) / (float)nPitch;
//
//				float fSampleT = (1.0f - fBlendX) * nRandomNoise2D[nSampleY1 * nWidth + nSampleX1] + fBlendX * nRandomNoise2D[nSampleY1 * nWidth + nSampleX2];
//				float fSampleB = (1.0f - fBlendX) * nRandomNoise2D[nSampleY2 * nWidth + nSampleX1] + fBlendX * nRandomNoise2D[nSampleY2 * nWidth + nSampleX2];
//				fNoise += (fBlendY * (fSampleB - fSampleT) + fSampleT) * fScale;
//				fScaleAcc += fScale;
//
//				fScale = fScale / fBias;
//			}
//			nPerlinNoise2D[x * nHeight + y] = fNoise / fScaleAcc; // kanske måste ändra till x * nheight + y istället
//			//nPerlinNoise2D[y * nWidth + x] = (fNoise / fScaleAcc) * nHeight; // kanske måste ändra till x * nheight + y istället
//		}
//
//	}
//	delete[] nRandomNoise2D;
//	return nPerlinNoise2D;
//}
//float Random::RangeMap(float input, float inputStart, float inputEnd, float outputStart, float outputEnd)
//{
//	float output = input - inputStart;
//	output /= (inputEnd - inputStart);
//	output = SmoothStep(output);
//	output *= (outputEnd - outputStart);
//	return output + outputStart;
//}
//float Random::SmoothStep(float t) //Default 3 gånger gångrad
//{
//	float inputSquared = (t * t);
//	return (3.0f * inputSquared) - (2.0f * inputSquared * inputSquared);
//}
//float Random::SmoothStart(float t) //Default 3 gånger gångrad
//{
//	return t * t * t;
//}
//float Random::SmoothStop(float t) //Default 3 gånger gångrad
//{
//	float flipped = Flip(t);
//	return 1.f - (flipped * flipped * flipped);
//}
//float Random::Flip(float t)
//{
//	return 1.f - t;
//}
//float Random::Mix(float smoothStart, float smoothStop, float blendPercentage, float t)
//{
//	return (1.f - blendPercentage) * smoothStart + (blendPercentage)*smoothStop;
//}
//float Random::Scale(float(*func)(float), float t)
//{
//	return t * func(t);
//}
//float Random::ReverseScale(float(*func)(float), float t)
//{
//	return (1.f - t) * func(t);
//}
//float Random::Arch(float t)
//{
//	return (t * (1.f - t)) * 4; //Gångra med 4 för att normalisera mellan 0 och 1
//}
//float Random::BellCurve(float t)
//{
//	return ((1.f - t) * t) * ((1.f - t) * t) * 16; //Gångra med 16 för att normalisera mellan 0 och 1
//}
//float Random::BellCurve2(float t, float fDesiredStart, float fDesiredEnd)
//{
//	float fUpdated = UpdateRange(t, fDesiredStart, fDesiredEnd);
//	return ((1.f - fUpdated) * fUpdated) * ((1.f - fUpdated) * fUpdated) * 16;
//}
//float Random::Bezier(float b, float c, float t)
//{
//	float s = 1.f - t;
//	float t2 = t * t;
//	float s2 = s * s;
//	float t3 = t2 * t;
//	return (3.f * b * s2 * t) + (3.f * c * s * t2) + (t3);
//}
//float Random::CalculateRange(float input, float inputStart, float inputEnd, float outputStart, float outputEnd, float(*func)(float))
//{
//	float output = input - inputStart;
//	output /= (inputEnd - inputStart);
//	output = func(output);
//	output *= (outputEnd - outputStart);
//	return output + outputStart;
//}
//float Random::UpdateRange(float fInput, float fDesiredStart, float fDesiredEnd)
//{
//	float output = fInput - fDesiredStart;
//	output /= (fDesiredEnd - fDesiredStart), 0.f, 1.f;
//	output = std::clamp(output, 0.f, 1.f);
//	return output;
//}
////float Gauss(float x)
////{
////	
////}