#pragma once
#include "Characters.h"
#include "Global.h"
#include "Pickup.h"
struct InventorySlot
{
	EObjectType SlotType;
	int Amount;
	class Player* Owner;

	void ExecuteEffect()
	{
		if (Amount > 0)
		{
			unique_ptr<class Pickup> SlotEffect = make_unique<Pickup>(SlotType,nullptr);
			SlotEffect->Effect(Owner);
			Amount--;
		}
	};

	void Setup(Player* NewOwner, EObjectType NewType)
	{		
		Owner = NewOwner;

		SlotType = NewType;
	};
	InventorySlot()
	{
		SlotType = EObjectType::NONE;
		Amount = 0;
	}

};
class Player :public Character
{
public:
	Player();
	virtual void Spawn(class GamePlay* CurrentGame, shared_ptr<class DzX_Console>CurrentConsole, int spawnX, int spawnY)override;


	InventorySlot Slot1;//HP
	InventorySlot Slot2;//Armor
	InventorySlot Slot3;//Weapon

	void UseInventorySlot(int InventorySlot);

	void OverlappEvent();
	void PickupEvent();
	bool CheckForEnemiesAround(COORD &EnemyCoord);

	void Fight(class Enemy * Opponent);

};
