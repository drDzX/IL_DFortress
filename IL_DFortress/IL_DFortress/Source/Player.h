#pragma once
#include "Characters.h"
#include "Global.h"
#include "Pickup.h"

struct InventorySlot
{
	EObjectType SlotType;
	int Amount;
	unique_ptr<class Player> Owner;

	void ExecuteEffect()
	{
		if (Amount > 0)
		{
			unique_ptr<class Pickup> SlotEffect = make_unique<Pickup>(SlotType);
			SlotEffect->Effect(Owner.get());
			Amount--;
		}
	};

	void Setup(class Player* NewOwner, EObjectType NewType)
	{
		unique_ptr<class Player> _Owner(NewOwner);
		Owner = std::move(_Owner);

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

	void Fight(Enemy * Opponent);

};
