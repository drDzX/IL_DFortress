#pragma once
#include "Global.h"

class Pickup
{
public:
	//Constructor
	Pickup(EObjectType PickupType, class GamePlay*Game);

	//Pickup event
	void OnPickup(class Player*NewOwner);
	//Start effect on player
	void Effect(class Player* EffectOnPlayer);
	//Sets spawn information
	void Spawn(COORD SpawnPos, shared_ptr<class DzX_Console> console);
	//Draw this object in the console
	void Draw();
private:
	//Setup effects internally
	void SetEffect();


/************************************************************************/
/*                        VARIABLES                                     */
/************************************************************************/
public:
	//Type of this pickup
	EObjectType m_PickupType;
	//Location to spawn
	COORD m_SpawnCoord;
	//Is picked up or not
	bool bIsPickedUp;
private:
	//Effect intensity to increase on player
	int m_EffectIntensity;
	//Pointer to active console
	shared_ptr<class DzX_Console> m_ActiveConsole;
	//Pointer to game mode
	shared_ptr<class GamePlay> m_GamePlay;
	//Shape of this item
	std::wstring m_look;

};