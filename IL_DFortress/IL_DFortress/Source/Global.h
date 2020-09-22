#pragma once
#include <iostream>
#include <memory>
#include <iterator>
#include <algorithm>
#include <fstream>
#include <thread>
#include <future>
#include "json.hpp"

#include "ConsoleEngine/DzX_ConsoleEngine.h"


using json = nlohmann::json;

//Symbols that represent different objects in the game.
enum class ESymbols :char
{
	WALL = '#',
	SPAWNABLE='.',
	TREE = 'A',
	PLAYER = 'X',
	ENEMY_1 = 'H',
};

std::string ReadJson(const char InPath[], const  char elemKey[]);


