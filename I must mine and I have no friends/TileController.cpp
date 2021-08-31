#include "TileController.h"
#include "../I must mine and I have no friends/Include/olcPixelGameEngine.h"

TileController::TileController()
{
}

void TileController::CheckActiveTiles()
{
    for (std::unordered_map<int, TileData>::const_iterator it = nActiveTiles.begin();it != nActiveTiles.end();)
    {
        auto& key = it->first;
        nActiveTiles[key].fHitPoints += nActiveTiles[key].fHitPoints * 0.02f;
        if (nActiveTiles[key].fHitPoints >= nActiveTiles[key].fMaxHP)
        {
            nActiveTiles.erase(it++);
        }
        else if (nActiveTiles[key].fHitPoints <= 0)
        {
            World::Main()->SetTile(key, TileType::Empty);
            nActiveTiles.erase(it++);
        }
        else
        {
            ++it;
        }
    }
}

void TileController::DamageBlock(olc::vf2d fBlockPos, float fDmg)
{
    if (!World::Main()->IsBlock(fBlockPos))
        return;

    int tileIndex = World::Main()->Index(fBlockPos);
    if (nActiveTiles.find(tileIndex) == nActiveTiles.end())
    {
        TileData* nTile = new TileData(Tiles::GetData(World::Main()->GetTile(tileIndex)).fHitPoints, Tiles::GetData(World::Main()->GetTile(tileIndex)).fMaxHP);
        nActiveTiles[tileIndex] = *nTile;

    }       
    nActiveTiles[tileIndex].fHitPoints -= fDmg;
}

void TileController::DamageBlockAOE(olc::vf2d fBlockPos, float fDmg, int nAoe)
{

}

 void TileController::CreateBlock(olc::vf2d pos, TileType tile)
{
    if (!World::Main()->IsBlock(pos))
    {
        World::Main()->SetTile(pos, tile);
    }
}