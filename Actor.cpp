#include "Actor.h"
#include <algorithm>
#include <cassert>
#include "Utility.h"
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

void Actor::removeFromParent() {
	if (this->parent == NULL) {
		printf("Warning: Removing NULL parent %p", this);
		return;
	}

	auto& x = this->parent->children;
	for (int i = 0; i < x.size(); i++) {
		if (x[i] == this) {
			x.erase(x.begin() + i);
			this->parent = NULL;
			return;
		}
	}

	assert(false);
}

bool Actor::processMouse(VecI mouse) {
	if (mousePolicy & MOUSE_ACCEPT) {
		if (inRect(getGlobalPosition(), size, mouse)) {
			click();
		}
	}

	if (mousePolicy & MOUSE_FALL_THROUGH) {
		for (int i = children.size() - 1; i >= 0; i--) {
			bool ans=children[i]->processMouse(mouse);
			if (ans) {
				return true;
			}
		}
		return false;
	}
	else {
		return true;
	}
}

void Actor::centerAt(VecI center)
{
	position = center - size / 2;
}

void Actor::click() {
	//do nothing
}