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
void GamePlay::StartNewGame()
{
	m_WorldLook = ReadXML("Config/world.xml", "World", "Topology");
	//Draw world so we can take char from field where player will be spawned.
	DrawWorld();

	//Create player character
	m_Player = make_shared<class Player>();
	m_Player->Spawn(this, m_Console, 5, 5);
	//Get  char from field where player will be spawned
	char ch = m_Console->GetCharAtPosition(5, 5);
	//Set field on top of which is player
	m_Player->SetCurrentField(ch);
	int NoOfEnemies = stoi(ReadXML("Config/GamePlay_Settings.xml", "GameSettings", "NoOfEnemies"));
	SpawnEnemies(NoOfEnemies);
	SpawnPickup(3);

	BeginPlay();
}
void GamePlay::BeginPlay()
{

	//Set game to playing
	m_GameState = EGameState::GamePlaying;	


	//Create HUD
	m_HUD = make_shared<class HUD>(this);
	m_HUD->BeginPlay();
	
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
			//char ch = _getch();
			int ASCII_Key = _getch();
			if (ASCII_Key == 97 || ASCII_Key == 65)/*A*/
			{
				m_Player->MoveTo(-1, 0);
			}
			if (ASCII_Key == 100 || ASCII_Key == 68)/*D*/
			{
				m_Player->MoveTo(1, 0);
			}
			if (ASCII_Key == 115 || ASCII_Key == 68)/*S*/
			{
				m_Player->MoveTo(0, 1);
			}
			if (ASCII_Key == 119 || ASCII_Key == 87)/*W*/
			{
				m_Player->MoveTo(0, -1);
			}
			if (ASCII_Key == 49)/*1*/
			{
				m_Player->UseInventorySlot(0);
			}
			if (ASCII_Key == 69 || ASCII_Key == 101) /*E*/
			{
				m_Player->PickupEvent();
			}
			if (ASCII_Key == 63/*F5*/)
			{
				SaveGame();
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

void GamePlay::RemovePickup(class Pickup* PickupToRemove)
{
	m_Pickups.erase(std::remove(m_Pickups.begin(), m_Pickups.end(), PickupToRemove), m_Pickups.end());
}

void GamePlay::RemoveEnemy(class Enemy* EnemyToRemove)
{
	m_Enemies.erase(std::remove(m_Enemies.begin(), m_Enemies.end(), EnemyToRemove), m_Enemies.end());

}

void GamePlay::DrawWorld()
{
	std::cout << m_WorldLook;
}

void GamePlay::SaveGame()
{
	pugi::xml_document doc;
	//	pugi::xml_parse_result result = doc.load_file("Config/SaveFile.xml");

	pugi::xml_node PickupNode = doc.append_child("Pickups");
	//Pickups
	for (Pickup* pck : m_Pickups)
	{

		pugi::xml_node param = PickupNode.append_child("Pickup");
		param.append_attribute("Type") = static_cast<char>(pck->m_PickupType);
		param.append_attribute("PosX") = pck->m_SpawnCoord.X;
		param.append_attribute("PosY") = pck->m_SpawnCoord.Y;
	}
	//Enemies
	pugi::xml_node EnemyNode = doc.append_child("Enemies");
	for (Enemy* nme : m_Enemies)
	{
		pugi::xml_node param = EnemyNode.append_child("Enemy");
		param.append_attribute("Type") = static_cast<char>(nme->m_CharType);
		param.append_attribute("PosX") = nme->GetPosition().X;
		param.append_attribute("PosY") = nme->GetPosition().Y;
	}
	//Player
	pugi::xml_node PlayerNode = doc.append_child("Player");
	if (m_Player)
	{
		PlayerNode.append_attribute("Type") = static_cast<char>(m_Player->m_CharType) - '0';
		PlayerNode.append_attribute("PosX") = m_Player->GetPosition().X;
		PlayerNode.append_attribute("PosY") = m_Player->GetPosition().Y;
		PlayerNode.append_attribute("HP") = m_Player->m_Stats.CurrentHealth;
		PlayerNode.append_attribute("STR") = m_Player->m_Stats.Strength;
		PlayerNode.append_attribute("DEF") = m_Player->m_Stats.Defence;
		PlayerNode.append_attribute("INV1") = m_Player->Slot1.Amount;
	}
	doc.save_file("Config/SaveFile.xml");
}

void GamePlay::StartLoadGame()
{
	m_WorldLook = ReadXML("Config/world.xml", "World", "Topology");
	//Draw world so we can take char from field where player will be spawned.
	DrawWorld();


	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file("Config/SaveFile.xml");

	//Player
	pugi::xml_node PlayerNode = doc.child("Player");


	int PosX =stoi( PlayerNode.attribute("PosX").value() );
	int PosY = stoi(PlayerNode.attribute("PosY").value());
	m_Player = make_shared<class Player>();
	m_Player->Spawn(this, m_Console, PosX, PosY);
	m_Player->m_Stats.CurrentHealth= stoi(PlayerNode.attribute("HP").value());
	m_Player->m_Stats.Strength = stoi(PlayerNode.attribute("STR").value());
	m_Player->m_Stats.Defence = stoi(PlayerNode.attribute("DEF").value());
	m_Player->Slot1.Amount = stoi(PlayerNode.attribute("INV1").value());

	//Pickups
	for (pugi::xml_node pck : doc.child("Pickups").children("Pickup"))
	{
		int PosX = stoi(pck.attribute("PosX").value());
		int PosY = stoi(pck.attribute("PosY").value());
		COORD Pos;
		Pos.X = PosX;
		Pos.Y = PosY;
		char c = stoi(pck.attribute("Type").value());
		EObjectType pckType = (EObjectType)c;
		Pickup* tempPickup = new Pickup(pckType, this);
		m_Pickups.push_back(tempPickup);
		tempPickup->Spawn(Pos, m_Console);

	}
	//Enemies

	for (pugi::xml_node nme : doc.child("Enemies").children("Enemy"))
	{
		char c = stoi(nme.attribute("Type").value());
		EObjectType nmeType = (EObjectType)c;
		Enemy* TempNME = new Enemy(nmeType);
		m_Enemies.push_back(TempNME);

		int PosX = stoi(nme.attribute("PosX").value());
		int PosY = stoi(nme.attribute("PosY").value());

		TempNME->Spawn(this, m_Console, PosX, PosY);
		//Get  char from field where player will be spawned
		char ch = m_Console->GetCharAtPosition(PosX, PosY);
		
		//Set field on top of which is player
		TempNME->SetCurrentField(ch);

	}


	BeginPlay();
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
	//Taking index out of loop to make variations in random seed
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
		Pickup* tempPickup = new Pickup(EObjectType::HPPOTION, this);
		m_Pickups.push_back(tempPickup);

		COORD RandSpawnPoint = GetRandomPoint(i + 9);
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
	srand((unsigned int)time(0) + (Seed * rand()));
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
