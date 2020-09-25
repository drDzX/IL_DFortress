#include "PlayerMenu.h"
#include "Characters/Player.h"
#include "Game.h"

HUD::HUD(GamePlay* GameMode)
{
	if (GameMode)
	{
		//Sets pointer to console
		shared_ptr<class DzX_Console> _console(GameMode->m_Console);
		m_Console = _console;
		//Sets pointer to player
		shared_ptr<class Player> _ply(GameMode->m_Player.get());
		m_Player = _ply;

	}
	if (m_Player)
	{
		//Read max hp from player stats
		m_PlayerMaxHP = m_Player->m_Stats.MaxHealth;
	}
	//Get interface look from XML file
	GetInterfaceLook();
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
			m_Console->GoToXY(62, 1 + RowAdd);
			//Print
			std::cout << s;
			//Add for next row
			RowAdd++;
		}
		RowAdd = 0;
		for (auto s : m_borkenStringUIBot)
		{
			//Offset it to match UI position
			m_Console->GoToXY(62, 27 + RowAdd);
			//Print
			std::cout << s;
			//Add for next row
			RowAdd++;
		}
		//Top menu
		m_Console->Draw(2, 62, 0, 50, 7, 0, 0);
		//Enemies
		m_Console->Draw(1, 62, 15, 50, 4, 0, 0);
		//Armor
		m_Console->Draw(1, 62, 12, 25, 3, 0, 0);
		//Weapon
		m_Console->Draw(1, 87, 12, 25, 3, 0, 0);
		//Consumables
		m_Console->Draw(1, 62, 9, 50, 3, 0, 0);
		//CharStats
		m_Console->Draw(1, 62, 7, 50, 3, 0, 0);

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
		
		//Draw HP / STR / DEF for enemies
		m_Console->GoToXY(64, 16);
		cout << E1_Look << " - Enemy | HP:" << E1_HP << " STR:" << E1_STR << " DEF:" << E1_DEF;
		m_Console->GoToXY(64, 17);
		cout << E2_Look << " - Enemy | HP:" << E2_HP << " STR:" << E2_STR << " DEF:" << E2_DEF;
	}
}

void HUD::UpdateHud()
{
	if (m_Console && m_Player)
	{
	
#pragma region Player HP STR DEF
		//Line location
		m_Console->GoToXY(62, 8);

		//Update Current HP
		int CurrHP = m_Player->m_Stats.CurrentHealth > 0 ? m_Player->m_Stats.CurrentHealth : 0;
		cout << "	" << "HP: " << CurrHP << " / " << m_PlayerMaxHP<<"  ";
		//Update STR / DEF
		cout << "	| ";
		cout << "STR: " << m_Player->m_Stats.Strength;
		cout << "	| ";
		cout << "DEF: " << m_Player->m_Stats.Defence << "";
#pragma endregion Player HP STR DEF



#pragma region HP Pots
		//Line location
		m_Console->GoToXY(63,10);
		//HP pots amount
		cout << "(1) 8 - HP Poitons: " << m_Player->Slot1.Amount;
#pragma endregion HP Pots

#pragma region Inventory Armor
	//Line location
		m_Console->GoToXY(65, 13);
		//Armor stats
		int Armor = m_Player->m_Stats.Defence - m_Player->m_BaseStats.Defence;
		if (Armor > 0)
		{

			cout << "+" << Armor << " ARMOR(DEF)";
		}
		else
		{
			cout << static_cast<char>(EObjectType::ARMOR) << " - NO ARMOR";
		}
#pragma endregion Inventory Armor

#pragma region Inventory Weapon
		//Line location
		m_Console->GoToXY(90, 13);
		//Weapon stats
		int Weapon = m_Player->m_Stats.Strength - m_Player->m_BaseStats.Strength;
		if (Weapon > 0)
		{
			cout << "+" << Weapon << " WEAPON(STR)";
		}
		else
		{
			cout << static_cast<char>(EObjectType::WEAPON) << " - NO WEAPON";
		}
#pragma endregion Inventory Weapon
	}
}


void HUD::GetInterfaceLook()
{
	if (m_Console)
	{
		//Get upper menu part
		std::string top = ReadXML("Config/HUD.xml", "Top", "Topology");
		//Parse string to rows
		m_borkenStringUITop = ExplodeString(top, '\n');
		//Get lower menu part
		std::string bot = ReadXML("Config/HUD.xml", "Bot", "Topology");
		//Parse string to rows
		m_borkenStringUIBot = ExplodeString(bot, '\n');
	}
}
