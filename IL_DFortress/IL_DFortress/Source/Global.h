#pragma once

#include <stdio.h>

#include <windows.h>
#include <conio.h>

#include <thread>
#include <future>
#include <memory>

#include <iterator>
#include <algorithm>

#include <fstream>
#include <iostream>

#include <vector>
#include <chrono>

#include "json.hpp" //#PROBLEM - trouble with exceptions for ifstream, terminating program.
#include "pugixml.hpp"

#include "ConsoleEngine/DzX_ConsoleEngine.h"

#include "Profiling/Profiler.h"

//To switch between starting game or main menu first
#define PR_DEBUG 0

#define PROFILING 0
#if PROFILING
#define PROFILING_SCOPE(name) Timer timer##__LINE__(name)
#define PROFILING_FUNCTION() PROFILING_SCOPE(__FUNCTION__)
#else
#define PROFILING_SCOPE(name)
#define PROFILING_FUNCTION()
#endif


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

//#PROBLEM - not in use. Terminates console by calling NOEXCEPTION for ifstream.
std::string ReadJson(const char InPath[], const  char elemKey[]);

//Read XML file from default node list
std::string ReadXML(const char InPath[], const  char NodeName[],const char Attribute[]);
//Read XML file with exact node / child list
std::string ReadXML(const char InPath[], const  char NodeName[], const  char NodeChildren[], const char Attribute[]);
/*Explode string to rows
* @s - string to parse.
* @c - char to split string.
* */
const std::vector<std::string> ExplodeString(const std::string& s, const char& c);
