#include "Game.h"
#include <fstream>
#include <time.h>
#include <stdio.h>
#include "Characters.h"
#include "Enemy.h"
#include "Player.h"
#include <boost/range/adaptor/indexed.hpp>
#include <boost/assign.hpp>

GamePlay::GamePlay(shared_ptr<DzX_Console> console)
{
	m_Console = console;

	m_GamePlayArea.Left = 0;
	m_GamePlayArea.Top = 0;
	m_GamePlayArea.Right = 60;
	m_GamePlayArea.Bottom = 30;
}

void GamePlay::BeginPlay()
{
	//Draw world so we can take char from field where player will be spawned.
	DrawWorld();
	//Set game to playing
	m_GameState = EGameState::GamePlaying;
	//Create player character
	m_Player = make_shared<class Player>();
	m_Player->Spawn(this, m_Console, 5, 5);
	//Get  char from field where player will be spawned
	char ch= m_Console->GetCharAtPosition(5, 5);
	//Set field on top of which is player
	m_Player->SetCurrentField(ch);

	int NoOfEnemies = stoi(ReadJson("Config/GamePlay_Settings.json", "NoOfEnemies"));
	SpawnEnemies(NoOfEnemies);
	Play();

}

void GamePlay::Play()
{

	system("cls");
	DrawWorld();

	while (EGameState::GamePlaying)
	{
		//Catch input
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
			//Push draw enemies to another thread
			std::thread t1(&GamePlay::MoveEnemies, this);
			t1.join();
			//m_Player->GetPosition();
		}
		//Push draw enemies to another thread
		std::thread t2(&GamePlay::DrawEnemies,this);
		t2.join();

		//Draw Character
		m_Player->DrawCharacter();
	}	
}

void GamePlay::GameOver()
{

}

void GamePlay::DrawWorld()
{
	std::string world = ReadJson("Config/world.json", "World");
	std::cout << world;
}

void GamePlay::DrawEnemies()
{
	for (Enemy* NME : m_Enemies)
	{
		if (NME)
		{
			NME->DrawCharacter();
		}		
	}
}

void GamePlay::MoveEnemies()
{
	for (const auto& NME : m_Enemies | boost::adaptors::indexed(0))
	{
		if (NME.value())
		{
			NME.value()->MoveRandom(NME.index());
		}
	}
}

void GamePlay::SpawnEnemies(int NoOfEnemies)
{
	if (NoOfEnemies <= (int)m_Enemies.size())
	{
		for (int i = 0; i < NoOfEnemies; i++)
		{
			m_Enemies[i] = new Enemy();

			COORD RandSpawnPoint = GetRandomPoint(i+1);
			m_Enemies[i]->Spawn(this, m_Console, RandSpawnPoint.X, RandSpawnPoint.Y);


			//Get  char from field where player will be spawned
			char ch = m_Console->GetCharAtPosition(RandSpawnPoint.X, RandSpawnPoint.Y);
			//Set field on top of which is player
			m_Enemies[i]->SetCurrentField(ch);
		}
	}
}

COORD GamePlay::GetRandomPoint(int Seed)
{
	srand((unsigned int)time(0)+(Seed*2));
	int X = m_GamePlayArea.Left + (std::rand() % (m_GamePlayArea.Right - m_GamePlayArea.Left + 1));
	int Y = m_GamePlayArea.Top + (std::rand() % (m_GamePlayArea.Bottom - m_GamePlayArea.Top + 1));
	char c = m_Console->GetCharAtPosition(X, Y);
	if (c != static_cast<char>(ESymbols::SPAWNABLE))
	{
		return GetRandomPoint();
	}
	else
	{
		COORD RandPoint;
		RandPoint.X = X;
		RandPoint.Y = Y;
		return RandPoint;
	}
}
