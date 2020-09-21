#include "Characters.h"
#include <io.h>
#include <fcntl.h>

Character::Character(shared_ptr<class DzX_Console>CurrentConsole, int spawnX, int spawnY)
{
	m_look = L"X";
	m_posX = spawnX;
	m_posY = spawnY;
	if (CurrentConsole)
	{
		m_ActiveConsole = CurrentConsole;		
	}
	else
	{
		cout << "There is no console for " << this << " !";
	}
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
		if (c == static_cast<char>(ESymbols::WALL))
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
	if (m_ActiveConsole)
	{
		m_ActiveConsole->GoToXY(m_posX, m_posY);
		std::wcout << m_look;

	}

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

//std::vector<int, int> Character::GetPosition()
//{
//	vector<int, int> RetVal(m_posX, m_posY);
//	return RetVal;
//}
