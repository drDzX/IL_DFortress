#include "DzX_ConsoleEngine.h"
#include <Windows.h>



DzX_Console::DzX_Console()
{
	
	m_handleConsoleOut=GetStdHandle(STD_OUTPUT_HANDLE);

	//m_handleConsoleIn = GetStdHandle(STD_INPUT_HANDLE);
	m_ScreenSize.X = 150;
	m_ScreenSize.Y = 150;
	m_ScreenRect.Top = 0;
	m_ScreenRect.Left = 0;
	m_ScreenRect.Bottom = m_ScreenSize.Y;
	m_ScreenRect.Right = m_ScreenSize.X;
}


void DzX_Console::GoToXY(int x, int y)
{
	m_CursorPos.X = x;
	m_CursorPos.Y = y;
	SetConsoleCursorPosition(m_handleConsoleOut, m_CursorPos);
	SetCursor(false, 20);
}

void DzX_Console::UpdateConsoleSize()
{
	if (m_handleConsoleOut == INVALID_HANDLE_VALUE)
	{
		Error(L"Bad Handle");
	}		
	SetConsoleScreenBufferSize(m_handleConsoleOut, m_ScreenSize);
	SetConsoleWindowInfo(m_handleConsoleOut, true, &m_ScreenRect);
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

void DzX_Console::Error(const wchar_t* msg)
{
	wchar_t buf[256];
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buf, 256, NULL);
	SetConsoleActiveScreenBuffer(m_handleConsoleOut);
	wprintf(L"ERROR: %s\n\t%s\n", msg, buf);

}

void DzX_Console::BeginPlay()
{
	m_Menu = make_unique<MainMenu>();
	if (m_Menu)
	{
		UpdateConsoleSize();
		m_Menu->NewGame(this);		
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