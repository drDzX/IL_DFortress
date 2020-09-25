#pragma once
#include "Characters.h"
#include "Global.h"


class Enemy :public Character
{
public:
	//Constructor
	Enemy(EObjectType EnemyType=EObjectType::ENEMY_1);
	//Move to random position
	void MoveRandom(int Seed = 0);

private:
	//Drop item on death
	void DropItem();
	virtual void Die()override;
};