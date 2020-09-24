#pragma once
#include <iostream>
#include <memory>
#include <iterator>
#include <algorithm>
#include <fstream>
#include <thread>
#include <future>
#include <chrono>
#include <vector>
#include "json.hpp" //#PROBLEM - trouble with exceptions for ifstream, terminating program.
#include "pugixml.hpp"


#include "ConsoleEngine/DzX_ConsoleEngine.h"


using json = nlohmann::json;

//Symbols that represent different objects in the game.
enum class EObjectType :char
{
	NONE = ' ',
	WALL = '#',
	SPAWNABLE='.',
	TREE = 'A',
	PLAYER = 'X',
	ENEMY_1 = 'H',
	ENEMY_2 = '@',
	ARMOR = 'O',
	WEAPON= 'I',
	HPPOTION = '8',
};

std::string ReadJson(const char InPath[], const  char elemKey[]);

std::string ReadXML(const char InPath[], const  char NodeName[],const char Attribute[]);