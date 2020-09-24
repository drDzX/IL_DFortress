#pragma once
#include "Characters.h"
#include "Global.h"


class Enemy :public Character
{
public:
	Enemy(EObjectType EnemyType=EObjectType::ENEMY_1);

	void MoveRandom(int Seed = 0);

	void DropItem();

	void OverlappEvent();

	virtual void Die()override;

};