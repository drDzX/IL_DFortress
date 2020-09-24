#pragma once
#include "Global.h"

class Pickup
{
public:
	Pickup(EObjectType PickupType);



	//Pointer to active console
	shared_ptr<class DzX_Console> m_ActiveConsole;

	//Shape of this item
	std::wstring m_look;
	
	EObjectType m_PickupType;

	CharStats PickupBoostStats;

	COORD m_SpawnCoord;
	int m_EffectIntensity;

	bool bIsPickedUp;

	//Pickup event
	void OnPickup(Player*NewOwner);
	//Start effect
	void Effect(Player* EffectOnPlayer);
	void Spawn(COORD SpawnPos, shared_ptr<class DzX_Console> console);
	void Draw();



private:
	//Setup effects internaly
	void SetEffect();


};