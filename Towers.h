#pragma once
#include "Tower.h"
#include "Sprite.h"

enum SoldierState {
	SOLDIER_IDLE,
	SOLDIER_MOVING,
	SOLDIER_FIGHTING
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
};

class SoldierTower : public Tower {
public:
	SoldierTower() {
		name = "Soldier Tower";
		id = "soldier";
		desc = "Send out soldiers to stop enemies";
		price = 80;
		upgradeCost = { 110,140,170 };
	}

	VecI assemblyPosition;

	int lastSoldier;
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
	}

	void tick();
	void init();
};

class BomberTower : public Tower {
public:
	BomberTower() {
		name = "Bomber Tower";
		id = "bomber";
		desc = "Throw bombs that deal AOE damage";
		price = 110;
		upgradeCost = { 160,200,240 };
	}

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
	}

	void tick();
	void init();
};
