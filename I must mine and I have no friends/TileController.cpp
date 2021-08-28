//#include "TileController.h"
//using TileMap = std::unordered_map<int, TileData>;
//TileController::TileController()
//{
//    nKeys = new std::vector<int>;
//    nActiveTiles = new TileMap;
//}
//
//void TileController::CheckActiveTiles()
//{
//    //nkeys.AddRange(World.GetActiveKeys());
//
//    for (TileMap::const_iterator it = nActiveTiles->begin();it != nActiveTiles->end(); it++)
//    {
//        auto& key = it->first;
//
//        sTileData = it->second;
//        sTileData.fTimeAlive += f
//    }
//
//    //for (int i = 0; i < keys.Count; i++)
//    //{
//    //    //Debug.Log(keys[i] + " key at index " + i);
//    //    tile = World.GetTileData(keys[i]);
//    //    tile.timeAlive += Time.deltaTime;
//    //    World.SetTileData(keys[i], tile);
//    //    if (World.GetTileData(keys[i]).timeAlive >= resetTimer)
//    //    {
//    //        World.DeactivateTileData(keys[i]);
//    //    }
//    //    else if (World.GetTileData(keys[i]).miningHp <= 0 || World.GetTileData(keys[i]).hitPoints <= 0)
//    //    {
//    //        World.DeactivateTileData(keys[i]);
//    //        World.SetTile(keys[i], TileType.Empty);
//    //    }
//    //}
//    //keys.Clear();
//}