#pragma once
#include <string>
#include <vector>
#include "Vector2.h"
using namespace std;

class Tower {
public:
	string name;
	string id;
	string desc;
	int price;
	vector<int> upgradeCost;

	int level;
	void* scene;
	VecI position;
	int at;
	int radius;
	virtual void tick() = 0;
	virtual void init() {};
	virtual void upgrade() {};
};