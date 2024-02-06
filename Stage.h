#pragma once
#include "Actor.h"

class Stage :public Actor {
public:
	int globalSonId = 0;

	void addChild(Actor* actor) {
		actor->id = globalSonId;
		globalSonId++;

		Actor::addChild(actor);
	}
};