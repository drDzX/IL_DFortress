#pragma once
#include <iostream>
#include <memory>
#include <iterator>
#include <algorithm>
#include <fstream>
#include <thread>
#include <future>
#include <vector>
#include "json.hpp"

#include "ConsoleEngine/DzX_ConsoleEngine.h"


using json = nlohmann::json;

//Symbols that represent different objects in the game.
enum class EObjectType :char
{
	WALL = '#',
	SPAWNABLE='.',
	TREE = 'A',
	PLAYER = 'X',
	ENEMY_1 = 'H',
	ARMOR = 'O',
	WEAPON= 'I',
	HPPOTION = '8',
	ADRENALIN='!',
};

std::string ReadJson(const char InPath[], const  char elemKey[]);


