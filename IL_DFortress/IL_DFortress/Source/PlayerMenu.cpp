#include "PlayerMenu.h"
#include "Player.h"
#include "Game.h"

HUD::HUD(GamePlay* GameMode)
{
	if (GameMode)
	{
		shared_ptr<class DzX_Console> _console(GameMode->m_Console);
		shared_ptr<class Player> _ply(GameMode->m_Player.get());
		m_Player = _ply;
		m_Console = _console;
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
		for (auto s : m_borkenStringUITop)
		{
			//Offset it to match UI position
			m_Console->GoToXY(61, 1 + RowAdd);
			//Print
			std::cout << s;
			//Add for next row
			RowAdd++;
		}
		for (auto s : m_borkenStringUIBot)
		{
			//Offset it to match UI position
			m_Console->GoToXY(61, 23 + RowAdd);
			//Print
			std::cout << s;
			//Add for next row
			RowAdd++;
		}
		//Top menu
		m_Console->Draw(2, 61, 0, 50, 5, 0, 0);
		//Enemies
		m_Console->Draw(1, 61, 13, 50, 4, 0, 0);
		//Armor
		m_Console->Draw(1, 61, 10, 25, 3, 0, 0);
		//Weapon
		m_Console->Draw(1, 86, 10, 25, 3, 0, 0);
		//Consumables
		m_Console->Draw(1, 61, 7, 50, 3, 0, 0);
		//CharStats
		m_Console->Draw(1, 61, 5, 50, 3, 0, 0);

		//Enemy 1 stats to display
		char E1_Look = static_cast<char>(EObjectType::ENEMY_1);
		std::string E1_HP = ReadXML("Config/GamePlay_Settings.xml", "Enemy1", "HP");
		std::string E1_STR = ReadXML("Config/GamePlay_Settings.xml", "Enemy1", "STR");
		std::string E1_DEF = ReadXML("Config/GamePlay_Settings.xml", "Enemy1", "DEF");
		//Enemy 2 stats to display
		char E2_Look = static_cast<char>(EObjectType::ENEMY_2);
		std::string E2_HP = ReadXML("Config/GamePlay_Settings.xml", "Enemy2", "HP");
		std::string E2_STR = ReadXML("Config/GamePlay_Settings.xml", "Enemy2", "STR");
		std::string E2_DEF = ReadXML("Config/GamePlay_Settings.xml", "Enemy2", "DEF");

		m_Console->GoToXY(63, 14);
		cout << E1_Look << " - Enemy | HP:" << E1_HP << " STR:" << E1_STR << " DEF:" << E1_DEF;
		m_Console->GoToXY(63, 15);
		cout << E2_Look << " - Enemy | HP:" << E2_HP << " STR:" << E2_STR << " DEF:" << E2_DEF;
	}
}

void HUD::UpdateHud()
{
	if (m_Console && m_Player)
	{
		m_Console->GoToXY(62, 6);

		int CurrHP = m_Player->m_Stats.CurrentHealth > 0 ? m_Player->m_Stats.CurrentHealth : 0;
		cout << "	" << "HP: " << CurrHP << " / " << m_PlayerMaxHP;

		cout << "	| ";
		cout << "STR: " << m_Player->m_Stats.Strength;
		cout << "	| ";
		cout << "DEF: " << m_Player->m_Stats.Defence << "";




		m_Console->GoToXY(62, 8);
		cout << "(1) 8 - HP Poitons: " << m_Player->Slot1.Amount;

		int Armor = m_Player->m_Stats.Defence - m_Player->m_BaseStats.Defence;
		m_Console->GoToXY(64, 11);
		if (Armor > 0)
		{

			cout << "+" << Armor << " ARMOR(DEF)";
		}
		else
		{
			cout << static_cast<char>(EObjectType::ARMOR) << " - NO ARMOR";
		}
		m_Console->GoToXY(89, 11);
		int Weapon = m_Player->m_Stats.Strength - m_Player->m_BaseStats.Strength;
		if (Weapon > 0)
		{
			cout << "+" << Weapon << " WEAPON(STR)";
		}
		else
		{
			cout << static_cast<char>(EObjectType::WEAPON) << " - NO WEAPON";
		}
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
		std::string top = ReadXML("Config/HUD.xml", "Top", "Topology");
		m_borkenStringUITop = explode(top, '\n');

		std::string bot = ReadXML("Config/HUD.xml", "Bot", "Topology");
		m_borkenStringUIBot = explode(bot, '\n');

	}

}
