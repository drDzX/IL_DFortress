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
public:
	GamePlay(shared_ptr<class DzX_Console> console);

	void BeginPlay();
	void Play();
	EGameState m_GameState;

	std::array<class Enemy*, 15> m_Enemies;

		shared_ptr < class  Player > m_Player;

	void GameOver();

private:
	void DrawWorld();
	void DrawEnemies();
	void MoveEnemies();
	
	void SpawnEnemies(int NoOfEnemies);

	COORD GetRandomPoint(int Seed=0);



protected:
	shared_ptr<class DzX_Console> m_Console;
};