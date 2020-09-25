#pragma once
#include "Global.h"

#define BIT(x) (1<<x)

using namespace std;

//Game states
enum EGameState
{
	None = 0,
	GameStopped = BIT(0),
	GamePaused = BIT(1),
	GamePlaying = BIT(2),
};

class GamePlay
{
public:
	//Constructor
	GamePlay(shared_ptr<class DzX_Console> console);

	//Entry point - start the game from load file
	void StartLoadGame();
	//Entry point - start new game.
	void StartNewGame();

	//Game over event
	void GameOver();
	//Victory event
	void Victory();

	//Remove pickup from the m_Pickups container
	void RemovePickup(class Pickup* PickupToRemove);
	//Remove enemy from the m_Enemies container
	void RemoveEnemy(class Enemy* EnemyToRemove);
private:
	//Start the game
	void BeginPlay();

	/*
	* Play loop for the game.
	* Execute all the logic for drawing elements, updating stats and taking inputs.
	*/
	void Play();

	//Draw world
	void DrawWorld();

	//Saves game stats to XML
	void SaveGame();

	//Draw all enemies
	void DrawEnemies();
	//Change enemy position - randomly
	void MoveEnemies();
	//Draw pickups
	void DrawPickups();
	//Spawn HP Pots on random locations
	void SpawnHPPots(int NoOfPickups);
	//Spawn enemies on random locations
	void SpawnEnemies(int NoOfEnemies);

	//Get random point on playable area
	COORD GetRandomPoint(int Seed = 0);



	/************************************************************************/
	/*                        VARIABLES                                     */
	/************************************************************************/
public:
	//Playable area
	SMALL_RECT m_GamePlayArea;
	//State of the game
	EGameState m_GameState;

	//Container with pointers to alive enemies
	std::vector<class Enemy*>m_Enemies;
	//Container with pointers to available pickups
	std::vector<class Pickup*> m_Pickups;
	
	//#Optimization - 1 class is using it (private potential)
	//Pointer to console 
	shared_ptr<class DzX_Console> m_Console;
	//#Optimization - 1 class is using it (private potential)
	//Pointer to active player
	shared_ptr < class  Player > m_Player;
private:
	//Contains world layout
	std::string m_WorldLook;
	//Pointer to side menu
	shared_ptr<class HUD> m_HUD;
};