#pragma once
#include "Characters.h"
#include "Global.h"
#include "Pickups/Pickup.h"

//Inventory slots
struct InventorySlot
{
	//Type of slot(what does it contain)
	EObjectType SlotType;
	//How much items contain
	int Amount;
	//Who is owner - pointer
	class Player* Owner;

	//Execute effect on owner
	void ExecuteEffect()
	{
		//If there are charges
		if (Amount > 0)
		{
			unique_ptr<class Pickup> SlotEffect = make_unique<Pickup>(SlotType,nullptr);
			SlotEffect->Effect(Owner);
			//Remove charge
			Amount--;
		}
	};
	//Setup inventory slot
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
	//Constructor
	Player();
	virtual void Spawn(class GamePlay* CurrentGame, shared_ptr<class DzX_Console>CurrentConsole, int spawnX, int spawnY)override;
	//Use effect from inventory item
	void UseInventorySlot(int InventorySlot);
	//Check for overlaps around player
	void OverlappEvent();
	//Pickup dropped elements under player
	void PickupEvent();
private:
	//Check if there are enemies around
	bool CheckForEnemiesAround(COORD &EnemyCoord);	
	//Fight enemy
	void Fight(class Enemy * Opponent);
	//Death of player
	virtual void Die()override;

/************************************************************************/
/*                        VARIABLES                                     */
/************************************************************************/
public:
	InventorySlot Slot1;//HP
	InventorySlot Slot2;//Armor
	InventorySlot Slot3;//Weapon

};
