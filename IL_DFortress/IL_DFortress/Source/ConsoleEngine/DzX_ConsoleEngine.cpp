#include "DzX_ConsoleEngine.h"
#include "Characters/Characters.h"
#include "Menu/MainMenu.h"


DzX_Console::DzX_Console()
{
	//Get console OUT handle
	m_handleConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);

	//Set screen size - for buffer
	m_ScreenSize.X = 112;
	m_ScreenSize.Y = 30;
	//Make screen size rectangle - for window
	m_ScreenRect.Top = 0;
	m_ScreenRect.Left = 0;
	m_ScreenRect.Bottom = m_ScreenSize.Y;
	m_ScreenRect.Right = m_ScreenSize.X;
}


void DzX_Console::GoToXY(int x, int y)
{
	//Change cursor position location
	m_CursorPos.X = x;
	m_CursorPos.Y = y;
	//Set cursor invisible
	SetCursor(false, 20);
	//Change cursor position
	SetConsoleCursorPosition(m_handleConsoleOut, m_CursorPos);
}

void DzX_Console::GoToXY(int x, int y, std::string text)
{
	//Change cursor position location
	m_CursorPos.X = x;
	m_CursorPos.Y = y;
	//Set cursor invisible
	SetCursor(false, 20);
	//Change cursor position
	SetConsoleCursorPosition(m_handleConsoleOut, m_CursorPos);
	cout << text;
}

void DzX_Console::UpdateConsoleSize()
{
	//Check for handle
	if (m_handleConsoleOut == INVALID_HANDLE_VALUE)
	{
		Error(L"Bad Handle");
	}
	//Set cursor invisable
	SetCursor(false, 20);

	//Setup console buffer size
	SetConsoleScreenBufferSize(m_handleConsoleOut, m_ScreenSize);
	//Setup console window size
	SetConsoleWindowInfo(m_handleConsoleOut, true, &m_ScreenRect);
}

char DzX_Console::GetCharAtPosition(int X, int Y)
{
	//Container to store character
	char buffer[2];
	DWORD num_read;
	//Position to take char from
	COORD loc;
	loc.X = X;
	loc.Y = Y;
	//Check for handle
	if (m_handleConsoleOut)
	{
		//Read char from asked position
		ReadConsoleOutputCharacter(m_handleConsoleOut, (LPTSTR)buffer, 1, loc, (LPDWORD)&num_read);
	}
	//Return char
	return buffer[0];
}

void DzX_Console::SetColor(int value)
{
	//Check for handle
	if (m_handleConsoleOut)
	{
		//Set text color for given handle
		SetConsoleTextAttribute(m_handleConsoleOut, value);
	}
}

void DzX_Console::Draw(int style, int startCol, int startRow, int width, int height, bool fill, int sw)
{
	// Draws a 1 or 2 line box 
	int a;
	//Limit shadow to 4 styles
	if (sw > 4)
	{
		sw = 4;
	}

	style = (style - 1) * 6;
	//Container for rectangle border characters
	char box[12];
	//Container for shadow border characters
	char shdw[5];

	box[0] = '\xDA';//  ┌
	box[1] = '\xBF';//  ┐
	box[2] = '\xC0';//  └
	box[3] = '\xD9';//  ┘
	box[4] = '\xB3';//  │ 
	box[5] = '\xC4';//  ─
	box[6] = '\xC9';//  ╔ 
	box[7] = '\xBB';//  ╗
	box[8] = '\xC8';//  ╚
	box[9] = '\xBC';//  ╝
	box[10] = '\xBA';// ║
	box[11] = '\xCD';// ═
	shdw[1] = '\xB0';// ░
	shdw[2] = '\xB1';// ▒
	shdw[3] = '\xB2';// ▓
	shdw[4] = '\xDB';// █

	//Characters for each edge
	char tl, tr, bl, br, side, edge, shadow;
	tl = box[style];
	tr = box[style + 1];
	bl = box[style + 2];
	br = box[style + 3];
	side = box[style + 4];
	edge = box[style + 5];
	shadow = shdw[sw];


	std::string Line(width - 2, edge);
	std::string Shadow(width, shadow);
	std::string Fill(width - 2, ' ');

	//Drawing method for shadow and fill
	GoToXY(startCol, startRow);
	cout << tl << Line << tr;
	for (a = 1; a < height - 1; a++)
	{
		GoToXY(startCol, startRow + a);
		cout << side;
		if (fill)
			cout << Fill;
		else
			GoToXY(startCol + width - 1, startRow + a);
		cout << side;
		if (sw)
			cout << shadow;
	}
	//Drawing method for shadow and fill
	GoToXY(startCol, (height + startRow) - 1);
	cout << bl << Line << br;
	if (sw)
	{
		cout << shadow;
		GoToXY(startCol + 1, startRow + height, Shadow);
	}
}

void DzX_Console::SetCursor(bool bIsVisible, DWORD size)
{
	//Resize cursor if input is 0
	if (size == 0)
	{
		size = 20;
	}
	//Create new curosr info
	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = bIsVisible;
	lpCursor.dwSize = size;
	//Set curosr to new info
	SetConsoleCursorInfo(m_handleConsoleOut, &lpCursor);
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
	//Change color to white
	SetColor(7);
	//Create Main menu 
	m_Menu = make_shared<MainMenu>();
	//Validate menu
	if (m_Menu)
	{
		//Update console size to match params
		UpdateConsoleSize();
		//Debug branch, start game or load menu
		if (PR_DEBUG == 1)
		{
			//Start a new game from  the menu after loading console
			m_Menu->NewGame(this);
		}
		else
		{
			//Load main menu after loading the console
			m_Menu->LoadMenu(this);
		}
	}
}
//#Main - Entry point
int main()
{
	//Create console
	unique_ptr<DzX_Console>Console = make_unique<DzX_Console>();
	//Validate console
	if (Console)
	{
		//Start console
		Console->BeginPlay();
	}
	//Exit blocking for debug
	std::cin.get();
}