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
	std::array<class Enemy*, 15> m_Enemies;
	std::array<class Pickup*, 15> m_Pickups;

	shared_ptr<class DzX_Console> m_Console;
	shared_ptr < class  Player > m_Player;
	shared_ptr<class HUD> m_HUD;

	void GameOver();

private:
	void DrawWorld();


	void DrawEnemies();
	void MoveEnemies();

	void DrawPickups();
	void SpawnPickup(int NoOfPickups);

	void SpawnEnemies(int NoOfEnemies);

	COORD GetRandomPoint(int Seed=0);

};