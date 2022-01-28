#pragma once
#include "GameEngine.h"
namespace IMM
{

	class BluePrint
	{
	public:
		BluePrint(int nWidth, int nHeight, std::map<TileType, char> vTileSet, std::wstring sPattern);
		~BluePrint();

		std::wstring sPattern;
		std::map<TileType, char> vTileSet;

		int nWidth;
		int nHeight;

	private:

	};

	BluePrint::BluePrint(int nWidth, int nHeight, std::map<TileType, char> vTileSet,std::wstring sPattern)
	{
		this->nWidth = nWidth;
		this->nHeight = nHeight;
		this->vTileSet = vTileSet;

		this->sPattern = sPattern;
	}

	BluePrint::~BluePrint()
	{
	}

	class TestBuilding
	{
	public:
		TestBuilding();
		~TestBuilding();

	private:

		std::map<Building, BluePrint> mAllBluePrints;

		void WorkBench()
		{
		    std::map<TileType, char> map;
			map[TileType::Dirt] = '1';

			std::wstring sPattern;

			sPattern += L"000000";
			sPattern += L"011110";
			sPattern += L"010010";
			sPattern += L"######";

			BluePrint bp(6, 4, map, sPattern);

			mAllBluePrints[Building::Workbench] = bp;
		}
		void CheckBuilding(olc::vf2d pos, Building building)
		{
			BluePrint bp = mAllBluePrints[building];

			for (size_t x = 0; x < bp.nWidth; x++)
			{
				for (size_t y = 0; y < bp.nHeight; y++)
				{

				}
			}
		}
	};

	TestBuilding::TestBuilding()
	{
	}

	TestBuilding::~TestBuilding()
	{
	}

	enum Building
	{
		Workbench
	};

}