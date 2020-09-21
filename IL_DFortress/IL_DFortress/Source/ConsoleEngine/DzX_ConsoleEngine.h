#pragma once
#include "json.hpp"
#include <windows.h>
#include <conio.h>
#include <dos.h>
#include <memory>
#include "Characters.h"
#include "MainMenu.h"
#include <iostream>



using namespace std;

class DzX_Console
{
public:
	DzX_Console();
	void Play();
	void BeginPlay();
	void GoToXY(int x, int y);

	char GetCharAtPosition(int X, int Y);

private:
	void SetCursor(bool bIsVisible, DWORD size);

protected:
	int m_ScreenX;
	int m_ScreenY;
	HANDLE m_handleConsoleOut;
	COORD m_CursorPos;
	unique_ptr< class MainMenu> m_Menu;

	COORD GetConsoleCursorPosition(HANDLE hConsoleOutput);
};