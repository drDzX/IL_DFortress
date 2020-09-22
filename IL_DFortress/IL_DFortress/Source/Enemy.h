#pragma once
#include "Characters.h"
#include "Global.h"


class Enemy :public Character
{
public:
	Enemy();

	void MoveRandom(int Seed = 0);

};