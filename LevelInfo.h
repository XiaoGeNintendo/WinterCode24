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
	string id="";
	string name="";
	double speed=0;
	int attack=0;
	int defense=0;
	int attackDelay=0;
	int dropCoin=0;
	int pathId=0;
	int maxhp=0;
	//Will randomize a path between pathId, pathId+1 and pathId+2
	bool randomPath=false;
	//walking animation count
	int wac=2;
	//attacking animation count
	int aac=2;
	bool thumbnail=false;

	EnemyData() {}

	EnemyData(string _id, string _name, 
		double _speed, int _attack, int _defense, int _attackDelay,
		int _dropCoin, int _pathId, int _maxhp, bool _randomPath,
		int _wac, int _aac, bool _thumbnail)
		: id(_id), name(_name), speed(_speed), attack(_attack),
		defense(_defense), attackDelay(_attackDelay), dropCoin(_dropCoin),
		pathId(_pathId), maxhp(_maxhp), randomPath(_randomPath),
		wac(_wac), aac(_aac), thumbnail(_thumbnail) {
	}

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

string difficultyString(int a);
int difficultySelect(int e, int n, int h, int l);
