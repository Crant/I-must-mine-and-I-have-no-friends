#pragma once

#include <random>

namespace IMM
{
	namespace Utils
	{
		namespace Random
		{
			std::mt19937 randomGen;
			unsigned int seed;
			void Srand(uint32_t seed)
			{
				randomGen = std::mt19937(seed);
			}

			void Srand(const std::string& pSeed)
			{
				seed = stoi(pSeed);
				//sscanf_s(pSeed.c_str(), "%d", &seed);
				randomGen = std::mt19937(seed);
			}

			unsigned int RandMax()
			{
				return randomGen.max();
			}

			unsigned int RandMin()
			{
				return randomGen.min();
			}

			int Seed()
			{
				return seed;
			}

			uint32_t Rand()
			{
				return randomGen();
			}
		}
	}
}