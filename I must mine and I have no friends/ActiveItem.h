#pragma once

#include "Item.h"
class ActiveItem
{
private:
	float fDamage;
	Item cActiveItem;
public:
	void ItemUpdate();//KOLLAR VARJE FRAME IFALL SPELAREN ANVÄNDER ITEMET
	void ItemChange();//ÄNDRAR VILKET ITEM SPELAREN ANVÄNDER
};

