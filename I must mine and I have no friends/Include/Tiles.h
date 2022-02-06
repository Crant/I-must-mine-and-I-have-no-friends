#pragma once
#include "olcPixelGameEngine.h"

namespace IMM
{
	enum class TileType : unsigned char
	{
		Empty, //DIRT
		Dirt,  //DIRT
        Clay,  //DIRT
        Sand,  //DIRT
        Silt,  //DIRT
        Chalk, //DIRT
        Peat,  //DIRT
		Basalt,     //VOLCANIC
        Granite,    //VOLCANIC
        Obsidian,   //VOLCANIC
        Limestone,  //SEDIMENTARY
        Sandstone,  //SEDIMENTARY
        Shale,      //SEDIMENTARY

        Lignite,    //COAL
        Bituminous, //COAL
        Anthracite,  //COAL

        Magnesite, //MINERAL
        Debug,
	};
    enum class TileProperties : unsigned char
    {
        None,
        Coal,
        Iron,
        Copper,
        Quartz,
        Magnesium,
        Cobalt,
        Nickel,
        Magnesium_Carbonate,
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

    struct BlockStats
    {
        std::string sType;
        TileProperties cCommonOre = TileProperties::None;
        TileProperties cGoodUncommonOre = TileProperties::None;
        TileProperties cBadUncommonOre = TileProperties::None;
        TileProperties cGoodRareOre = TileProperties::None;
        TileProperties cBadRareOre = TileProperties::None;
        TileProperties cGoodEpicOre = TileProperties::None;
        TileProperties cBadEpicOre = TileProperties::None;
    };

    struct BlockYield
    {
        std::string sCommonYield = "N/A";
        std::string sGoodUncommonYield = "N/A";
        std::string sBadUncommonYield = "N/A";
        std::string sGoodRareYield = "N/A";
        std::string sBadRareYield = "N/A";
        std::string sGoodEpicYield = "N/A";
        std::string sBadEpicYield = "N/A";
    };

    static class AllBlocks
    {
    public:
        AllBlocks(TileProperties cCommonYield, TileProperties cGoodUncommonYield, 
            TileProperties cBadUncommonYield, TileProperties cGoodRareYield, 
            TileProperties cBadRareYield, TileProperties cGoodEpicYield, TileProperties cBadEpicYield);
        ~AllBlocks();

        //static std::unordered_map<TileType, BlockStats*> mBlockDesc;
        static std::unique_ptr<BlockYield> GetBlockYield(float fPerlinSeed);
        static std::unique_ptr<std::string> CalculateBlockYield(float fRange);
        static void LoadBlocks();

    private:

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
        static TileData GetData(TileType tile);

        
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

