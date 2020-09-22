#pragma once
#include "Characters.h"
#include "Global.h"


class Player :public Character
{
public:
	Player();
	virtual void Spawn(class GamePlay* CurrentGame, shared_ptr<class DzX_Console>CurrentConsole, int spawnX, int spawnY)override;

	shared_ptr<class Pickup> Slot1;//HP
	shared_ptr<class Pickup> Slot2;//Armor
	shared_ptr<class Pickup> Slot3;//Weapon
	shared_ptr<class Pickup> Slot4;//Adrenaline

	void UseInventorySlot(int InventorySlot);

	void OverlappEvent();

};
