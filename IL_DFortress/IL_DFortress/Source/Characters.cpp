#include "Characters.h"
#include "Game.h"
#include <io.h>
#include <fcntl.h>
#include<type_traits>

Character::Character()
{
	m_look = L"X";	
	m_PlayerState = EPlayerState::ALIVE;	
}

Character::~Character()
{
	delete m_ActiveGame;
}

void Character::EraseCharacter()
{
	if (m_ActiveConsole)
	{
		m_ActiveConsole->GoToXY(m_posX, m_posY);
		cout << m_currentField;
	}
}

bool Character::bCanMoveToNext(int X, int Y)
{
	if (m_ActiveConsole)
	{
		char c = m_ActiveConsole->GetCharAtPosition(X, Y);
		if (c == static_cast<char>(EObjectType::WALL))
		{
			return false;
		}
	}

	return true;
}

void Character::SetCurrentField(const char CurrentField)
{
	m_currentField = CurrentField;
}

void Character::DrawCharacter()
{
	if (m_ActiveConsole && m_PlayerState!=EPlayerState::DEAD)
	{
		m_ActiveConsole->GoToXY(m_posX, m_posY);
		std::wcout << m_look;
	}	
	
}

COORD Character::GetPosition()
{
	COORD _C;
	_C.X = m_posX;
	_C.Y = m_posY;
	return _C;
}

void Character::MoveTo(int X, int Y)
{
	int nextX = m_posX + X;
	int nextY = m_posY + Y;
	if (bCanMoveToNext(nextX, nextY))
	{
	
		EraseCharacter();
		if (m_posX - X < 0)
		{
			m_posX = 0;
		}
		if (m_posY - Y < 0)
		{
			m_posY = 0;
		}
		m_posX += X;
		m_posY += Y;
		m_currentField = m_ActiveConsole->GetCharAtPosition(m_posX, m_posY);

	}
}

void Character::UpdateHP(int InChange)
{
	
	m_Stats.CurrentHealth += InChange;
	if (m_Stats.CurrentHealth > m_Stats.MaxHealth)
	{
		m_Stats.CurrentHealth = m_Stats.MaxHealth;
	}
	if (m_Stats.CurrentHealth <= 0)
	{
		Die();
	}
}

void Character::UpdateSTR(int InChange)
{
	m_Stats.Strength = m_BaseStats.Strength+InChange;
}

void Character::UpdateDEF(int InChange)
{
	m_Stats.Defence = m_BaseStats.Defence + InChange;
}


void Character::Spawn(class GamePlay* CurrentGame, shared_ptr<class DzX_Console>CurrentConsole, int spawnX, int spawnY)
{
	m_posX = spawnX;
	m_posY = spawnY;
	m_ActiveGame = CurrentGame;
	if (CurrentConsole)
	{
		m_ActiveConsole = CurrentConsole;
	}
	else
	{
		cout << "There is no console for " << this << " !";
	}
}

void Character::Attack(class Character* Enemy)
{
	int Damage = m_Stats.Strength - Enemy->m_Stats.Defence;
	Enemy->UpdateHP(-Damage);
}

void Character::Die()
{
	m_PlayerState = EPlayerState::DEAD;
	if (m_ActiveGame)
	{
		m_ActiveGame->GameOver();
	}
}

//std::vector<int, int> Character::GetPosition()
//{
//	vector<int, int> RetVal(m_posX, m_posY);
//	return RetVal;
//}
