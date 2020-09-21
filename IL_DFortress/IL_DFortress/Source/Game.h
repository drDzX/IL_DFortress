#pragma once
#include <iostream>
#include <memory>
#include "Characters.h"
#include "ConsoleEngine/DzX_ConsoleEngine.h"

#define BIT(x) (1<<x)

using namespace std;
using json = nlohmann::json;

enum class ESymbols :char
{
	WALL = '#',
	TREE = 'A',
};
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
	GamePlay(shared_ptr<class DzX_Console> console);

	void BeginPlay();
	void Play();
	EGameState m_GameState;


	shared_ptr < class  Character > m_Player;

private:
	void DrawWorld();
	std::string ReadJson(const char InPath[], const  char elemKey[]);
protected:
	json m_World_Data;
	shared_ptr<class DzX_Console> m_Console;
};