#include "Game.h"
#include <fstream>
#include "Characters.h"
GamePlay::GamePlay(shared_ptr<DzX_Console> console)
{
	m_Console = console;
}

void GamePlay::BeginPlay()
{
	//Draw world so we can take char from field where player will be spawned.
	DrawWorld();
	//Set game to playing
	m_GameState = EGameState::GamePlaying;
	//Create player character
	m_Player = make_shared<class Character>(m_Console,5,5);
	//Get  char from field where player will be spawned
	char ch= m_Console->GetCharAtPosition(5, 5);
	//Set field on top of which is player
	m_Player->SetCurrentField(ch);

	Play();

}

void GamePlay::Play()
{

	system("cls");
	DrawWorld();

	while (EGameState::GamePlaying)
	{
		if (_kbhit())
		{
			char ch = _getch();
			if (ch == 'a' || ch == 'A')
			{
				m_Player->MoveTo(-1, 0);
			}
			if (ch == 'd' || ch == 'D')
			{
				m_Player->MoveTo(1, 0);
			}
			if (ch == 's' || ch == 'S')
			{
				m_Player->MoveTo(0, 1);
			}
			if (ch == 'w' || ch == 'W')
			{
				m_Player->MoveTo(0, -1);
			}
			//m_Player->GetPosition();
		}

		m_Player->DrawCharacter();
	}	
}

void GamePlay::DrawWorld()
{
	std::cout << this->ReadJson("Config/world.json", "World");
}

std::string GamePlay::ReadJson(const char InPath[], const char elemKey[])
{
	std::ifstream i(InPath);

	i >> m_World_Data;

	std::string s = m_World_Data.at(elemKey);

	return s;
}
