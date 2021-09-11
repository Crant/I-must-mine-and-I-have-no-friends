#include "World.h"

#include <queue>
#include <memory>

using namespace IMM;


//World::World(int width, int height, TileType* newWorld, std::string newName)
//{
//    nWidth = width;
//    nHeight = height;
//    nWorld = newWorld;
//    sWorldName = newName;
//    Instance = this;
//}
//World* World::GetWorld()
//{
//    return Instance;
//}

World::~World()
{
    delete[] nWorld;
    delete Instance;
}
void World::Init(int width, int height, Tile* newWorld, std::string newName)
{
    nWidth = width;
    nHeight = height;
    nSize = width * height;
    nWorld = newWorld;
    sWorldName = newName;
}
void World::SetTile(int index, TileType value)
{
    nWorld[index].type = value;
    SetNeighbours(index);
}
void World::SetTile(float x, float y, TileType value)
{
    nWorld[Index(x, y)].type = value;
    SetNeighbours(x, y);
}
void World::SetTile(olc::vf2d pos, TileType value)
{
    nWorld[Index(pos)].type = value;
    SetNeighbours(pos);
}
void World::SetTileGeneration(int index, TileType value) //USE ONLY IN GENERATION 
{
    nWorld[std::clamp(index, 0, nHeight * nWidth - 1)].type = value;
}

TileType* World::GetTile(int index)
{
    return &nWorld[index].type;
}
TileType* World::GetTile(int x, int y)
{
    return &nWorld[Index(x, y)].type;
}
TileType* World::GetTile(olc::vf2d pos)
{
    return &nWorld[Index(pos)].type;
}
TileNeighbours World::GetNbour(int index)
{
    return nWorld[std::clamp(index, 0, nHeight * nWidth - 1)].Nbrs;
}
TileNeighbours World::GetNbour(int x, int y)
{
    return nWorld[Index(x, y)].Nbrs;
}
TileNeighbours World::GetNbour(olc::vf2d pos)
{
    return nWorld[Index(pos)].Nbrs;
}
int World::GetWidth()
{
    return nWidth;
}
int World::GetHeight()
{
    return nHeight;
}
int World::GetSize()
{
    return nSize;
}

std::string World::GetName()
{
    return sWorldName;
}
int World::Index(float x, float y)
{
    return std::clamp(nHeight * (int)x + (int)y, 0, (nWidth * nHeight) - 1);
}
int World::Index(olc::vf2d pos)
{
    return std::clamp(nHeight * (int)pos.x + (int)pos.y, 0, (nWidth * nHeight) - 1);
}
olc::vi2d World::Coordinates(int flatIndex)
{
    return olc::vi2d(flatIndex / nHeight, flatIndex % nHeight);
}
bool World::IsInside(olc::vf2d tile, olc::vf2d check)
{
    return check.x >= tile.x && check.y >= tile.y && check.x < tile.x + 1 && check.y < tile.y + 1;
}
bool World::IsInside(int tileIndex, olc::vf2d check)
{
    olc::vi2d tile = Coordinates(tileIndex);
    return check.x >= tile.x && check.y >= tile.y && check.x < tile.x + 1 && check.y < tile.y + 1;
}
//bool World::IsOverlap(olc::vf2d tileOne, olc::vf2d tileTwo)
//{
//    return tileOne.x < tileTwo.x + 1 && tileOne.x + 1 > tileTwo.x && tileOne.y < tileTwo.y + 1 && tileOne.y + 1 > tileTwo.y;
//}
bool World::IsBlock(olc::vf2d pos)
{
    return (int)nWorld[Index(pos)].type >= 1;
}
bool World::IsBlock(float x, float y)
{
    return (int)nWorld[Index(x, y)].type >= 1;
}
bool World::IsBlock(int index)
{
    return (int)nWorld[std::clamp(index, 0, nWidth * nHeight)].type >= 1;
}
std::shared_ptr<std::vector<int>> World::FloodFill(int x, int y)
{
    std::shared_ptr<std::vector<int>> nFilledArea = std::make_shared<std::vector<int>>();
    std::unique_ptr<int[]> pFlags = std::make_unique<int[]>(nSize);
    std::queue<int> nQueue;
    nQueue.push(Index(x, y));
    
    while (!nQueue.empty())
    {
        int nTile = nQueue.front();
        nFilledArea->push_back(nTile);
        nQueue.pop();

        if (pFlags[std::clamp(nTile + 1, 0, nSize - 1)] == 0 && !IsBlock(std::clamp(nTile + 1, 0, nSize - 1)))
        {
            pFlags[std::clamp(nTile + 1, 0, nSize - 1)] = 1;
            nQueue.push(std::clamp(nTile + 1, 0, nSize - 1));
        }
        if (pFlags[std::clamp(nTile - 1, 0, nSize - 1)] == 0 && !IsBlock(std::clamp(nTile - 1, 0, nSize - 1)))
        {
            pFlags[std::clamp(nTile - 1, 0, nSize - 1)] = 1;
            nQueue.push(std::clamp(nTile - 1, 0, nSize - 1));
        }
        if (pFlags[std::clamp(nTile + nHeight, 0, nSize - 1)] == 0 && !IsBlock(std::clamp(nTile + nHeight, 0, nSize - 1)))
        {
            pFlags[std::clamp(nTile + nHeight, 0, nSize - 1)] = 1;
            nQueue.push(std::clamp(nTile + nHeight, 0, nSize - 1));
        }
        if (pFlags[std::clamp(nTile - nHeight, 0, nSize - 1)] == 0 && !IsBlock(std::clamp(nTile - nHeight, 0, nSize - 1)))
        {
            pFlags[std::clamp(nTile - nHeight, 0, nSize - 1)] = 1;
            nQueue.push(std::clamp(nTile - nHeight, 0, nSize - 1));
        }
    }
    return nFilledArea;
}
std::shared_ptr<std::vector<std::vector<int>>> World::GetRegions()
{
    std::shared_ptr<std::vector<std::vector<int>>> vRegions = std::make_shared<std::vector<std::vector<int>>>();
    std::unique_ptr<int[]> pFlags = std::make_unique<int[]>(nSize);

    for (size_t x = 0; x < nWidth; x++)
    {
        for (size_t y = 0; y < nHeight; y++)
        {
            if (pFlags[Index(x, y)] == 0 && !IsBlock(x, y))
            {
                vRegions->push_back(*FloodFill(x, y));

                for (size_t i = 0; i < vRegions->back().size(); i++)
                {
                    pFlags[vRegions->back()[i]] = 1;
                }
            }
        }
    }
    return vRegions;
}
void World::RemoveRegions()
{
    std::vector<std::vector<int>> vAllRegions = *GetRegions();
    std::shared_ptr<std::vector<std::vector<int>>> vRemainRegions = std::make_shared<std::vector<std::vector<int>>>();

    for (auto it = std::begin(vAllRegions); it != std::end(vAllRegions); it++)
    {
        if (it->size() < 150)
        {
            for (auto sit = std::begin(*it); sit != std::end(*it); sit++)
            {
                SetTile(*sit, TileType::Dirt);
            }
        }
        else
        {
            vRemainRegions->push_back(*it);
        }
    }
    //std::vector<int> funfun;
    //olc::vf2d vBestDistance;
    //olc::vf2d vFirstDistance;
    //olc::vf2d vSecondDistance;

    //

    //for (auto regionA = std::begin(*vRemainRegions); regionA != std::end(*vRemainRegions); regionA++)
    //{
    //    int& nRegionA = regionA->back();
    //    int& nRegionB = regionA->back();
    //    for (auto regionB = std::begin(*vRemainRegions); regionB != std::end(*vRemainRegions); regionB++)
    //    {

    //    }
    //}
    //for (auto it = std::begin(*vRemainRegions); it != std::end(*vRemainRegions); it++)
    //{
    //    //CreateRegion(*it);
    //    Region* cRegion = new Region(*it);
    //    
    //}

}
void World::CreateRegion(std::vector<int>& vRegionTiles)
{
    //std::unique_ptr<std::vector<int>> vEdgeTiles = std::make_unique<std::vector<int>>();
    //std::unique_ptr<Region> cRegion = std::make_unique<Region>();
    //cRegion->vRegionTiles = vRegionTiles;
    //for (int i = 0; i < vRegionTiles.size(); i++)
    //{
    //        if (IsBlock(i + 1) || IsBlock(i - 1) || IsBlock(i + nHeight) || IsBlock(i - nHeight))
    //        {
    //            //vEdgeTiles->push_back(std::clamp(i, 0, nSize - 1));
    //            cRegion->vEdgeTiles.push_back(std::clamp(i, 0, nSize - 1));
    //        }
    //        //else if (IsBlock(i - 1))
    //        //{
    //        //    cRegion->vEdgeTiles.push_back(std::clamp(i, 0, nSize - 1));
    //        //}
    //        //else if (IsBlock(i + nHeight))
    //        //{
    //        //    cRegion->vEdgeTiles.push_back(std::clamp(i, 0, nSize - 1));
    //        //}
    //        //else if (IsBlock(i - nHeight))
    //        //{
    //        //    cRegion->vEdgeTiles.push_back(std::clamp(i, 0, nSize - 1));
    //        //}
    //}
    //cRegions.push_back(*cRegion);
}

//struct Region
//{
//    std::vector<int> vRegionTiles;
//    std::vector<int> vEdgeTiles;
//    std::unique_ptr<std::vector<std::vector<int>>>  vCnctRegions;
//
//    Region(std::vector<int> vRegionTiles)
//    {
//        this->vRegionTiles = vRegionTiles;
//
//        for (size_t i = 0; i < vRegionTiles.size(); i++)
//        {
//            if (vRegionTiles[std::clamp(i + 1, 0, World::GetSize() - 1)] == 0 && !IsBlock(std::clamp(nTile + 1, 0, nSize - 1)))
//            {
//                pFlags[std::clamp(nTile + 1, 0, nSize - 1)] = 1;
//                nQueue.push(std::clamp(nTile + 1, 0, nSize - 1));
//            }
//            if (pFlags[std::clamp(nTile - 1, 0, nSize - 1)] == 0 && !IsBlock(std::clamp(nTile - 1, 0, nSize - 1)))
//            {
//                pFlags[std::clamp(nTile - 1, 0, nSize - 1)] = 1;
//                nQueue.push(std::clamp(nTile - 1, 0, nSize - 1));
//            }
//            if (pFlags[std::clamp(nTile + nHeight, 0, nSize - 1)] == 0 && !IsBlock(std::clamp(nTile + nHeight, 0, nSize - 1)))
//            {
//                pFlags[std::clamp(nTile + nHeight, 0, nSize - 1)] = 1;
//                nQueue.push(std::clamp(nTile + nHeight, 0, nSize - 1));
//            }
//            if (pFlags[std::clamp(nTile - nHeight, 0, nSize - 1)] == 0 && !IsBlock(std::clamp(nTile - nHeight, 0, nSize - 1)))
//            {
//                pFlags[std::clamp(nTile - nHeight, 0, nSize - 1)] = 1;
//                nQueue.push(std::clamp(nTile - nHeight, 0, nSize - 1));
//            }
//        }
//
//    }
//};




void World::SetNeighbours(float x, float y)
{
    int nTileNeighbours = 0;
    if (IsBlock(x, y + 1) > 0)
    {
        nTileNeighbours += 1;
        nWorld[Index(x, y + 1)].Nbrs = SetNeighboursNotRecursive(x, y + 1);
    }
    if (IsBlock(x + 1, y) > 0)
    {
        nTileNeighbours += 2;
        nWorld[Index(x + 1, y)].Nbrs = SetNeighboursNotRecursive(x + 1, y);
    }
    if (IsBlock(x, y - 1) > 0)
    {
        nTileNeighbours += 4;
        nWorld[Index(x, y - 1)].Nbrs = SetNeighboursNotRecursive(x, y - 1);
    }
    if (IsBlock(x - 1, y) > 0)
    {
        nTileNeighbours += 8;
        nWorld[Index(x - 1, y)].Nbrs = SetNeighboursNotRecursive(x - 1, y);
    }
    nWorld[Index(x, y)].Nbrs = (TileNeighbours)nTileNeighbours;
}
void World::SetNeighbours(olc::vf2d pos)
{
    int nTileNeighbours = 0;
    if (IsBlock(pos.x, pos.y + 1) > 0)
    {
        nTileNeighbours += 1;
        nWorld[Index(pos.x, pos.y + 1)].Nbrs = SetNeighboursNotRecursive(pos.x, pos.y + 1);
    }
    if (IsBlock(pos.x + 1, pos.y) > 0)
    {
        nTileNeighbours += 2;
        nWorld[Index(pos.x + 1, pos.y)].Nbrs = SetNeighboursNotRecursive(pos.x + 1, pos.y);
    }
    if (IsBlock(pos.x, pos.y - 1) > 0)
    {
        nTileNeighbours += 4;
        nWorld[Index(pos.x, pos.y - 1)].Nbrs = SetNeighboursNotRecursive(pos.x, pos.y - 1);
    }
    if (IsBlock(pos.x - 1, pos.y) > 0)
    {
        nTileNeighbours += 8;
        nWorld[Index(pos.x - 1, pos.y)].Nbrs = SetNeighboursNotRecursive(pos.x - 1, pos.y);
    }
    nWorld[Index(pos)].Nbrs = (TileNeighbours)nTileNeighbours;
}
void World::SetNeighbours(int index)
{
    int nTileNeighbours = 0;
    if (IsBlock(index + 1) > 0)
    {
        nTileNeighbours += 1;
        nWorld[index + 1].Nbrs = SetNeighboursNotRecursive(index + 1);
    }
    if (IsBlock(index + nWidth) > 0)
    {
        nTileNeighbours += 2;
        nWorld[index + nWidth].Nbrs = SetNeighboursNotRecursive(index + nWidth);
    }
    if (IsBlock(index - 1) > 0)
    {
        nTileNeighbours += 4;
        nWorld[index - 1].Nbrs = SetNeighboursNotRecursive(index - 1);
    }
    if (IsBlock(index - nWidth) > 0)
    {
        nTileNeighbours += 8;
        nWorld[index - nWidth].Nbrs = SetNeighboursNotRecursive(index - nWidth);
    }
    nWorld[index].Nbrs = (TileNeighbours)nTileNeighbours;
}
TileNeighbours World::SetNeighboursNotRecursive(float x, float y)
{
    int nTileNeighbours = 0;
    if (IsBlock(x, y + 1) > 0)
    {
        nTileNeighbours += 1;
    }
    if (IsBlock(x + 1, y) > 0)
    {
        nTileNeighbours += 2;
    }
    if (IsBlock(x, y - 1) > 0)
    {
        nTileNeighbours += 4;
    }
    if (IsBlock(x - 1, y) > 0)
    {
        nTileNeighbours += 8;
    }
    return (TileNeighbours)nTileNeighbours;
}
TileNeighbours World::SetNeighboursNotRecursive(olc::vf2d pos)
{
    int nTileNeighbours = 0;
    if (IsBlock(pos.x, pos.y + 1) > 0)
    {
        nTileNeighbours += 1;
    }
    if (IsBlock(pos.x + 1, pos.y) > 0)
    {
        nTileNeighbours += 2;
    }
    if (IsBlock(pos.x, pos.y - 1) > 0)
    {
        nTileNeighbours += 4;
    }
    if (IsBlock(pos.x - 1, pos.y) > 0)
    {
        nTileNeighbours += 8;
    }
    return (TileNeighbours)nTileNeighbours;
}
TileNeighbours World::SetNeighboursNotRecursive(int index)
{
    int nTileNeighbours = 0;
    if (IsBlock(index + 1) > 0)
    {
        nTileNeighbours += 1;
    }
    if (IsBlock(index + nWidth) > 0)
    {
        nTileNeighbours += 2;
    }
    if (IsBlock(index - 1) > 0)
    {
        nTileNeighbours += 4;
    }
    if (IsBlock(index - nWidth) > 0)
    {
        nTileNeighbours += 8;
    }
    return (TileNeighbours)nTileNeighbours;
}
void Region::ConnectRegions(Region& cRegionA, Region& cRegionB)
{
    cRegionA.vConnectedRegions->push_back(cRegionB);
    cRegionB.vConnectedRegions->push_back(cRegionA);
}
//bool Region::IsConnected(Region& cOtherRegion)
//{
//    auto aResult = std::find(this->vConnectedRegions->begin(), this->vConnectedRegions->end(), cOtherRegion);
//    if ()
//        return true;
//    else
//        return false;
//}
Region::Region(std::vector<int>& vRegionTiles)
{
    this->vRegionTiles = vRegionTiles;
    this->vEdgeTiles = new std::vector<int>;
    this->vConnectedRegions = new std::vector<Region>;

    for (int i = 0; i < vRegionTiles.size(); i++)
    {
        if (World::Main()->IsBlock(i + 1) || World::Main()->IsBlock(i - 1) || World::Main()->IsBlock(i + World::Main()->GetHeight()) || World::Main()->IsBlock(i - World::Main()->GetHeight()))
        {
            vEdgeTiles->push_back(i);
        }
    }
}
Region::~Region()
{
    delete vEdgeTiles;
}