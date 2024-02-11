#include "LevelInfo.h"
#include "Towers.h"
int currentLevel = 0;
int currentDifficulty = 0;

LevelInfo levels[LEVEL_COUNT];
Tower* towers[TOWER_COUNT];

void loadLevelInfos() {

	towers[0] = new SoldierTower();
	towers[1] = new ArcherTower();
	towers[2] = new BomberTower();
	towers[3] = new MageTower();

	//load level 1
	levels[0].desc = { "Enemies are attacking the kingdom in all directions!","Use hero and towers to protect yourself!","Time to get familiar with the towers!!" };
	levels[0].background = "level0";
	levels[0].deployPosition = { {272,363},{164,417},{73,570},{84,664},{104,778},{149,867},{1296,657},{1128,651},{964,658},{821,656},{694,653},{515,617},{621,485},{679,338}, };

}