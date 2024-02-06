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

int Actor::getGlobalX() {
	if (parent == NULL) {
		return x;
	}
	return parent->getGlobalX() + x;
}

int Actor::getGlobalY() {
	if (parent == NULL) {
		return y;
	}
	return parent->getGlobalY() + y;
}

void Actor::addChild(Actor* son){

	printf("Add child: %p to %p\n", son, this);
	printf("Next: %p info= %d %d %d %d\n", son, son->x, son->y, son->w, son->h);
	son->parent = this;
	children.push_back(son);
	sort(children.begin(), children.end(), compareChildren);
}

void Actor::click() {
	//do nothing
}