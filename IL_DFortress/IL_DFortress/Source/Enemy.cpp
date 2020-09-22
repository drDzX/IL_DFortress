#include "Enemy.h"

Enemy::Enemy()
{

	m_look = static_cast<char>(ESymbols::ENEMY_1);
	m_Stats.Strength = 3;
	m_Stats.Defence = 1;
}

void Enemy::MoveRandom(int Seed)
{
	if (m_ActiveConsole)
	{

		unsigned int RandSeed =  (unsigned int)time(0) + (Seed * 2);
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

		char c = m_ActiveConsole->GetCharAtPosition(m_posX + X, m_posY + Y);
		if (c != static_cast<char>(ESymbols::SPAWNABLE))
		{
			//Recursion - if next point is unavailable to move.
			MoveRandom(Seed+1);
		}
		else
		{
			MoveTo(X, Y);
		}

	}
}

