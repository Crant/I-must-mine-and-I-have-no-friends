#pragma once

#include <random>

namespace IMM
{
	//namespace Utils
	//{
	//	namespace Random
	//	{
	//		std::mt19937 randomGen;
	//		unsigned int seed;
	//		void Srand(uint32_t seed)
	//		{
	//			randomGen = std::mt19937(seed);
	//		}

	//		void Srand(const std::string& pSeed)
	//		{
	//			seed = stoi(pSeed);
	//			//sscanf_s(pSeed.c_str(), "%d", &seed);
	//			randomGen = std::mt19937(seed);
	//		}

	//		unsigned int RandMax()
	//		{
	//			return randomGen.max();
	//		}

	//		unsigned int RandMin()
	//		{
	//			return randomGen.min();
	//		}

	//		int Seed()
	//		{
	//			return seed;
	//		}

	//		uint32_t Rand()
	//		{
	//			return randomGen();
	//		}
	//	}
	//}
	class Block
	{
	public:
		Block(unsigned int seed);
		~Block();

		float nIronAmount;
		float nCopperAmount;
		float nCoalAmount;
		float nQuartzAmount;

	private:


	};
	static class Random
	{
	public:
		Random() {}
		~Random(){}
		static void GenerateBlock(unsigned int seed);
		static unsigned int RandNumber(int pos, unsigned int seed); //Grundfunktionen för randomisering
		static void Srand(const std::string& seed);
		static unsigned int Rand() {return RandNumber(nPosInNoise++, nSeed);} //För helt random värden. Använd inte när du vill seeda planeter etc
		static float RandZeroToOne(){return (float)Rand() / UINT_MAX;}
		static float RandZeroToOne(unsigned int seed){return (float)RandNumber(seed, nSeed) / UINT_MAX;}
		static void ResetRand(){nPosInNoise = 0;}
		static unsigned int* GetWorldBlockSeeds(const std::string& seed, int nWidth, int nHeight);
		static float RangeMap(float input, float inputStart, float inputEnd, float outputStart, float outputEnd);
		static float CalculateRange(float input, float inputStart, float inputEnd, float outputStart, float outputEnd, float(*func)(float));
		static float SmoothStep(float t);
		static float SmoothStop(float t);
		static float SmoothStart(float t);
		static float Flip(float t);
		static float Mix(float a, float b, float weightB, float t);
		static float Scale(float(*func)(float), float t);
		static float ReverseScale(float(*func)(float), float t);
		static float Arch(float t);
		static float BellCurve(float t);
		static float BellCurve2(float t, float fDesiredStart, float fDesiredEnd);
		static float Bezier(float b, float c, float t);
		static float UpdateRange(float fInput, float fDesiredStart, float fDesiredEnd);
#pragma region InteImplementeratän
		static inline unsigned int Rnd2D(int posX, int posY, unsigned int seed)
		{
			constexpr int PRIME = 198491317;
			return RandNumber(posX + (PRIME * posY), seed);
		}



		float RndZeroToOne2D(int posX, int posY)
		{
			const float maxPlusOne = (1.0f / 2147483648.0f);
			int pos = posX + (posY * 57);
			int bits = (pos << 13) ^ pos;
			int pseudoRndPositiveInt = (bits * ((bits * bits * 15731) + 789221) + 1376312589) & 0x7fffffff;
			float zeroToOne = maxPlusOne * (float)pseudoRndPositiveInt;
			return zeroToOne;
		}


#pragma endregion
#pragma region PerlinNoise
		static float* Get1DPerlinNoise(const std::string& seed, int nWidth, int nHeight);
		static float* Get2DPerlinNoise(const std::string& seed, int nWidth, int nHeight);
#pragma endregion
	private:

		static inline int nPosInNoise = 0;
		static inline int nSeed = 198491317; //Detta är den allmänna seeden för randomisering.
											 //För att seeda planeter bör Srand INTE användas, utan den seeden skickas med i genereringen och sparas ner i mapfilen.
	};
}