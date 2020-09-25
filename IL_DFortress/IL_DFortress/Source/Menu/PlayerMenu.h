#pragma once
#include "Global.h"

class HUD
{
	
public:
	//Constructor
	HUD(class GamePlay* GameMode);

	//Draw this component in console
	void DrawHud();
	//Update/Redraw variable stats
	void UpdateHud();

	//Get XML data for graphic style of this interface
	void GetInterfaceLook();

/************************************************************************/
/*                        VARIABLES                                     */
/************************************************************************/
private:
	//Parsed strings for menu look
	vector<std::string> m_borkenStringUITop;
	vector<std::string> m_borkenStringUIBot;
	
	//Stored player MaxHP
	int m_PlayerMaxHP;
	//Pointer to player
	shared_ptr < class  Player > m_Player;
	//Pointer to console
	shared_ptr<class DzX_Console> m_Console;

};