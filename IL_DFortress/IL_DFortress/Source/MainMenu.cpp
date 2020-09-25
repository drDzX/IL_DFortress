#include "MainMenu.h"
#include <cstdlib>

MainMenu::MainMenu()
{
	m_bIsInMenu = true;
	m_GameInstance = nullptr;
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
		if (!m_GameInstance)
		{
			m_GameInstance = new GamePlay(m_Console);
		}
		if (m_GameInstance)
		{
			m_GameInstance->StartNewGame();
		}
	}
}

void MainMenu::LoadGame()
{
	if (m_Console)
	{
		m_GameInstance = new GamePlay(m_Console);

		if (m_GameInstance)
		{
			m_GameInstance->StartLoadGame();
		}
	}
}

void MainMenu::LoadMenu(DzX_Console* Console)
{
	m_bIsInMenu = true;
	if (m_GameInstance)
	{
		m_GameInstance = nullptr;
	}
	if (!m_Console)
	{
		shared_ptr<class DzX_Console> _cons(Console);
		m_Console = _cons;
	}

	DrawMenu();

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

void MainMenu::DrawMenu()
{
	system("cls");

	m_Console->Draw(2, 0, 4, 50, 8, 0, 1);
	m_Console->GoToXY(11, 5);
	cout << "--------- Main Menu ---------" << endl;
	m_Console->GoToXY(11, 7);
	cout << "	1.New Game" << endl;
	m_Console->GoToXY(11, 8);
	cout << "	2.Load Game" << endl;
	m_Console->GoToXY(11, 9);
	cout << "	0.Exit" << endl;

	std::string top = ReadXML("Config/HUD.xml", "Top", "Topology");
	vector<std::string> m_borkenStringUITop = ExplodeString(top, '\n');
	
	//Add row for each NewLine
	int RowAdd = 0;
	//Loop each line
	for (auto s : m_borkenStringUITop)
	{
		//Offset it to match UI position
		m_Console->GoToXY(0, 0 + RowAdd);
		//Print
		std::cout << s;
		//Add for next row
		RowAdd++;
	}
}
