#include "DzX_ConsoleEngine.h"
#include <iostream>
#include <iterator>
#include <algorithm>
#include <fstream>


DzX_Console::DzX_Console()
{
	
	m_handleConsoleOut=GetStdHandle(STD_OUTPUT_HANDLE);

	//m_handleConsoleIn = GetStdHandle(STD_INPUT_HANDLE);
	m_ScreenX = 60;
	m_ScreenY = 30;
}


void DzX_Console::GoToXY(int x, int y)
{
	m_CursorPos.X = x;
	m_CursorPos.Y = y;
	SetConsoleCursorPosition(m_handleConsoleOut, m_CursorPos);
	SetCursor(false, 20);
}

char DzX_Console::GetCharAtPosition(int X, int Y)
{
	char buffer[2];
	DWORD num_read;
	COORD loc;
	loc.X = X;
	loc.Y = Y;
	//COORD loc=GetConsoleCursorPosition(m_handleConsoleOut);
	

	ReadConsoleOutputCharacter(m_handleConsoleOut, (LPTSTR)buffer, 1, loc, (LPDWORD)&num_read);
	
	return buffer[0];
}

void DzX_Console::SetCursor(bool bIsVisible, DWORD size)
{
	if (size == 0)
	{
		size = 20;
	}
	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = bIsVisible;
	lpCursor.dwSize = size;
	SetConsoleCursorInfo(m_handleConsoleOut, &lpCursor);
}


COORD DzX_Console::GetConsoleCursorPosition(HANDLE hConsoleOutput)
{
	CONSOLE_SCREEN_BUFFER_INFO cbsi;
	if (GetConsoleScreenBufferInfo(hConsoleOutput, &cbsi))
	{
		COORD c = cbsi.dwCursorPosition;
		c.X -= 1;
		return c;
	}
	else
	{
		// The function failed. Call GetLastError() for details.
		COORD invalid = { 0, 0 };
		return invalid;
	}
}

void DzX_Console::Play()
{
	//system("cls");
	//SetCursor(false, 20);
	//DrawWorld();
	//
	//while (1)
	//{
	//	if (_kbhit())
	//	{
	//		char ch = _getch();
	//		if (ch == 'a' || ch == 'A')
	//		{
	//			cout << "Pressed";
	//		}
	//	}
	//}
	
	//DrawPlayer();
}


void DzX_Console::BeginPlay()
{
	shared_ptr<DzX_Console> console = make_shared<DzX_Console>();
	m_Menu = make_unique<MainMenu>();
	if (m_Menu)
	{
		m_Menu->NewGame(move(console));
		//this->Play();
	}
	
}

int main()
{
	DzX_Console* Console = new DzX_Console();
	if (Console)
	{	
		Console->BeginPlay();

	}
	//std::string sout = Console->ReadJson("Config/world.json");
	std::cin.get();
	delete Console;

}