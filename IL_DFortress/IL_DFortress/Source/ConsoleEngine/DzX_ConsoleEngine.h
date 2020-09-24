#pragma once

#include "Global.h"
#include <windows.h>
#include <conio.h>
#include <dos.h>




using namespace std;

class DzX_Console
{
public:
	DzX_Console();
	void BeginPlay();
	void GoToXY(int x, int y);
	void GoToXY(int x, int y, std::string text);
	void UpdateConsoleSize();
	char GetCharAtPosition(int X, int Y);

	void SetColor(int value);

	void Draw(int style, int startCol, int startRow, int width, int height, bool fill, int sw);
	shared_ptr<class MainMenu> m_Menu;
private:
	void SetCursor(bool bIsVisible, DWORD size);

protected:

	HANDLE m_handleConsoleOut;
	COORD m_CursorPos;
	_COORD  m_ScreenSize;
	_SMALL_RECT  m_ScreenRect;



	COORD GetConsoleCursorPosition(HANDLE hConsoleOutput);
	void Error(const wchar_t* msg);
};