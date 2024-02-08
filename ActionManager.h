#pragma once
#include "Action.h"

struct ActionManager {
	vector<Action*> actions;
	void add(Action* a);
	void tick();
	void close();
};