#include "LevelInfo.h"
#include "Towers.h"
#include "Utility.h"
#include <string>
#include <cassert>
using namespace std;

int currentLevel = 0;
int currentDifficulty = 0;

LevelInfo levels[LEVEL_COUNT];
//sample towers
Tower* towers[TOWER_COUNT];
function<Tower* ()> towersCreateFunction[TOWER_COUNT];

string difficultyString(int a) {
	switch (a) {
	case 0:
		return "Easy";
	case 1:
		return "Normal";
	case 2:
		return "Hard";
	case 3:
		return "Lunatic";
	default:
		assert(false);
	}
}

int difficultySelect(int e, int n, int h, int l)
{
	switch (currentDifficulty) {
	case 0:
		return e;
	case 1:
		return n;
	case 2:
		return h;
	case 3:
		return l;
	default:
		assert(false);
	}
}


void loadLevelInfos() {

	towers[0] = new SoldierTower();
	towersCreateFunction[0] = []() {return new SoldierTower(); };
	towers[1] = new ArcherTower();
	towersCreateFunction[1] = []() {return new ArcherTower(); };
	towers[2] = new BomberTower();
	towersCreateFunction[2] = []() {return new BomberTower(); };
	towers[3] = new MageTower();
	towersCreateFunction[3] = []() {return new MageTower(); };

	//load level 1
	auto goblin = EnemyData("goblin", "Goblin", 1, 10, 0, 60, 30, 0, 100, true, 2, 3, false);
	auto wolf = EnemyData("wolf", "Wolf", 2, 8, 2, 40, 30, 0, 80, true, 2, 2, true);
	levels[0].enemies["gb"] = goblin;
	levels[0].enemies["wf"] = wolf;
	levels[0].desc = { "Enemies are attacking the kingdom in all directions!","Use hero and towers to protect yourself!","Time to get familiar with the towers!!" };
	levels[0].background = "level0";
	levels[0].deployPosition = { {272,363},{164,417},{73,570},{84,664},{104,778},{149,867},{1296,657},{1128,651},{964,658},{821,656},{694,653},{515,617},{621,485},{679,338}, };
	levels[0].waves.resize(6);
	for (int i = 0; i < 3; i++) {
		levels[0].waves[i].length = 60*30;
		levels[0].waves[i].enemies = repeat(string("gb"), (i+1)*5);
		levels[0].waves[i].delay = 60-20*i;
	}
	levels[0].waves[3].length = 60 * 30;
	levels[0].waves[3].enemies = repeat("wf", 5);
	levels[0].waves[3].delay = 60;
	levels[0].waves[4].length = 60 * 30;
	levels[0].waves[4].enemies = repeat("gb",10)+repeat("wf", 5);
	levels[0].waves[4].delay = 30;
	levels[0].waves[5].length = 60 * 30;
	levels[0].waves[5].enemies = repeatv({ "gb","wf" }, 15);
	levels[0].waves[5].delay = 20;

	levels[0].enemyMarks = { {515,42} };
	levels[0].path[0] = {{429,12},{419,351},{331,441},{254,490},{172,576},{166,720},{207,812},{314,868},{467,885},{1435,895}, };
	levels[0].path[1] = {{514,13},{513,370},{290,572},{281,719},{533,811},{1383,807}, };
	levels[0].path[2] = { {601,14},{594,400},{403,547},{399,689},{511,762},{1435,742}, };
	levels[0].obstacles = { {{0,0},{423,291}},{{618,118},{1435,272}},{{1028,611},{1412,241}} };

}