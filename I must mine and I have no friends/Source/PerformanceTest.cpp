#include "PerformanceTest.h"
#include <Windows.h>
#include <fstream>

IMM::Utils::PerformanceTest::PerformanceTest()
{
	LARGE_INTEGER li;
	QueryPerformanceFrequency(&li);
	mPCFreq = float(li.QuadPart) / 1000.0f;
	this->filePath = "MPR.txt";
}

IMM::Utils::PerformanceTest::~PerformanceTest()
{

}

void IMM::Utils::PerformanceTest::PreMeasure(const std::string& perfName, int tier)
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	float Timer = (li.QuadPart / mPCFreq);

	bool found = false;
	for (int i = 0; i < this->mPerfs[tier].size(); i++)
	{
		if (this->mPerfs[tier].at(i).name == perfName)
		{
			this->mPerfs[tier][i].lastClock = Timer;
			found = true;
		}
	}
	if (!found)
	{
		PerformanceMeasurement pm;
		pm.name = perfName;
		pm.totalTime = 0.0f;
		pm.measures = 1;
		pm.lastClock = Timer;
		pm.maxTime = 0.0f;
		this->mPerfs[tier].push_back(pm);
	}
}

void IMM::Utils::PerformanceTest::PostMeasure(const std::string& perfName, int tier)
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	float Timer = (li.QuadPart / mPCFreq);

	for (int i = 0; i < this->mPerfs[tier].size(); i++)
	{
		if (this->mPerfs[tier].at(i).name == perfName)
		{
			float timeDiff = Timer - this->mPerfs[tier][i].lastClock;
			this->mPerfs[tier][i].totalTime += timeDiff;
			this->mPerfs[tier][i].measures++;
			if (timeDiff > this->mPerfs[tier][i].maxTime)
				this->mPerfs[tier][i].maxTime = timeDiff;
		}
	}
}

void IMM::Utils::PerformanceTest::GenerateReport()
{
	std::fstream writeFile;
	writeFile.open(this->filePath, std::ios::out | std::ios::trunc);
	writeFile << "Performance report, times in milliseconds, IE 100 in the file = 0.1 seconds." << std::endl << std::endl;
	//writeFile << "Settings: " << endl;
	//writeFile << "Resolution: " << gep.WindowWidth << " x " << gep.WindowHeight << endl;
	//writeFile << "Shadow Quality: " << gep.ShadowMapSettings << endl;
	//writeFile << "FXAA Quality: " << gep.FXAAQuality << endl;
	//writeFile << "Field of View: " << gep.FOV << endl;
	//writeFile << "FarClip: " << gep.FarClip << endl;
	//writeFile << "MaxFPS: " << gep.MaxFPS << endl;
	//writeFile << "ShadowFit: " << gep.ShadowFit << endl;
	//writeFile << "Billboard Range: " << gep.BillboardRange << endl;
	writeFile << std::endl;

	//if (gep.MaxFPS > 0)
	//	writeFile << "WARNING, MAXFPS IS SET!" << endl << endl;
	//else
	//	writeFile << endl;

	for (int u = 0; u < NR_OF_PERF_TIERS; u++)
	{
		writeFile << "           Tier " << u + 1 << ": " << std::endl;
		for (int i = 0; i < this->mPerfs[u].size(); i++)
		{
			writeFile << this->mPerfs[u][i].name << ": " << std::endl << "Max: " << this->mPerfs[u][i].maxTime <<
				"     Avg: " << this->mPerfs[u][i].totalTime / this->mPerfs[u][i].measures << "     Tot:" <<
				this->mPerfs[u][i].totalTime << ", Measures: " << this->mPerfs[u][i].measures << std::endl << std::endl;
		}
		writeFile << std::endl;
	}

	writeFile.close();
}