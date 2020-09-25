#include "Game.h"

#include "Characters/Characters.h"
#include "Characters/Enemy.h"
#include "Characters/Player.h"
#include "Pickups/Pickup.h"
#include "Menu/PlayerMenu.h"
#include "Menu/MainMenu.h"
#include <boost/range/adaptor/indexed.hpp>
#include <boost/assign.hpp>

GamePlay::GamePlay(shared_ptr<DzX_Console> console)
{
	//Pointer to the active console
	m_Console = console;

	//Setting playable area
	m_GamePlayArea.Left = 0;
	m_GamePlayArea.Top = 0;
	m_GamePlayArea.Right = 60;
	m_GamePlayArea.Bottom = 30;
}
void GamePlay::StartNewGame()
{
	PROFILING_FUNCTION();
	//Clear the screen
	system("cls");
	//Read world layout from XML
	m_WorldLook = ReadXML("Config/world.xml", "World", "Topology");
	//Draw world so we can take char from field where player will be spawned.
	DrawWorld();

	//Create player
	m_Player = make_shared<class Player>();
	int SpawnX = stoi(ReadXML("Config/GamePlay_Settings.xml", "GameSettings", "StartSpawnX"));
	int SpawnY = stoi(ReadXML("Config/GamePlay_Settings.xml", "GameSettings", "StartSpawnY"));
	m_Player->Spawn(this, m_Console, SpawnX, SpawnX);
	//Get  char from field where player will be spawned
	char ch = m_Console->GetCharAtPosition(SpawnX, SpawnY);
	//Set field on top of which is player
	m_Player->SetCurrentField(ch);

	//Spawn enemies on random points
	int NoOfEnemies = stoi(ReadXML("Config/GamePlay_Settings.xml", "GameSettings", "NoOfEnemies"));
	SpawnEnemies(NoOfEnemies);

	//Spawn HP pots on random points
	int NoOfPots = stoi(ReadXML("Config/GamePlay_Settings.xml", "GameSettings", "NoOfHPPots"));
	SpawnHPPots(NoOfPots);

	//Start game
	BeginPlay();
}
void GamePlay::BeginPlay()
{
	PROFILING_FUNCTION();
	//Create HUD if there is none
	if (!m_HUD)
	{
		m_HUD = make_shared<class HUD>(this);
	}

	//Set game to playing
	m_GameState = EGameState::GamePlaying;

	//Start Play loop
	Play();
}

void GamePlay::Play()
{
	PROFILING_FUNCTION();
	//Clear console
	system("cls");

	//Draw world
	DrawWorld();
	//Draw player HUD (side menu)
	m_HUD->DrawHud();



	//While playing loop
	while (m_GameState != EGameState::GameStopped)
	{
		PROFILING_SCOPE("PlayWhileLoop");
		//Catch input
		if (_kbhit())
		{
			//Get input char as ASCII (needed for F5)
			int ASCII_Key = _getch();
			//If game is playing
			if (m_GameState == EGameState::GamePlaying)
			{
				//Flag to block other keys execute move enemy
				bool flagInput = false;
				//Inputs - Move player, mark flag as true
				if (ASCII_Key == 97 || ASCII_Key == 65)/*A*/
				{
					m_Player->MoveTo(-1, 0);
					flagInput = true;
				}
				if (ASCII_Key == 100 || ASCII_Key == 68)/*D*/
				{
					m_Player->MoveTo(1, 0);
					flagInput = true;
				}
				if (ASCII_Key == 115 || ASCII_Key == 68)/*S*/
				{
					m_Player->MoveTo(0, 1);
					flagInput = true;
				}
				if (ASCII_Key == 119 || ASCII_Key == 87)/*W*/
				{
					m_Player->MoveTo(0, -1);
					flagInput = true;
				}
				if (ASCII_Key == 49)/*1*/
				{
					//Use HP potion
					m_Player->UseInventorySlot(0);
					flagInput = true;
				}
				if (ASCII_Key == 69 || ASCII_Key == 101) /*E*/
				{
					//Pickup dropped pickups
					m_Player->PickupEvent();
					flagInput = true;
				}
				if (ASCII_Key == 63/*F5*/)
				{
					//Save the game
					SaveGame();
				}
				if (flagInput)
				{
					//Push draw enemies to another thread
					std::thread t1(&GamePlay::MoveEnemies, this);
					t1.join();
				}
			}
			if (ASCII_Key == 120 || ASCII_Key == 88 /*X*/)
			{

				//Stop the game
				m_GameState = EGameState::GameStopped;
				//Exit to main menu
				m_Console->m_Menu->LoadMenu();
			}
		}

		//If game is in progress
		if (m_GameState == EGameState::GamePlaying)
		{
			//Update parts of the HUD with stats
			m_HUD->UpdateHud();

			//Push draw enemies to another thread
			std::thread t2(&GamePlay::DrawEnemies, this);
			t2.join();

			//Draw pickups (HP Pots, Armor, Weapon)
			DrawPickups();

			//Set color to green for the player
			m_Console->SetColor(10);
			//Draw player			
			m_Player->DrawCharacter();
			//Reset color to white after player is spawned
			m_Console->SetColor(7);
			//Check for overlaps around the player
			m_Player->OverlappEvent();
		}	
	}
}


void GamePlay::GameOver()
{
	PROFILING_FUNCTION();
	//Get graphic to draw on screen from XML
	std::string top = ReadXML("Config/HUD.xml", "GameOver", "Topology");
	vector<std::string> m_borkenStringUITop = ExplodeString(top, '\n');

	//Draw border on screen
	m_Console->Draw(2, 7, 8, 100, 15, 1, 1);

	//Add row for each NewLine
	int RowAdd = 0;
	//Loop each line
	for (auto s : m_borkenStringUITop)
	{
		//Offset it to match UI position
		m_Console->GoToXY(10, 10 + RowAdd);
		//Print
		std::cout << s;
		//Add for next row
		RowAdd++;
	}
	//Print information to exit
	m_Console->GoToXY(49, 20);
	cout << "Press X to Exit.";

}


void GamePlay::Victory()
{
	PROFILING_FUNCTION();
	//Get graphic to draw on screen from XML
	std::string top = ReadXML("Config/HUD.xml", "Victory", "Topology");
	vector<std::string> m_borkenStringUITop = ExplodeString(top, '\n');

	//Draw border on screen
	m_Console->Draw(2, 7, 8, 100, 15, 1, 1);

	//Add row for each NewLine
	int RowAdd = 0;
	//Loop each line
	for (auto s : m_borkenStringUITop)
	{
		//Offset it to match UI position
		m_Console->GoToXY(10, 10 + RowAdd);
		//Print
		std::cout << s;
		//Add for next row
		RowAdd++;
	}
	//Print information to exit
	m_Console->GoToXY(49, 20);
	cout << "Press X to Exit.";

}

void GamePlay::RemovePickup(class Pickup* PickupToRemove)
{
	PROFILING_FUNCTION();
	//Remove from container by value
	m_Pickups.erase(std::remove(m_Pickups.begin(), m_Pickups.end(), PickupToRemove), m_Pickups.end());
}

void GamePlay::RemoveEnemy(class Enemy* EnemyToRemove)
{
	PROFILING_FUNCTION();
	//Remove from container by value
	m_Enemies.erase(std::remove(m_Enemies.begin(), m_Enemies.end(), EnemyToRemove), m_Enemies.end());
}

void GamePlay::DrawWorld()
{
	PROFILING_FUNCTION();
	std::cout << m_WorldLook;
}

void GamePlay::SaveGame()
{
	PROFILING_FUNCTION();
	//Create XML doc
	pugi::xml_document doc;

#pragma region Pickups
	//Create Pickup node and store data
	pugi::xml_node PickupNode = doc.append_child("Pickups");
	//Pickups
	for (Pickup* pck : m_Pickups)
	{

		pugi::xml_node param = PickupNode.append_child("Pickup");
		param.append_attribute("Type") = static_cast<char>(pck->m_PickupType);
		param.append_attribute("PosX") = pck->m_SpawnCoord.X;
		param.append_attribute("PosY") = pck->m_SpawnCoord.Y;
	}
#pragma endregion Pickups
#pragma region Enemies
	//Create Enemy node and store data
	pugi::xml_node EnemyNode = doc.append_child("Enemies");
	for (Enemy* nme : m_Enemies)
	{
		pugi::xml_node param = EnemyNode.append_child("Enemy");
		param.append_attribute("Type") = static_cast<char>(nme->m_CharType);
		param.append_attribute("PosX") = nme->GetPosition().X;
		param.append_attribute("PosY") = nme->GetPosition().Y;
	}
#pragma endregion Enemies

#pragma region Player
	//Create Player node and store data
	pugi::xml_node PlayerNode = doc.append_child("Player");
	if (m_Player)
	{
		PlayerNode.append_attribute("Type") = static_cast<char>(m_Player->m_CharType);
		PlayerNode.append_attribute("PosX") = m_Player->GetPosition().X;
		PlayerNode.append_attribute("PosY") = m_Player->GetPosition().Y;
		PlayerNode.append_attribute("HP") = m_Player->m_Stats.CurrentHealth;
		PlayerNode.append_attribute("STR") = m_Player->m_Stats.Strength;
		PlayerNode.append_attribute("DEF") = m_Player->m_Stats.Defence;
		PlayerNode.append_attribute("INV1") = m_Player->Slot1.Amount;
	}
#pragma endregion Player
	
	//Save to file
	doc.save_file("Config/SaveFile.xml");

	//Stop the game
	m_GameState = EGameState::GameStopped;
	//Exit to main menu
	m_Console->m_Menu->LoadMenu();
}

void GamePlay::StartLoadGame()
{

	//Read XML with World layout
	m_WorldLook = ReadXML("Config/world.xml", "World", "Topology");
	//Draw world so we can take char from field where player will be spawned.
	DrawWorld();

	//Read XML with data to load
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file("Config/SaveFile.xml");

#pragma region Player
	//Read player data and spawn player
	pugi::xml_node PlayerNode = doc.child("Player");

	int PosX = stoi(PlayerNode.attribute("PosX").value());
	int PosY = stoi(PlayerNode.attribute("PosY").value());
	m_Player = make_shared<class Player>();
	m_Player->Spawn(this, m_Console, PosX, PosY);
	m_Player->m_Stats.CurrentHealth = stoi(PlayerNode.attribute("HP").value());
	m_Player->m_Stats.Strength = stoi(PlayerNode.attribute("STR").value());
	m_Player->m_Stats.Defence = stoi(PlayerNode.attribute("DEF").value());
	m_Player->Slot1.Amount = stoi(PlayerNode.attribute("INV1").value());
#pragma endregion Player

#pragma region Pickups
	//Read data for pickup data and spawn pickups
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
#pragma endregion Pickups

#pragma region Enemies
	//Read data for enemie data and spawn enemies
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
#pragma endregion Enemies

	//Start the game after data is loaded
	BeginPlay();
}



void GamePlay::DrawEnemies()
{
	if (m_Console)
	{
		//Set enemy color
		m_Console->SetColor(12);
		//Loop all enemies in container
		for (Enemy* NME : m_Enemies)
		{
			if (NME)
			{
				//Draw enemy
				NME->DrawCharacter();
			}
		}
		//Chenge color to default
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
			//Move enemy to random position
			NME.value()->MoveRandom(NME.index());
		}
	}
}

void GamePlay::DrawPickups()
{
	//#Optimization - no need to loop all the time
	//Loop all the object in Pickup container
	for (Pickup* PCK : m_Pickups)
	{
		if (PCK)
		{
			//Draw pickup
			PCK->Draw();
		}
	}
}

void GamePlay::SpawnHPPots(int NoOfPickups)
{
	//Spawn X amount of elements
	for (int i = 0; i < NoOfPickups; i++)
	{
		//create potion
		Pickup* tempPickup = new Pickup(EObjectType::HPPOTION, this);
		//Add to m_Pickups container
		m_Pickups.push_back(tempPickup);
		//Get random location to spawn
		COORD RandSpawnPoint = GetRandomPoint(i + 9);
		//Spawn pickup
		tempPickup->Spawn(RandSpawnPoint, m_Console);
	}
}

void GamePlay::SpawnEnemies(int NoOfEnemies)
{
	//Spawn X amount of elements
	for (int i = 0; i < NoOfEnemies; i++)
	{
		//Create enemy with RNG variation
		Enemy* TempNME = new Enemy(i % 2 == 0 ? EObjectType::ENEMY_1 : EObjectType::ENEMY_2);
		//Add enemy to m_Enemies container
		m_Enemies.push_back(TempNME);
		//Ger random point
		COORD RandSpawnPoint = GetRandomPoint(i + 1);
		//Spawn enemy
		TempNME->Spawn(this, m_Console, RandSpawnPoint.X, RandSpawnPoint.Y);

		//Get  char from field where player will be spawned
		char ch = m_Console->GetCharAtPosition(RandSpawnPoint.X, RandSpawnPoint.Y);
		//Set field on top of which is player
		TempNME->SetCurrentField(ch);
	}
}

COORD GamePlay::GetRandomPoint(int Seed)
{
	//Reset random seed by time and input seed
	srand((unsigned int)time(0) + (Seed * rand()));
	//Check for console
	if (m_Console)
	{
		//Get random points in playable area
		int X = m_GamePlayArea.Left + (std::rand() % (m_GamePlayArea.Right - m_GamePlayArea.Left));
		int Y = m_GamePlayArea.Top + (std::rand() % (m_GamePlayArea.Bottom - m_GamePlayArea.Top));
		//Check for what is on the field
		char c = m_Console->GetCharAtPosition(X, Y);
		//Check if it is in playable area
		bool bInRange = X < m_GamePlayArea.Right&& Y < m_GamePlayArea.Bottom;
		char field = static_cast<char>(EObjectType::SPAWNABLE);
		bool bEqual = c != field;
		//Recursion if point is not on playable area or out of bounds
		if (bEqual || !bInRange)
		{
			return GetRandomPoint(Seed + 1);
		}
		else
		{
			//Return point
			COORD RandPoint;
			RandPoint.X = X;
			RandPoint.Y = Y;
			return RandPoint;
		}
	}
	else
	{
		//If no console fail safe
		COORD rt;
		rt.X = 0;
		rt.Y = 0;
		return rt;
	}
}
