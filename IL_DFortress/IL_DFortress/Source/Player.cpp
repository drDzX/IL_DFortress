#include "Player.h"

Player::Player()
{
	m_look = static_cast<char>(ESymbols::PLAYER);
	m_Stats.Strength = 5;
	m_Stats.Defence = 1;
}
