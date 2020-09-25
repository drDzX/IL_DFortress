#include "Characters/Player.h"
#include "Pickups/Pickup.h"
#include "Enemy.h"
#include "Game.h"
Player::Player()
{
	//Set character type
	m_CharType = EObjectType::PLAYER;
	//Set look
	m_look = static_cast<char>(EObjectType::PLAYER);
	//Set inventory slots
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
	//Switch to called slot and execute its effect 
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
	//Check if there are enemies around player and return their position 
	COORD EnemyPos;
	if (CheckForEnemiesAround(EnemyPos))
	{
		//#Optimization - Can be changed to ask for value, but its O(n) also
		//Loop m_Enemie on game mode and search for this enemy
		for (Enemy* NME : m_ActiveGame->m_Enemies)
		{
			if (NME)
			{
				//Get enemy position and compare to scanned location
				COORD p1 = NME->GetPosition();
				COORD p0 = EnemyPos;
				if (p1.X == p0.X && p1.Y == p0.Y)
				{
					//Fight enemy
					Fight(NME);
				}
			}
		}
	}
}

void Player::PickupEvent()
{
	//If player is on top of the pickup
	if (m_currentField != static_cast<char>(EObjectType::SPAWNABLE))
	{
		//Get player location
		COORD p0 = GetPosition();
		//Search for that pickup in m_Pickups list
		for (Pickup* PCK : m_ActiveGame->m_Pickups)
		{
			if (PCK && !PCK->bIsPickedUp)
			{
				//Compare if char is on top of this object
				COORD p1 = PCK->m_SpawnCoord;
				if (p1.X == p0.X && p1.Y == p0.Y)
				{
					//Collect dropped item
					PCK->OnPickup(this);
				}
			}
		}
	}
}

bool Player::CheckForEnemiesAround(COORD& EnemyCoord)
{
	//Get player position
	COORD p0 = GetPosition();
	//Sweep 9x9 grid around player O(n^2)
	for (int x = -1; x <= 1; x++)
	{
		for (int y = -1; y <= 1; y++)
		{
			//Exclude mid point
			if (!(x == 0 && y == 0))
			{
				//Check if grid segment is enemy
				char _c = m_ActiveConsole->GetCharAtPosition(p0.X + x, p0.Y + y);
				if (_c == static_cast<char>(EObjectType::ENEMY_1) || _c == static_cast<char>(EObjectType::ENEMY_2))
				{
					//Change referenced coordinates
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
	//Enemy attack Player
	Opponent->Attack(this);
	//Player attack Enemy
	Attack(Opponent);
	
	//If enemy is still alive repeat until someone die - recursion
	if (Opponent->m_PlayerState == EPlayerState::ALIVE)
	{
		Fight(Opponent);
	}
}

void Player::Die()
{
	//Call parent Die
	Character::Die();
	//If game is still active
	if (m_ActiveGame->m_GameState == GamePlaying)
	{
		//Pause game
		m_ActiveGame->m_GameState = EGameState::GamePaused;
		//Trigger Game over sequance
		m_ActiveGame->GameOver();
	}
}
