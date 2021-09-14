#pragma once

#include <string>
#include <vector>

#define NR_OF_PERF_TIERS 6

namespace IMM
{
	namespace Utils
	{
		struct PerformanceMeasurement
		{
			std::string name;
			float lastClock;
			float totalTime;
			int measures;
			float maxTime;

			PerformanceMeasurement()
			{
				lastClock = 0.0f;
				name = "";
				totalTime = 0.0f;
				measures = 0;
				maxTime = 0.0f;
			}
		};

		class PerformanceTest
		{
		private:
			float mPCFreq;
			std::vector<PerformanceMeasurement> mPerfs[NR_OF_PERF_TIERS];
			std::string filePath;
		public:
			PerformanceTest();
			virtual ~PerformanceTest();

			void SetFilePath(const std::string& filePath) { this->filePath = filePath; }
			void PreMeasure(const std::string& perfName, int tier);
			void PostMeasure(const std::string& perfName, int tier);
			void GenerateReport();
			void ResetAll();
		};
	}
}



