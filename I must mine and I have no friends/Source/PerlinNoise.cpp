//#include "PerlinNoise.h"
//#include "Game.h"
//
//void PerlinNoise::PerlinNoise1D(int nCount, float* fSeed, int nOctaves, float* fOutput)
//{
//	for (size_t x = 0; x < nCount; x++)
//	{
//		float fScale = 1.0f;
//		float fNoise = 0.0f;
//		float fScaleAcc = 0.0f;
//		for (size_t y = 0; y < nOctaves; y++)
//		{
//			int nPitch = nCount >> y;
//			int nSample1 = (x / nPitch) * nPitch;
//			int nSample2 = (nSample1 + nPitch) % nCount;
//
//			float fBlend = (float)(x - nSample1) / (float)nPitch;
//			float fSample = (1.0f - fBlend) * fSeed[nSample1] + fBlend * fSeed[nSample2];
//			fNoise += fSample * fScale;
//			fScaleAcc += fScale;
//
//			fScale / 2.0f;
//		}
//		fOutput[x] = fNoise / fScaleAcc;
//	}
//}
//
//PerlinNoise::PerlinNoise()
//{
//	nOutputSize = Game::Main()->ScreenWidth();
//	fNoiseSeed1D = new float[nOutputSize];
//	fPerlinNoise1D = new float[nOutputSize];
//
//	for (size_t i = 0; i < nOutputSize; i++)
//	{
//		fNoiseSeed1D[i] = (float)rand() / (float)RAND_MAX;
//	}
//}
//
//void PerlinNoise::PerlinTry()
//{
//	int* nTopTerrain = new int[100];
//
//	for (size_t i = 0; i < 100; i++)
//	{
//
//	}
//}