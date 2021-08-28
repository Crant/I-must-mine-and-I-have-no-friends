#pragma once

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
	struct TileData
	{
		float fHitPoints;
		float fMiningHp;
		float fTimeAlive;
		TileData(float fHitPoints, float fMiningHp);
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

public:
    //public static TileData Dirt()
    //{
    //    return GetTile(TileType.Dirt);
    //}
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

//class TileNeighbour
//{
//public:
//	TileNeighbour(){}
//	~TileNeighbour(){}
//
//private:
//
//};
