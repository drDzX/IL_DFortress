#include "MainMenu.h"

MainMenu::MainMenu()
{

}

void MainMenu::ButtonPressed(int Index)
{

}

void MainMenu::NewGame(DzX_Console*Console)
{
	shared_ptr<class DzX_Console> _cons(Console);
	m_Console = _cons;
	m_NewGameInstance = make_unique<class GamePlay>(m_Console);
	if (m_NewGameInstance)
	{
		m_NewGameInstance->BeginPlay();
	}
}
