#pragma once
#include "Global.h"

class HUD
{
	
public:
	
	HUD(GamePlay* GameMode);

	int m_PlayerMaxHP;
	shared_ptr < class  Player > m_Player;
	shared_ptr<class GamePlay> m_Game;
	shared_ptr<class DzX_Console> m_Console;

	void DrawHud();
	void UpdateHud();
	

	void BeginPlay();
	void GetInterfaceLook();

private:
	vector<std::string> m_borkenStringUITop;
	vector<std::string> m_borkenStringUIBot;
};