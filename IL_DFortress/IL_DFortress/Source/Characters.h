#pragma once
#include "Global.h"


using namespace std;


enum EPlayerState
{
	ALIVE,
	DEAD,
};

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
	/*Constructor
	* @Active console
	* @Spawn location on X
	* @Spawn location on Y
	*/
	Character();
	~Character();
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
	//On what field character is on top of
	char m_currentField;
public:
	CharStats m_Stats;
	EPlayerState m_PlayerState;
private:
	//Erase character from spot after he moved to another
	void EraseCharacter();
	//Checker to see if next field is available to move
	bool bCanMoveToNext(int X, int Y);
public:
	//Set field (symbol) on top of which char is standing
	void SetCurrentField(const char CurrentField);
	//Draw character on its position in the world
	void DrawCharacter();
	//Return position
	vector<int, int> GetPosition();
	//Move character to new location
	void MoveTo(int X, int Y);
	//Updates HP of the char
	void UpdateHP(int InChange);
	//Update Strength
	void UpdateSTR(int InChange);
	//Update defence
	void UpdateDEF(int InChange);

	void Spawn(class GamePlay* CurrentGame, shared_ptr<class DzX_Console>CurrentConsole, int spawnX, int spawnY);
protected:
	//Attack
	virtual void Attack(class Character* Enemy);
	//Death
	virtual void Die();
};