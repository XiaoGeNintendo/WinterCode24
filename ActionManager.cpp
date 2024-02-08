#include "ActionManager.h"
void ActionManager::add(Action* a) {
	printf("Action: New Action added: %s\n", a->describe().c_str());
	actions.push_back(a);
}

void ActionManager::tick() {
	for (int i = 0; i < actions.size(); i++) {
		actions[i]->tick();
		if (actions[i]->isFinished()) {
			printf("Action: Deleted action: %s\n", actions[i]->describe().c_str());
			delete actions[i];
			actions.erase(actions.begin() + i);
			i--;
		}
	}
}

void ActionManager::close() {
	for (auto a : actions) {
		delete a;
	}
}