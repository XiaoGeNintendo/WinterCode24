#pragma once
#include <string>
#include <vector>
using namespace std;

class Tower {
public:
	string name;
	string id;
	string desc;
	int price;
	vector<int> upgradeCost;

	virtual void tick() = 0;
};