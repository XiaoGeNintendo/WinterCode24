#include "LevelInfo.h"

int currentLevel = 0;
int currentDifficulty = 0;

LevelInfo levels[LEVEL_COUNT];

void loadLevelInfos() {
	//load level 1
	levels[0].desc = { "Enemies are attacking the kingdom in all directions!","Use hero and towers to protect yourself!","Time to get familiar with the towers!!" };
	levels[0].background = "level0";
}