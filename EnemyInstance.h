#pragma once
#include "Vector2.h"
#include "LevelInfo.h"
#include "Tower.h"
#include "Towers.h"

enum EnemyState {
	ENEMY_WALKING,
	ENEMY_PAUSING,
	ENEMY_FIGHTING
};
struct EnemyInstance {
	int hp;
	int maxhp;
	VecD position;
	EnemyData* data;
	int nextPoint;
	VecD speed;
	vector<VecI> path;
	int state;
	int id;
	vector<int> fighting;
	int attackTimer;
	bool noProcess;

	//effects
	int slowdownTimer;
};