#include "Actor.h"
#include <algorithm>
void Actor::render(SDL_Renderer* renderer) {
	for (auto child : children) {
		child->render(renderer);
	}
}

bool compareChildren(Actor* a, Actor* b) {
	if (a->zIndex != b->zIndex) {
		return a->zIndex < b->zIndex;
	}
	return a->id < b->id;
}

void Actor::addChild(Actor* son){
	children.push_back(son);
	sort(children.begin(), children.end(), compareChildren);
}

void Actor::click() {
	//do nothing
}