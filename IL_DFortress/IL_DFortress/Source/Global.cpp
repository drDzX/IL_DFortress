#include "Global.h"
#include <stdio.h>
#include <iostream>


std::string ReadJson(const char InPath[], const char elemKey[])
{
	std::ifstream i(InPath);

	json J;
	i >> J;
	std::string rt = J.at(elemKey);
	return rt;
}
