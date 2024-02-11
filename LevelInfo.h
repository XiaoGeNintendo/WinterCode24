#pragma once
#include <vector>
#include <string>
#include <map>
#include "Vector2.h"
using namespace std;

#define LEVEL_COUNT 3
extern int currentLevel;
extern int currentDifficulty;

struct EnemyData {
	string id;
	string name;
	string img;
	int speed;
	int attack;
	int defense;
	int pathId;
};
struct LevelInfo {
	vector<string> desc;
	string background;

	vector<VecI> deployPosition;
	vector<int> path[16];
	map<string, EnemyData> enemies;
	vector<vector<string>> waves;
};

extern LevelInfo levels[LEVEL_COUNT];

void loadLevelInfos();