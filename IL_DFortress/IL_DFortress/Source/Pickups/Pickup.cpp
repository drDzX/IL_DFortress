#include "Pickup.h"
#include "Characters/Player.h"
#include "Game.h"

Pickup::Pickup(EObjectType PickupType, class GamePlay* Game)
{
	//Assign pickup type
	m_PickupType = PickupType;
	m_look = static_cast<char>(m_PickupType);

	bIsPickedUp = false;

	//Setup effect for this type of pickup
	SetEffect();

	if (Game)
	{
		//Set pointer to active game
		shared_ptr<class GamePlay> sp(Game);
		m_GamePlay = sp;
	}

}

void Pickup::OnPickup(Player* NewOwner)
{
	//Switch by type, and add to Player inventory acordingly
	switch (m_PickupType)
	{
	case EObjectType::HPPOTION:
		NewOwner->Slot1.Amount++;
		break;
	case EObjectType::ARMOR:
		NewOwner->Slot2.Amount = 1;
		Effect(NewOwner);
		break;
	case EObjectType::WEAPON:
		NewOwner->Slot3.Amount = 1;
		Effect(NewOwner);
		break;
	}
	//Change look on top of which is character to spawnable object
	NewOwner->m_currentField = static_cast<char>(EObjectType::SPAWNABLE);
	//object is picked up
	bIsPickedUp = true;

	//Destroy shape
	m_ActiveConsole->GoToXY(m_SpawnCoord.X, m_SpawnCoord.Y);
	cout << static_cast<char>(EObjectType::SPAWNABLE);
	if (m_PickupType == EObjectType::ARMOR || m_PickupType == EObjectType::WEAPON)
	{
		cout << static_cast<char>(EObjectType::SPAWNABLE);
		cout << static_cast<char>(EObjectType::SPAWNABLE);
		cout << static_cast<char>(EObjectType::SPAWNABLE);
	}
	if (m_GamePlay)
	{
		//Remove from pickups container
		m_GamePlay->RemovePickup(this);
	}
}

void Pickup::Effect(Player* EffectOnPlayer)
{
	//Switch by type and execute effect to player acordingly
	switch (m_PickupType)
	{
	case EObjectType::HPPOTION:
		EffectOnPlayer->UpdateHP(m_EffectIntensity);
		break;
	case EObjectType::ARMOR:
		EffectOnPlayer->UpdateDEF(m_EffectIntensity);
		break;
	case EObjectType::WEAPON:
		EffectOnPlayer->UpdateSTR(m_EffectIntensity);
		break;
	}
}

void Pickup::Spawn(COORD SpawnPos, shared_ptr<class DzX_Console> console)
{
	//Sets console pointer
	m_ActiveConsole = console;
	//Sets spawn location
	m_SpawnCoord = SpawnPos;
}

void Pickup::Draw()
{
	if (m_ActiveConsole && !bIsPickedUp)
	{
		//Sets pickup color
		m_ActiveConsole->SetColor(14);
		//Move cursor to location where this obj will be spawned
		m_ActiveConsole->GoToXY(m_SpawnCoord.X, m_SpawnCoord.Y);
		//Draw shape of this object
		std::wcout << m_look;
		//If weapon and armor add stats next to it
		if (m_PickupType == EObjectType::ARMOR || m_PickupType == EObjectType::WEAPON)
		{
			std::wcout << "+" << m_EffectIntensity;
		}
		//Reset color to default
		m_ActiveConsole->SetColor(7);
	}
}


void Pickup::SetEffect()
{
	//Switch by type and read stats from XML file acordingly
	switch (m_PickupType)
	{
	case EObjectType::HPPOTION:
	{
		int Value = stoi(ReadXML("Config/GamePlay_Settings.xml", "Pickup", "HPPot"));
		m_EffectIntensity = Value;
	}
	break;
	case EObjectType::ARMOR:
	{
		int Value = stoi(ReadXML("Config/GamePlay_Settings.xml", "Pickup", "Armor"));
		m_EffectIntensity = Value;
	}
	break;
	case EObjectType::WEAPON:
	{
		int Value = stoi(ReadXML("Config/GamePlay_Settings.xml", "Pickup", "Weapon"));
		m_EffectIntensity = Value;
	}
	break;
	}
}
