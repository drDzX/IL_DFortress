#include "MainMenu.h"

MainMenu::MainMenu()
{

}

void MainMenu::ButtonPressed(int Index)
{

}

void MainMenu::NewGame(shared_ptr<DzX_Console> console)
{
	m_Console = console;
	m_NewGameInstance = make_unique<GamePlay>(m_Console);
	if (m_NewGameInstance)
	{
		m_NewGameInstance->BeginPlay();
	}
}
