#pragma once

#include "Item.h"
class ActiveItem
{
private:
	float fDamage;
	Item cActiveItem;
public:
	void ItemUpdate();//KOLLAR VARJE FRAME IFALL SPELAREN ANV�NDER ITEMET
	void ItemChange();//�NDRAR VILKET ITEM SPELAREN ANV�NDER
};

