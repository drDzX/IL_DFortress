#pragma once
#include "Global.h"


using namespace std;

class MainMenu
{
public:
	MainMenu();

	//Declared to call NewGame for debug purposes;
	friend DzX_Console;

	//Load this menu
	void LoadMenu(DzX_Console* Console = nullptr);

private:
	//Draw menu look
	void DrawMenu();
	//Create a new game instance
	void NewGame(DzX_Console* Console = nullptr /* this input is for debugg mode*/);

	//Create a new game by loading saved game
	void LoadGame();

/************************************************************************/
/*                        VARIABLES                                     */
/************************************************************************/
private:
	//Is focus on this menu or not
	bool m_bIsInMenu;
	//Pointer to an game instance
	class GamePlay* m_GameInstance;
	//Pointer to the console
	shared_ptr<class DzX_Console> m_Console;
};