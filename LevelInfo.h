#pragma once
#include <vector>
#include <string>
#include <map>
#include "Vector2.h"
#include "Tower.h"
#include <functional>
using namespace std;

#define LEVEL_COUNT 3
#define TOWER_COUNT 4

extern int currentLevel;
extern int currentDifficulty;

struct EnemyData {
	string id;
	string name;
	string img;
	double speed;
	int attack;
	int defense;
	int attackDelay;
	int dropCoin;
	int pathId;
	int maxhp;
	//Will randomize a path between pathId, pathId+1 and pathId+2
	bool randomPath;
	//walking animation count
	int wac;
	//attacking animation count
	int aac;
};

struct WaveData {
	int delay;
	//Store ID
	vector<string> enemies;
	//Countdown of this wave starts
	int length;
};

struct LevelInfo {
	vector<string> desc;
	string background;

	vector<VecI> deployPosition;
	vector<VecI> enemyMarks;

	vector<VecI> path[16];
	map<string, EnemyData> enemies;
	vector<WaveData> waves;
	vector<pair<VecI, VecI>> obstacles;
};

extern LevelInfo levels[LEVEL_COUNT];
extern Tower* towers[TOWER_COUNT];
extern function<Tower* ()> towersCreateFunction[TOWER_COUNT];

void loadLevelInfos();