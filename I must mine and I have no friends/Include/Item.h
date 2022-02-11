#pragma once
#include "PhysicsObject.h"
#include "Tiles.h"

namespace IMM
{
    enum class ItemType : unsigned char //Står för både vilken sprite som ska målas ut och vad för sorts item det är
    {
        Block,
        Default,
        MiningLaser,
    };

    struct ItemData
    {

    };

    class Item
    {
    public:
        Item();
        Item(const std::string& sName, ItemType eItem);
        Item(const std::string& sName, ItemType eItem, bool bIsUpdated);
        Item(const std::string& sName, ItemType eItem, bool bIsUpdated, bool bIsStackable);
        virtual void Use(PhysUnit* cItemHolder) = 0;
        
        ItemType GetType();
        bool IsStackable();
        TileType GetTile();
        int nAmount;
        int nStackSize = 1000;

    protected:

        std::string sName;
        ItemType eItem;
        std::string sDescription;
        bool bIsStackable;
        bool bIsUpdated; //Ifall itemet ska uppdateras på något vis varje frame
        TileType eTile;

    private:
        //int id = -1;
        //int nAmount;
        //std::string sType;
    };

    class MiningLaser : public Item
    {
    public:
        using Item::Item;
        void Use(PhysUnit* cItemHolder) override;

    private:

    };
    class PhysObjShooter : public Item
    {
        using Item::Item;
        void Use(PhysUnit* cItemHolder) override;
    };
    ///////////////////
    ///
    ///
    /// 
    /// 
    /// 
    /// 
    /// 
    /// 
     //blocks
    class Block : public Item, public Updated
    {
    public:
        Block(const std::string& sName, TileType eTile, int nAmount);
        Block(TileType eTile, int nAmount);
        Block(TileType eTile);
        void Use(PhysUnit* cItemHolder) override;
        bool UpdateSelf() override;

    protected:

    private:

    };

}



