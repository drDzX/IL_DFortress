#include "PlayerMenu.h"
#include "Player.h"
#include "Game.h"

HUD::HUD(GamePlay* GameMode)
{
	if (GameMode)
	{
		m_Player = GameMode->m_Player;
		m_Console = GameMode->m_Console;
		shared_ptr<class GamePlay> PG(GameMode);
		m_Game= PG;
	}
	if (m_Player)
	{
		m_PlayerMaxHP = m_Player->m_Stats.MaxHealth;
	}
}
const vector<std::string> explode(const std::string& s, const char& c)
{
	std::string buff{ "" };
	vector<std::string> v;

	for (auto n : s)
	{
		if (n != c) buff += n; else
			if (n == c && buff != "") { v.push_back(buff); buff = ""; }
	}
	if (buff != "") v.push_back(buff);

	return v;
}
void HUD::DrawHud()
{
	if (m_Console)
	{
		//Add row for each NewLine
		int RowAdd = 0;		
		//Loop each line
		for (auto s : m_borkenString)
		{
			//Offset it to match UI position
			m_Console->GoToXY(70, 0+ RowAdd);
			//Print
			std::cout << s;
			//Add for next row
			RowAdd++;
		}

		m_Console->GoToXY(70, 10);
		cout << "HP: " << m_Player->m_Stats.CurrentHealth << " / " << m_PlayerMaxHP;
		cout << "	|	";
		cout << "STR: " << m_Player->m_Stats.Strength;
		cout << "	|	";
		cout << "DEF: " << m_Player->m_Stats.Defence;
	}
}

void HUD::BeginPlay()
{
	GetInterfaceLook();
}

void HUD::GetInterfaceLook()
{
	if (m_Console)
	{
		m_InterfaceLook = ReadJson("Config/HUD.json", "World");
		m_borkenString = explode(m_InterfaceLook, '\n');
		
	}

}
