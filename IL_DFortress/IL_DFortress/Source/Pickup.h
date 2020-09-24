#pragma once
#include "Global.h"

class Pickup
{
public:
	Pickup(EObjectType PickupType, class GamePlay*Game);



	//Pointer to active console
	shared_ptr<class DzX_Console> m_ActiveConsole;
	//Pointer to game mode
	shared_ptr<class GamePlay> m_GamePlay;
	//Shape of this item
	std::wstring m_look;
	
	EObjectType m_PickupType;

	COORD m_SpawnCoord;
	int m_EffectIntensity;

	bool bIsPickedUp;

	//Pickup event
	void OnPickup(class Player*NewOwner);
	//Start effect
	void Effect(class Player* EffectOnPlayer);
	void Spawn(COORD SpawnPos, shared_ptr<class DzX_Console> console);
	void Draw();



private:
	//Setup effects internaly
	void SetEffect();


};