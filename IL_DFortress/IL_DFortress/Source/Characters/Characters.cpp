#include "Characters.h"
#include "Game.h"

Character::Character()
{
	//Default look
	m_look = L"X";	
	//Default character state
	m_PlayerState = EPlayerState::ALIVE;	
}

void Character::EraseCharacter()
{
	PROFILING_FUNCTION();
	if (m_ActiveConsole)
	{
		//Move cursor to current location
		m_ActiveConsole->GoToXY(m_posX, m_posY);
		//Draw spawnable field
		cout << static_cast<char>(EObjectType::SPAWNABLE);
	}
}

bool Character::bCanMoveToNext(int X, int Y)
{
	PROFILING_FUNCTION();
	if (m_ActiveConsole)
	{
		//Get field at next position
		char c = m_ActiveConsole->GetCharAtPosition(X, Y);
		//Check if it is obstacle 
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
		//Move curosr to position
		m_ActiveConsole->GoToXY(m_posX, m_posY);
		//Draw look
		std::wcout << m_look;
	}	
	
}

COORD Character::GetPosition()
{
	PROFILING_FUNCTION();
	COORD _C;
	_C.X = m_posX;
	_C.Y = m_posY;
	return _C;
}

void Character::MoveTo(int addX, int addY)
{
	PROFILING_FUNCTION();
	//Add to X and Y
	int nextX = m_posX + addX;
	int nextY = m_posY + addY;
	//Check if next field is available to move
	if (bCanMoveToNext(nextX, nextY))
	{
		//Erase character from current position
		EraseCharacter();
		//Fail safe to not go out of bounds
		if (m_posX - addX < 0)
		{
			m_posX = 0;
		}
		if (m_posY - addY < 0)
		{
			m_posY = 0;
		}
		//Add to position #Optimization-can be swaped with nextX
		m_posX += addX;
		m_posY += addY;
		//Get field mark where character will lend on and store data
		m_currentField = m_ActiveConsole->GetCharAtPosition(m_posX, m_posY);

	}
}

void Character::UpdateHP(int InChange)
{
	//Change current health by adding new change
	m_Stats.CurrentHealth += InChange;
	//Check for overflow
	if (m_Stats.CurrentHealth > m_Stats.MaxHealth)
	{
		m_Stats.CurrentHealth = m_Stats.MaxHealth;
	}
	//Die if under or zero
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
	PROFILING_FUNCTION();
	//Set spawn position
	m_posX = spawnX;
	m_posY = spawnY;
	//Set active game pointer
	m_ActiveGame = CurrentGame;
	if (CurrentConsole)
	{
		//set console pointer
		m_ActiveConsole = CurrentConsole;
	}
}

void Character::Attack(class Character* Enemy)
{
	PROFILING_FUNCTION();
	//Calculate dmg acording to STR and Enemy DEF
	int Damage = m_Stats.Strength - Enemy->m_Stats.Defence;
	//Inflict damage to enemy
	Enemy->UpdateHP((-1)*Damage);
}

void Character::Die()
{
	m_PlayerState = EPlayerState::DEAD;
}
