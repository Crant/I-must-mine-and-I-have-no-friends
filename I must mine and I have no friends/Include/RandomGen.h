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

			void Srand(std::string pSeed)
			{
				seed = sscanf_s(pSeed.c_str(), "%d", &seed);
				randomGen = std::mt19937(seed);
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