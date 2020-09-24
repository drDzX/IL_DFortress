#include "Pickup.h"
#include "Player.h"
#include "Game.h"

Pickup::Pickup(EObjectType PickupType, class GamePlay* Game)
{
	m_PickupType = PickupType;
	m_look = static_cast<char>(m_PickupType);
	bIsPickedUp = false;
	SetEffect();

	if (Game)
	{
		shared_ptr<class GamePlay> sp(Game);
		m_GamePlay = sp;
	}

}

void Pickup::OnPickup(Player* NewOwner)
{
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
	if (m_GamePlay)
	{
		m_GamePlay->RemovePickup(this);
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
