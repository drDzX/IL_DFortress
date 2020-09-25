#include "MainMenu.h"

#include "Game.h"
MainMenu::MainMenu()
{
	//Is in menu
	m_bIsInMenu = true;
	//No game instance on menu creation
	m_GameInstance = nullptr;
}

void MainMenu::NewGame(DzX_Console* Console)
{
	PROFILING_FUNCTION();
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
	PROFILING_FUNCTION();
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

	if (!Instrumentor::Get().IsSessionInProgress())
	{	
		//Begin profiling
		Instrumentor::Get().BeginSession("Profile");
	}

	PROFILING_FUNCTION();
	//Focus is in this menu
	m_bIsInMenu = true;

	/*#Optimization
	Remove game instance from this pointer*/
	if (m_GameInstance)
	{
		m_GameInstance = nullptr;
	}
	//Check for console pointer, and if none get pointer to it
	if (!m_Console)
	{
		shared_ptr<class DzX_Console> _cons(Console);
		m_Console = _cons;
	}
	
	
	//Draw menu look
	DrawMenu();

	//Loop while in this menu
	while (m_bIsInMenu)
	{
		//Catch input
		if (_kbhit())
		{
			//Get char that is inputed
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
				//Load game
				LoadGame();

			}
			if (ch == '0')
			{

				//Finish profiling
				Instrumentor::Get().EndSession();
				//Exit application
				exit(0);
			}
		}
	}
}

void MainMenu::DrawMenu()
{
	PROFILING_FUNCTION();
	//Clear console
	system("cls");

	//Draw menu options
	m_Console->Draw(2, 0, 4, 50, 8, 0, 1);
	m_Console->GoToXY(11, 5);
	cout << "--------- Main Menu ---------" << endl;
	m_Console->GoToXY(11, 7);
	cout << "	1.New Game" << endl;
	m_Console->GoToXY(11, 8);
	cout << "	2.Load Game" << endl;
	m_Console->GoToXY(11, 9);
	cout << "	0.Exit" << endl;

	//Get graphics from the config XML
	std::string top = ReadXML("Config/HUD.xml", "Top", "Topology");
	vector<std::string> m_borkenStringUITop = ExplodeString(top, '\n');
	
	//Parse string and output each row where it should go
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
