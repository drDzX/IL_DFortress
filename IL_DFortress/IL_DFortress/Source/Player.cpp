#include "Player.h"
#include "Pickup.h"
#include "Enemy.h"

Player::Player()
{
	m_look = static_cast<char>(EObjectType::PLAYER);
	
}

void Player::Spawn(class GamePlay* CurrentGame, shared_ptr<class DzX_Console>CurrentConsole, int spawnX, int spawnY)
{
	Character::Spawn(CurrentGame, CurrentConsole, spawnX, spawnY);

	m_Stats.Strength = stoi(ReadJson("Config/GamePlay_Settings.json", "PlayerSTR"));
	m_Stats.Defence = stoi(ReadJson("Config/GamePlay_Settings.json", "PlayerDEF"));
	m_Stats.MaxHealth= stoi(ReadJson("Config/GamePlay_Settings.json", "PlayerMaxHP"));
	m_Stats.CurrentHealth = m_Stats.MaxHealth;
	//Copy values to base stats
	m_BaseStats = m_Stats;
}

void Player::UseInventorySlot(int InventorySlot)
{

}

void Player::OverlappEvent()
{
	if (m_currentField != static_cast<char>(EObjectType::SPAWNABLE))
	{
		for (Enemy* NME : m_ActiveGame->m_Enemies)
		{		
			if (NME)
			{
				COORD p1 = NME->GetPosition();
				COORD p0 = GetPosition();
				if (p1.X == p0.X && p1.Y == p0.Y)
				{
					//Do smth
				}
			}

		}
		for (Pickup* PCK : m_ActiveGame->m_Pickups)
		{
			if (PCK && !PCK->bIsPickedUp)
			{
				COORD p1 = PCK->m_SpawnCoord;
				COORD p0 = GetPosition();
				if (p1.X == p0.X && p1.Y == p0.Y)
				{
					PCK->OnPickup(this);
				}
			}
	
		}
	}
}
