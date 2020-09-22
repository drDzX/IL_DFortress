#pragma once

#include "Global.h"
#include <windows.h>
#include <conio.h>
#include <dos.h>
#include "Characters.h"
#include "MainMenu.h"




using namespace std;

class DzX_Console
{
public:
	DzX_Console();
	void BeginPlay();
	void GoToXY(int x, int y);
	void UpdateConsoleSize();
	char GetCharAtPosition(int X, int Y);

private:
	void SetCursor(bool bIsVisible, DWORD size);

protected:

	HANDLE m_handleConsoleOut;
	COORD m_CursorPos;
	_COORD  m_ScreenSize;
	_SMALL_RECT  m_ScreenRect;
	unique_ptr<class MainMenu> m_Menu;


	COORD GetConsoleCursorPosition(HANDLE hConsoleOutput);
	void Error(const wchar_t* msg);
};