#pragma once
#include "Global.h"


#define BIT(x) (1<<x)

using namespace std;

enum EGameState
{
	None = 0,
	GameStopped = BIT(0),
	GamePaused = BIT(1),
	GamePlaying = BIT(2),
};

class GamePlay
{

private:
	SMALL_RECT m_GamePlayArea;
	std::string m_WorldLook;
public:
	GamePlay(shared_ptr<class DzX_Console> console);

	void BeginPlay();
	void Play();


	EGameState m_GameState;
	std::vector<class Enemy*>m_Enemies;
	std::vector<class Pickup*> m_Pickups;

	shared_ptr<class DzX_Console> m_Console;
	shared_ptr < class  Player > m_Player;
	shared_ptr<class HUD> m_HUD;

	void GameOver();

	void RemovePickup(class Pickup* PickupToRemove);
	void RemoveEnemy(class Enemy* EnemyToRemove);

	void StartLoadGame();
	void StartNewGame();
private:
	void DrawWorld();


	void SaveGame();

	void DrawEnemies();
	void MoveEnemies();

	void DrawPickups();
	void SpawnPickup(int NoOfPickups);

	void SpawnEnemies(int NoOfEnemies);

	COORD GetRandomPoint(int Seed=0);

};