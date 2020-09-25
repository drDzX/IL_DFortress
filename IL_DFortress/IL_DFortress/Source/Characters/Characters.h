#pragma once
#include "Global.h"

using namespace std;

//Character states
enum EPlayerState
{
	ALIVE,
	DEAD,
};
//Character stats
struct CharStats
{
	int MaxHealth;
	int CurrentHealth;
	int Strength;
	int Defence;

	CharStats()
	{
		MaxHealth = 10;
		CurrentHealth = MaxHealth;
		Strength = 1;
		Defence = 1;
	}
};

class Character
{
public:
	//Constructor
	Character();

public:
	//Set field (symbol) on top of which char is standing
	void SetCurrentField(const char CurrentField);
	//Draw character on its position in the world
	void DrawCharacter();
	//Return position
	COORD GetPosition();
	//Move character to new location by adding to existing position
	void MoveTo(int addX, int addY);
	//Updates HP of the char
	void UpdateHP(int InChange);
	//Change Strength
	void UpdateSTR(int InChange);
	//Change Defence
	void UpdateDEF(int InChange);

	//Spawn character at location
	virtual void Spawn(class GamePlay* CurrentGame, shared_ptr<class DzX_Console>CurrentConsole, int spawnX, int spawnY);
	//Attack opponent
	virtual void Attack(class Character* Enemy);
protected:
	//Death virtual
	virtual void Die();
private:
	//Erase character from spot after he moved to another
	void EraseCharacter();
	//Checker to see if next field is available to move
	bool bCanMoveToNext(int X, int Y);
/************************************************************************/
/*                        VARIABLES                                     */
/************************************************************************/
public:
	//Char stats variable
	CharStats m_Stats;
	//Char base stats to compare against
	CharStats m_BaseStats;
	//Type of this character / look
	EObjectType m_CharType;
	//On what field character is on top of
	char m_currentField;
	//State of this char
	EPlayerState m_PlayerState;
protected:
	//Pointer to active console
	shared_ptr<class DzX_Console> m_ActiveConsole;

	//Pointer to game mode
	class GamePlay* m_ActiveGame;
	//Shape of this character
	std::wstring m_look;
	//Current position X and Y
	int m_posX;
	int m_posY;

};