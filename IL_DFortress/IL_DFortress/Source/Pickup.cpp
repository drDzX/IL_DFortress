#include "Pickup.h"
#include "Player.h"


Pickup::Pickup(EObjectType PickupType, shared_ptr<class DzX_Console> console)
{
	m_ActiveConsole = console;
	m_PickupType = PickupType;
	m_look = static_cast<char>(m_PickupType);
	bIsPickedUp = false;
}

void Pickup::OnPickup(Player* NewOwner)
{
	//Transfer stats to player inventory
	shared_ptr<Player> _player(NewOwner);
	m_Owner = _player;	


	shared_ptr<Pickup> LocalPtr(this);

	switch (m_PickupType)
	{
	case EObjectType::HPPOTION:
		m_Owner->Slot1= LocalPtr;
		break;
	case EObjectType::ARMOR:
		m_Owner->Slot2 = LocalPtr;
		break;
	case EObjectType::WEAPON:
		m_Owner->Slot3 = LocalPtr;
		break;
	case EObjectType::ADRENALIN:
		m_Owner->Slot4 = LocalPtr;
		break;
	}
	m_Owner->m_currentField = static_cast<char>(EObjectType::SPAWNABLE);
	bIsPickedUp = true;
	//Destroy shape
	cout << static_cast<char>(EObjectType::SPAWNABLE);
}

void Pickup::Effect()
{
	switch (m_PickupType)
	{
	case EObjectType::HPPOTION:
		m_Owner->UpdateHP(m_EffectIntensity);
		break;
	case EObjectType::ARMOR:
		m_Owner->UpdateDEF(m_EffectIntensity);
		break;
	case EObjectType::WEAPON:
		m_Owner->UpdateSTR(m_EffectIntensity);
		break;
	case EObjectType::ADRENALIN:
		int CurrentSTR=m_Owner->m_Stats.Strength;
		m_Owner->UpdateSTR(m_EffectIntensity);
		Sleep(500);
		m_Owner->m_Stats.Strength = CurrentSTR;
		break;
	}
	//delete this object
	delete this;
}

void Pickup::Spawn(COORD SpawnPos)
{
	m_SpawnCoord = SpawnPos;
}

void Pickup::Draw()
{
	if (m_ActiveConsole && !bIsPickedUp)
	{
		m_ActiveConsole->GoToXY(m_SpawnCoord.X, m_SpawnCoord.Y);
		std::wcout << m_look;
	}
}

void Pickup::SetEffect()
{
	switch (m_PickupType)
	{
	case EObjectType::HPPOTION:
		m_EffectIntensity = 5;
		break;
	case EObjectType::ARMOR:
		m_EffectIntensity = 1;
		break;
	case EObjectType::WEAPON:
		m_EffectIntensity = 2;
		break;
	case EObjectType::ADRENALIN:
		m_EffectIntensity = 10;
		break;
	}
}
