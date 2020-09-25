#include "Player.h"
#include "Pickup.h"
#include "Enemy.h"
#include "Game.h"
Player::Player()
{
	m_CharType = EObjectType::PLAYER;
	m_look = static_cast<char>(EObjectType::PLAYER);
	Slot1.Setup(this, EObjectType::HPPOTION);
	Slot2.Setup(this, EObjectType::ARMOR);
	Slot3.Setup(this, EObjectType::WEAPON);
}

void Player::Spawn(class GamePlay* CurrentGame, shared_ptr<class DzX_Console>CurrentConsole, int spawnX, int spawnY)
{
	Character::Spawn(CurrentGame, CurrentConsole, spawnX, spawnY);

	m_Stats.Strength = stoi(ReadXML("Config/GamePlay_Settings.xml", "Player", "STR"));
	m_Stats.Defence = stoi(ReadXML("Config/GamePlay_Settings.xml", "Player", "DEF"));
	m_Stats.MaxHealth = stoi(ReadXML("Config/GamePlay_Settings.xml", "Player", "HP"));
	m_Stats.CurrentHealth = m_Stats.MaxHealth;
	//Copy values to base stats
	m_BaseStats = m_Stats;

}

void Player::UseInventorySlot(int InventorySlot)
{
	switch (InventorySlot)
	{
	case 0:
		Slot1.ExecuteEffect();
		break;
	case 1:
		Slot2.ExecuteEffect();
		break;
	case 2:
		Slot3.ExecuteEffect();
		break;
	default:
		break;
	}
}

void Player::OverlappEvent()
{
	COORD EnemyPos;
	if (CheckForEnemiesAround(EnemyPos))
	{
		for (Enemy* NME : m_ActiveGame->m_Enemies)
		{
			if (NME)
			{
				COORD p1 = NME->GetPosition();
				COORD p0 = EnemyPos;
				if (p1.X == p0.X && p1.Y == p0.Y)
				{
					//NME->DropItem();
					Fight(NME);
					if (NME->m_PlayerState == EPlayerState::DEAD)
					{
						//	delete NME;
					}
				}
			}

		}
	}
}

void Player::PickupEvent()
{
	if (m_currentField != static_cast<char>(EObjectType::SPAWNABLE))
	{
		for (Pickup* PCK : m_ActiveGame->m_Pickups)
		{
			if (PCK && !PCK->bIsPickedUp)
			{
				COORD p1 = PCK->m_SpawnCoord;
				COORD p0 = GetPosition();
				if (p1.X == p0.X && p1.Y == p0.Y)
				{
					PCK->OnPickup(this);
					//delete PCK;
				}
			}

		}
	}
}

bool Player::CheckForEnemiesAround(COORD& EnemyCoord)
{
	COORD p0 = GetPosition();
	for (int x = -1; x <= 1; x++)
	{
		for (int y = -1; y <= 1; y++)
		{
			if (!(x == 0 && y == 0))
			{
				char _c = m_ActiveConsole->GetCharAtPosition(p0.X + x, p0.Y + y);
				if (_c == static_cast<char>(EObjectType::ENEMY_1) || _c == static_cast<char>(EObjectType::ENEMY_2))
				{
					COORD NmeCoord;
					NmeCoord.X = p0.X + x;
					NmeCoord.Y = p0.Y + y;
					EnemyCoord = NmeCoord;
					return true;
				}
			}
		}
	}
	return false;
}

void Player::Fight(Enemy* Opponent)
{
	Opponent->Attack(this);
	Attack(Opponent);
	if (Opponent->m_PlayerState == EPlayerState::ALIVE)
	{
		Fight(Opponent);
	}
}

void Player::Die()
{
	Character::Die();
	if (m_ActiveGame->m_GameState == GamePlaying)
	{
		m_ActiveGame->m_GameState = EGameState::GamePaused;
		m_ActiveGame->GameOver();
	}
}
