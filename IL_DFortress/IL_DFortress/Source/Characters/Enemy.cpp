#include "Enemy.h"
#include "Pickups/Pickup.h"
#include "Game.h"
Enemy::Enemy(EObjectType EnemyType)
{
	//Set look
	m_look = static_cast<char>(EnemyType);
	//Set type of character
	m_CharType = EnemyType;
	//Get stats acording to the type
	switch (EnemyType)
	{
	case EObjectType::ENEMY_1:		
		m_Stats.Strength = stoi(ReadXML("Config/GamePlay_Settings.xml", "Enemy1","STR"));
		m_Stats.Defence = stoi(ReadXML("Config/GamePlay_Settings.xml", "Enemy1", "DEF"));
		m_Stats.MaxHealth = stoi(ReadXML("Config/GamePlay_Settings.xml", "Enemy1", "HP"));
		m_Stats.CurrentHealth = m_Stats.MaxHealth;
		break;
	case EObjectType::ENEMY_2:
		m_Stats.Strength = stoi(ReadXML("Config/GamePlay_Settings.xml", "Enemy2", "STR"));
		m_Stats.Defence = stoi(ReadXML("Config/GamePlay_Settings.xml", "Enemy2", "DEF"));
		m_Stats.MaxHealth = stoi(ReadXML("Config/GamePlay_Settings.xml", "Enemy2", "HP"));
		m_Stats.CurrentHealth = m_Stats.MaxHealth;
		break;
	}
}

void Enemy::MoveRandom(int Seed)
{
	PROFILING_FUNCTION();
	if (m_ActiveConsole)
	{
		//Reset random seed
		unsigned int RandSeed = (unsigned int)time(0) + (Seed * 2);
		srand(RandSeed);

		int X;
		int Y;

		//get random position index in 4 directions
		int Index = std::rand() % 4;
		switch (Index)
		{
		case 0:
			X = 0;
			Y = 1;
			break;
		case 1:
			X = 0;
			Y = -1;
			break;
		case 2:
			X = 1;
			Y = 0;
			break;
		case 3:
			X = -1;
			Y = 0;
			break;
		default:
			X = 0;
			Y = 0;
		}
		//Check if in bounds
		if (m_posX + X < m_ActiveGame->m_GamePlayArea.Right, m_posY + Y < m_ActiveGame->m_GamePlayArea.Bottom)
		{
			//Check if can move to next field
			char c = m_ActiveConsole->GetCharAtPosition(m_posX + X, m_posY + Y);
			//Recursion to get another point if next field is unavailable
			if (c != static_cast<char>(EObjectType::SPAWNABLE))
			{
				//Recursion - if next point is unavailable to move.
				MoveRandom(Seed + 1);
			}
			else
			{
				//Move to next point
				MoveTo(X, Y);
			}
		}

	}
}

void Enemy::DropItem()
{
	PROFILING_FUNCTION();
	//Create random seed by current time
	unsigned int RandSeed = (unsigned int)time(0);
	srand(RandSeed);
	
	//Random between to get Armor or Weapon to drop;
	EObjectType DropType;
	rand() % 2 == 0 ? DropType = EObjectType::ARMOR : DropType = EObjectType::WEAPON;
	
	//Create drop object out of Pickup class
	Pickup* Drop = new Pickup(DropType,m_ActiveGame);
	//Add to games m_Pickup container
	m_ActiveGame->m_Pickups.push_back(Drop);
	//Spawn object
	Drop->Spawn(this->GetPosition(), m_ActiveConsole);
}



void Enemy::Die()
{
	PROFILING_FUNCTION();
	//Call parent function
	Character::Die();
	if (m_ActiveGame)
	{
		//Remove enemy from game container m_Enemies
		m_ActiveGame->RemoveEnemy(this);
		//If there are no enemies win game
		if (m_ActiveGame->m_Enemies.size() == 0)
		{
			m_ActiveGame->m_GameState = EGameState::GamePaused;
			m_ActiveGame->Victory();
		}
	}
	//If enemy died and game is still on drop item
	if (m_ActiveGame->m_GameState == EGameState::GamePlaying)
	{		
		DropItem();
	}
}

