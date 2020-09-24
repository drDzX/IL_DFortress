#include "Pickup.h"
#include "Player.h"


Pickup::Pickup(EObjectType PickupType)
{
	m_PickupType = PickupType;
	m_look = static_cast<char>(m_PickupType);
	bIsPickedUp = false;
	SetEffect();
}

void Pickup::OnPickup(Player* NewOwner)
{
	switch (m_PickupType)
	{
	case EObjectType::HPPOTION:
		NewOwner->Slot1.Amount++;
		break;
	case EObjectType::ARMOR:
		Effect(NewOwner);
		break;
	case EObjectType::WEAPON:
		Effect(NewOwner);
		break;

	}
	NewOwner->m_currentField = static_cast<char>(EObjectType::SPAWNABLE);
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
}

void Pickup::Effect(Player* EffectOnPlayer)
{

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
	m_ActiveConsole = console;
	m_SpawnCoord = SpawnPos;
}

void Pickup::Draw()
{
	if (m_ActiveConsole && !bIsPickedUp)
	{
		m_ActiveConsole->SetColor(14);
		m_ActiveConsole->GoToXY(m_SpawnCoord.X, m_SpawnCoord.Y);

		std::wcout << m_look;
		if (m_PickupType == EObjectType::ARMOR || m_PickupType == EObjectType::WEAPON)
		{
			std::wcout << "+"<<m_EffectIntensity;
		}
		m_ActiveConsole->SetColor(7);
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

	}
}
