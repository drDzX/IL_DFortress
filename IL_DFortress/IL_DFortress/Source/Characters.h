#pragma once
#include "ConsoleEngine/DzX_ConsoleEngine.h"
#include <iostream>
#include <memory>

using namespace std;
class Character
{

public:
	Character(shared_ptr<class DzX_Console>CurrentConsole, int spawnX,int spawnY);
private:
	shared_ptr<DzX_Console> m_ActiveConsole;
	std::wstring m_look;
	int m_posX;
	int m_posY;

	char m_currentField;

	void EraseCharacter();



	bool bCanMoveToNext(int X, int Y);
public:
	void SetCurrentField(const char CurrentField);
	void DrawCharacter();
	vector<int, int> GetPosition();
	void MoveTo(int X, int Y);

};