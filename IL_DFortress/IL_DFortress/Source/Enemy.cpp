#include "Enemy.h"
#include "Pickup.h"
#include "Game.h"
Enemy::Enemy(EObjectType EnemyType)
{

	m_look = static_cast<char>(EnemyType);
	m_CharType = EnemyType;
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
	if (m_ActiveConsole)
	{

		unsigned int RandSeed = (unsigned int)time(0) + (Seed * 2);
		srand(RandSeed);
		int X;
		int Y;
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
		
		if (m_posX + X < m_ActiveGame->m_GamePlayArea.Right, m_posY + Y < m_ActiveGame->m_GamePlayArea.Bottom)
		{
			char c = m_ActiveConsole->GetCharAtPosition(m_posX + X, m_posY + Y);
			if (c != static_cast<char>(EObjectType::SPAWNABLE))
			{
				//Recursion - if next point is unavailable to move.
				MoveRandom(Seed + 1);
			}
			else
			{
				MoveTo(X, Y);
			}
		}

	}
	OverlappEvent();
}

void Enemy::DropItem()
{
	//Create random seed by current time
	unsigned int RandSeed = (unsigned int)time(0);
	srand(RandSeed);
	//Random between to get Armor or Weapon to drop;
	EObjectType DropType;
	rand() % 2 == 0 ? DropType = EObjectType::ARMOR : DropType = EObjectType::WEAPON;
	
	Pickup* Drop = new Pickup(DropType,m_ActiveGame);
	m_ActiveGame->m_Pickups.push_back(Drop);
	Drop->Spawn(this->GetPosition(), m_ActiveConsole);
}

void Enemy::OverlappEvent()
{
	if (m_currentField == static_cast<char>(EObjectType::PLAYER))
	{
		Attack((Character*)m_ActiveGame->m_Player.get());
	}
}

void Enemy::Die()
{
	Character::Die();
	if (m_ActiveGame)
	{
		m_ActiveGame->RemoveEnemy(this);
		if (m_ActiveGame->m_Enemies.size() == 0)
		{
			m_ActiveGame->m_GameState = EGameState::GamePaused;
			m_ActiveGame->Victory();
		}
	}
	if (m_ActiveGame->m_GameState == EGameState::GamePlaying)
	{
	
		DropItem();
	}
}

