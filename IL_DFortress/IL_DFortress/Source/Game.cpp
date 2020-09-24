#include "Game.h"
#include <fstream>
#include <time.h>
#include <stdio.h>
#include "Characters.h"
#include "Enemy.h"
#include "Player.h"
#include "Pickup.h"
#include "PlayerMenu.h"
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
	m_WorldLook = ReadXML("Config/world.xml", "World","Topology");
	//Draw world so we can take char from field where player will be spawned.
	DrawWorld();

	//Set game to playing
	m_GameState = EGameState::GamePlaying;
	//Create player character
	m_Player = make_shared<class Player>();
	m_Player->Spawn(this, m_Console, 5, 5);
	//Get  char from field where player will be spawned
	char ch = m_Console->GetCharAtPosition(5, 5);
	//Set field on top of which is player
	m_Player->SetCurrentField(ch);

	//Create HUD
	m_HUD = make_shared<class HUD>(this);
	m_HUD->BeginPlay();
	int NoOfEnemies = stoi(ReadXML("Config/GamePlay_Settings.xml","GameSettings", "NoOfEnemies"));
	SpawnEnemies(NoOfEnemies);
	SpawnPickup(3);
	Play();
}

void GamePlay::Play()
{
	system("cls");
	DrawWorld();
	m_HUD->DrawHud();

	while (EGameState::GamePlaying)
	{
		m_HUD->UpdateHud();
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
			if (ch == '1')
			{
				m_Player->UseInventorySlot(0);
			}
			if (ch == 'E' || ch == 'e')
			{
				m_Player->PickupEvent();
			}
			//Push draw enemies to another thread
			std::thread t1(&GamePlay::MoveEnemies, this);
			t1.join();
			//m_Player->GetPosition();
		}
		//Push draw enemies to another thread
		std::thread t2(&GamePlay::DrawEnemies, this);
		t2.join();

		DrawPickups();
		//Draw Character
		m_Console->SetColor(10);
		m_Player->DrawCharacter();
		m_Console->SetColor(7);
		m_Player->OverlappEvent();

	}

}

void GamePlay::GameOver()
{

}

void GamePlay::DrawWorld()
{
	std::cout << m_WorldLook;
}

void GamePlay::DrawEnemies()
{
	if (m_Console)
	{
		m_Console->SetColor(12);
		for (Enemy* NME : m_Enemies)
		{
			if (NME)
			{
				NME->DrawCharacter();
				NME->OverlappEvent();
			}
		}
		m_Console->SetColor(7);
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

void GamePlay::DrawPickups()
{
	for (Pickup* PCK : m_Pickups)
	{
		if (PCK)
		{
			PCK->Draw();
		}
	}
}

void GamePlay::SpawnPickup(int NoOfPickups)
{

	for (int i = 0; i < NoOfPickups; i++)
	{
		Pickup* tempPickup = new Pickup(EObjectType::HPPOTION);
		m_Pickups.push_back(tempPickup);

		COORD RandSpawnPoint = GetRandomPoint(i + 10);
		tempPickup->Spawn(RandSpawnPoint, m_Console);
	}

}

void GamePlay::SpawnEnemies(int NoOfEnemies)
{

	for (int i = 0; i < NoOfEnemies; i++)
	{
		Enemy* TempNME = new Enemy(i % 2 == 0 ? EObjectType::ENEMY_1 : EObjectType::ENEMY_2);
		m_Enemies.push_back(TempNME);

		COORD RandSpawnPoint = GetRandomPoint(i + 1);
		TempNME->Spawn(this, m_Console, RandSpawnPoint.X, RandSpawnPoint.Y);


		//Get  char from field where player will be spawned
		char ch = m_Console->GetCharAtPosition(RandSpawnPoint.X, RandSpawnPoint.Y);
		//Set field on top of which is player
		TempNME->SetCurrentField(ch);

		//delete TempNME;
	}
}

COORD GamePlay::GetRandomPoint(int Seed)
{
	//TODO - Problem with overlap
	srand((unsigned int)time(0) + (Seed * 2));
	if (m_Console)
	{
		int X = m_GamePlayArea.Left + (std::rand() % (m_GamePlayArea.Right - m_GamePlayArea.Left + 1));
		int Y = m_GamePlayArea.Top + (std::rand() % (m_GamePlayArea.Bottom - m_GamePlayArea.Top + 1));
		char c = m_Console->GetCharAtPosition(X, Y);
		if (c != static_cast<char>(EObjectType::SPAWNABLE))
		{
			return GetRandomPoint(Seed + 1);
		}
		else
		{
			COORD RandPoint;
			RandPoint.X = X;
			RandPoint.Y = Y;
			return RandPoint;
		}
	}
}
