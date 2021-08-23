#pragma once
#include "olcPixelGameEngine.h";

namespace IMM 
{
	enum class TileType
	{
		Empty,
		Dirt,
		Stone
	};

	class World
	{
	public:
		//World(int width, int height, TileType* newWorld, std::string newName);
		World(){}
		~World();
		
		static World* GetWorld()
		{
			if (Instance == nullptr)
				Instance = new World();

			return Instance;
		}
		void SetWorld(int width, int height, TileType* newWorld, std::string name);
		void SetTile(int index, TileType value);
		void SetTile(olc::vf2d pos, TileType value);
		TileType GetTile(int index);
		TileType GetTile(int x, int y);
		TileType GetTile(olc::vf2d pos);
		int GetWidth();
		int GetHeight();
		int GetSize();
		int Index(float x, float y);
		int Index(olc::vf2d pos);
		olc::vi2d Coordinates(int flatIndex);
		bool IsInside(olc::vf2d tile, olc::vf2d check);
		bool IsInside(int tileIndex, olc::vf2d check);
		bool IsBlock(olc::vf2d pos);
		bool IsBlock(float x, float y);


	private:
		 static inline World* Instance;
		 TileType* nWorld;
		 std::string sWorldName;

		 int nWidth = 0;
		 int nHeight = 0;
		 int nSize = 0;
	};
}

