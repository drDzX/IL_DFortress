#pragma once
#include "Global.h"
#include "Game.h"

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