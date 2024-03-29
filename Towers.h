#pragma once
#include "Tower.h"
#include "Sprite.h"
#include <map>
using namespace std;

enum SoldierState {
	SOLDIER_IDLE,
	SOLDIER_MOVING,
	SOLDIER_FIGHTING,
	SOLDIER_RETREATING
};

struct Soldier {
	int state;
	int hp;
	VecI assemblyPoint;
	int from;
	Sprite* locator;
	int fighting =-1;
	vector<VecI> steps;
	int attackTimer;
	int atk;
	bool noProcess;
	//maintained by tower
	int id;
	int maxhp;
};

class SoldierTower : public Tower {
public:
	SoldierTower() {
		name = "Soldier Tower";
		id = "soldier";
		desc = "Send out soldiers to stop enemies";
		price = 80;
		upgradeCost = { 110,140,170 };
		radius = 250;
	}

	VecI assemblyPosition;

	int lastSoldier;
	int freeSoldier;
	void upgrade();
	void tick();
	void init();
};

class MageTower : public Tower {
public:
	MageTower() {
		name = "Mage Tower";
		id = "mage";
		desc = "Cast armor-piercing magic towards the enemy";
		price = 140;
		upgradeCost = { 200,230,260 };
		radius = 150;
	}
	
	map<int, Sprite*> sprites;
	int lastTick;
	void tick();
	void upgrade();
};

class BomberTower : public Tower {
public:
	BomberTower() {
		name = "Bomber Tower";
		id = "bomber";
		desc = "Throw bombs that deal AOE damage";
		price = 110;
		upgradeCost = { 160,200,240 };
		radius = 225;
	}

	int lastBomb;
	void upgrade();
	void tick();
	void init();
};


class ArcherTower : public Tower {
public:
	ArcherTower() {
		name = "Archer Tower";
		id = "archer";
		desc = "Shoot fast arrows towards the enemy";
		price = 90;
		upgradeCost = { 110,130,150 };
		radius = 275;
	}

	int lastArrow;
	void tick();
	void upgrade();
};
