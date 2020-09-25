#pragma once

#include "Global.h"

using namespace std;

//Class to manipulate console view for the game
class DzX_Console
{
public:
	DzX_Console();
	//#Entry - Start the console called on entry
	void BeginPlay();

	//Move cursor to position
	void GoToXY(int x, int y);
	//Move cursor to position and output string (overload)
	void GoToXY(int x, int y, std::string text);
	
	//Return character at asked position in console
	char GetCharAtPosition(int X, int Y);
	
	//Set color for upcoming text
	void SetColor(int value);

	/*Draw rectangle inside the console
	* @style - change style of rectangle border
	* @startCol - X coordinate where rectangle starts
	* @startRow - Y coordinate where rectangle starts
	* @width - width of the rectangle (on X axis)
	* @height - height of the rectangle (on Y axis)
	* @fill - clear everything inside the rectangle
	* @sw - draw shadow around the rectangle	
	*/
	void Draw(int style, int startCol, int startRow, int width, int height, bool fill, int sw);
private:
	//Set cursor visibility
	void SetCursor(bool bIsVisible, DWORD size);
	//Update console window size
	void UpdateConsoleSize();
protected:
	//Error reporter
	void Error(const wchar_t* msg);

/************************************************************************/
/*                        VARIABLES                                     */
/************************************************************************/
public:
	//Pointer to the main menu
	shared_ptr<class MainMenu> m_Menu;
protected:
	//Console output handle
	HANDLE m_handleConsoleOut;
	//Current cursor position
	COORD m_CursorPos;
	//Screen size
	_COORD  m_ScreenSize;
	_SMALL_RECT  m_ScreenRect;

};