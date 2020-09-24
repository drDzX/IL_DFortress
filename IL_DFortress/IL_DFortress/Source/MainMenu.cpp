#include "MainMenu.h"
#include <cstdlib>

MainMenu::MainMenu()
{
	m_bIsInMenu = true;
}

void MainMenu::ButtonPressed(int Index)
{

}

void MainMenu::NewGame(DzX_Console* Console)
{
	//Need to check for console because in debug we jump start new instance.
	if (PR_DEBUG == 1)
	{
		if (!m_Console && Console)
		{
			shared_ptr<class DzX_Console> _cons(Console);
			m_Console = _cons;
		}
	}
	if (m_Console)
	{
		m_NewGameInstance = make_unique<class GamePlay>(m_Console);
		if (m_NewGameInstance)
		{

			m_NewGameInstance->StartNewGame();
		}
	}
}

void MainMenu::LoadGame()
{
	if (m_Console)
	{
		m_NewGameInstance = make_unique<class GamePlay>(m_Console);
		if (m_NewGameInstance)
		{

			m_NewGameInstance->StartLoadGame();
		}
	}
}

void MainMenu::LoadMenu(DzX_Console* Console)
{
	m_bIsInMenu = true;
	shared_ptr<class DzX_Console> _cons(Console);
	m_Console = _cons;

	m_Console->Draw(2, 0, 0, 31, 8, 0, 1);
	m_Console->GoToXY(1, 1);
	cout << "--------- Main Menu ---------"<<endl;
	m_Console->GoToXY(1, 3);
	cout << "	1.New Game" << endl;
	m_Console->GoToXY(1, 4);
	cout << "	2.Load Game" << endl;
	m_Console->GoToXY(1, 5);
	cout << "	0.Exit" << endl;

	m_Console->UpdateConsoleSize();
	while (m_bIsInMenu)
	{
		//Catch input
		if (_kbhit())
		{
			char ch = _getch();
			if (ch == '1')
			{
				//Turn looping off
				m_bIsInMenu = false;
				//Make a new game
				NewGame();
			}
			if (ch == '2')
			{
				//Load
				LoadGame();

			}
			if (ch == '0')
			{
				exit(0);
			}
		}
	}

}
