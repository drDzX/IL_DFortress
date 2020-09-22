#pragma once
#include "Global.h"

class Pickup
{
public:
	Pickup(EObjectType PickupType, shared_ptr<class DzX_Console> console);



	//Pointer to active console
	shared_ptr<class DzX_Console> m_ActiveConsole;

	//Shape of this item
	std::wstring m_look;
	
	EObjectType m_PickupType;

	CharStats PickupBoostStats;

	COORD m_SpawnCoord;
	int m_EffectIntensity;

	bool bIsPickedUp;
	shared_ptr<class Player> m_Owner;
	//Pickup event
	void OnPickup(Player*NewOwner);
	//Start effect
	void Effect();
	void Spawn(COORD SpawnPos);
	void Draw();
private:
	//Setup effects internaly
	void SetEffect();


};