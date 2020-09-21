#pragma once
#include "Game.h"
#include <memory>
#include <iostream>
#include "ConsoleEngine/DzX_ConsoleEngine.h"
using namespace std;

class MainMenu
{


public:
	MainMenu();
	void ButtonPressed(int Index);

	void NewGame(shared_ptr<class DzX_Console> console);

	unique_ptr<class GamePlay> m_NewGameInstance;
	 shared_ptr<class DzX_Console> m_Console;
};