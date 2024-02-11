#pragma once
#include <vector>
#include <string>
#include <map>
#include "Vector2.h"
#include "Tower.h"
using namespace std;

#define LEVEL_COUNT 3
#define TOWER_COUNT 4

extern int currentLevel;
extern int currentDifficulty;

struct EnemyData {
	string id;
	string name;
	string img;
	int speed;
	int attack;
	int defense;
	int dropCoin;
	int pathId;
};

struct WaveData {
	int delay;
	//Store ID
	vector<string> enemies;
	//Total length before next wave enters the map
	int length;
};

struct LevelInfo {
	vector<string> desc;
	string background;

	vector<VecI> deployPosition;
	vector<int> path[16];
	map<string, EnemyData> enemies;
	vector<WaveData> waves;
};

extern LevelInfo levels[LEVEL_COUNT];
extern Tower* towers[TOWER_COUNT];

void loadLevelInfos();