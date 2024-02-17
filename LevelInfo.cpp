#include "LevelInfo.h"
#include "Towers.h"
#include "Utility.h"
#include <string>
using namespace std;

int currentLevel = 0;
int currentDifficulty = 0;

LevelInfo levels[LEVEL_COUNT];
//sample towers
Tower* towers[TOWER_COUNT];
function<Tower* ()> towersCreateFunction[TOWER_COUNT];

void loadLevelInfos() {

	towers[0] = new SoldierTower();
	towersCreateFunction[0] = []() {return new SoldierTower(); };
	towers[1] = new ArcherTower();
	towersCreateFunction[1] = []() {return new ArcherTower(); };
	towers[2] = new BomberTower();
	towersCreateFunction[2] = []() {return new BomberTower(); };
	towers[3] = new MageTower();
	towersCreateFunction[3] = []() {return new MageTower(); };

	//load enemy
	auto goblin = EnemyData();
	goblin.aac = 3;
	goblin.wac = 2;
	goblin.attack = 10;
	goblin.defense = 0;
	goblin.dropCoin = 30;
	goblin.attackDelay = 60;
	goblin.id = "goblin";
	goblin.maxhp = 100;
	goblin.name = "Goblin";
	goblin.pathId = 0;
	goblin.randomPath = true;
	goblin.speed = 1;

	//load level 1
	levels[0].desc = { "Enemies are attacking the kingdom in all directions!","Use hero and towers to protect yourself!","Time to get familiar with the towers!!" };
	levels[0].background = "level0";
	levels[0].deployPosition = { {272,363},{164,417},{73,570},{84,664},{104,778},{149,867},{1296,657},{1128,651},{964,658},{821,656},{694,653},{515,617},{621,485},{679,338}, };
	levels[0].waves.resize(13);
	levels[0].waves[0].length = 60;
	levels[0].waves[0].enemies = repeat(string("gb"),100);
	levels[0].waves[0].delay = 20;

	levels[0].path[0] = {{429,12},{419,351},{331,441},{254,490},{172,576},{166,720},{207,812},{314,868},{467,885},{1435,895}, };
	levels[0].path[1] = {{514,13},{513,370},{290,572},{281,719},{533,811},{1383,807}, };
	levels[0].path[2] = { {601,14},{594,400},{403,547},{399,689},{511,762},{1435,742}, };
	levels[0].enemies["gb"] = goblin;
	levels[0].obstacles = { {{0,0},{423,291}},{{618,118},{1435,272}},{{1028,611},{1412,241}} };

}