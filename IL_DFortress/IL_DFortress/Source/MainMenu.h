#pragma once
#include "Global.h"
#include "Game.h"

using namespace std;

class MainMenu
{


public:
	MainMenu();

	void ButtonPressed(int Index);

	void NewGame(DzX_Console* Console = nullptr /* this input is for debugg mode*/);
	void LoadGame();

	void LoadMenu(DzX_Console* Console = nullptr);
	void DrawMenu();
	bool m_bIsInMenu;

	class GamePlay* m_GameInstance;
	shared_ptr<class DzX_Console> m_Console;
};