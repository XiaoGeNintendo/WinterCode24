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

void Actor::destroy() {
	for (auto child : children) {
		child->destroy();
		delete child;
	}
}

VecI Actor::getGlobalPosition() {
	if (parent == NULL) {
		return position;
	}
	return parent->getGlobalPosition() + position;
}

void Actor::addChild(Actor* son){

	printf("Add child: %p to %p\n", son, this);
	son->parent = this;
	children.push_back(son);
	sort(children.begin(), children.end(), compareChildren);
}

void Actor::click() {
	//do nothing
}