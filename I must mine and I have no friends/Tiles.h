#pragma once
#include "Include/olcPixelGameEngine.h"

namespace IMM
{
	enum class TileType : unsigned char
	{
		Empty,
		Dirt,
		Stone,
	};
    enum class TileNeighbours : unsigned char
    {
        NONE,
        S,
        E,
        SE,
        N,
        NS,
        NE,
        NSE,
        W,
        WS,
        WE,
        WSE,
        WN,
        WNS,
        WNE,
        ALL
    };
	class TileData
	{
    public:
		float fHitPoints;
		float fMaxHP;
        TileData(float fHitPoints, float fMaxHP)
        {
            this->fHitPoints = fHitPoints;
            this->fMaxHP = fMaxHP;
        };
        TileData()
        {
            this->fHitPoints = 0.0f;
            this->fMaxHP = 0.0f;
        }
        ~TileData(){}
	};
    struct Tile
    {
        TileType type;
        TileNeighbours Nbrs;
    };


    class Tiles
    {
    private:
	    //static TileAssets tileAssets[];
        static inline std::unordered_map<TileType, TileData> mTypeToData;
        
    public:
        Tiles(){}
        ~Tiles(){}
        static void LoadTiles();
        static TileData GetData(TileType* tile);

        
        //public static TileData Stone()
        //{ 
        //    return GetTile(TileType.Stone);
        //}

        //public static TileData GetTile(TileType tile)
        //{
        //    for (int i = 0; i < tiles.Length; i++)
        //    {

        //        if (tile.ToString() == tiles[i].name)
        //        {
        //            return new TileData(tiles[i].hitPoints, tiles[i].miningStr);
        //        }

        //    }
        //    return new TileData(tiles[0].hitPoints, tiles[0].miningStr);
        //}

    };

}

