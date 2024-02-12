#pragma once
#include "Vector2.h"
#include "LevelInfo.h"

enum EnemyState {
	WALKING,
	PAUSING,
	FIGHTING
};
struct EnemyInstance {
	int hp;
	VecD position;
	EnemyData* data;
	int nextPoint;
	VecD speed;
	vector<VecI> path;
	int state;
	int id;
};