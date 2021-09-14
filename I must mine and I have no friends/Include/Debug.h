#pragma once

#include <string>
#include <fstream>

namespace IMM
{
	namespace Utils
	{
		void Debug(const std::string& msg, bool erase = false)
		{
			if (erase)
			{
				std::ofstream writeFile;
				writeFile.open("Debug.txt", std::ios::out | std::ios::trunc);

				writeFile << "";

				writeFile.close();
			}

			std::fstream writeFile;
			writeFile.open("Debug.txt", std::fstream::in | std::fstream::out | std::fstream::app);

			writeFile << msg << std::endl;

			writeFile.close();
		}

		void Debug(const float& nr, bool erase = false)
		{
			if (erase)
			{
				std::ofstream writeFile;
				writeFile.open("Debug.txt", std::ios::out | std::ios::trunc);

				writeFile << "";

				writeFile.close();
			}


			std::fstream writeFile;
			writeFile.open("Debug.txt", std::fstream::in | std::fstream::out | std::fstream::app);

			writeFile << nr << std::endl;

			writeFile.close();
		}

		void ClearDebug()
		{
			std::ofstream writeFile;
			writeFile.open("Debug.txt", std::ios::out | std::ios::trunc);

			writeFile << "";

			writeFile.close();
		}
	}
	
}