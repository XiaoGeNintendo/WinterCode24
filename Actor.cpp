#include "Actor.h"
#include <algorithm>
#include <cassert>
#include "Utility.h"
void Actor::render(SDL_Renderer* renderer) {

	for (int i = 0; i < children.size(); i++) {
		auto child = children[i];
		//printf("%p %d\n", child,i);
		if (child->visible) {
			child->render(renderer);
		}
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

VecI Actor::getRenderPosition()
{
	return getGlobalPosition() - VecI(pivot.x * size.x, pivot.y * size.y);
}

void Actor::addChild(Actor* son){

	//printf("Add child: %p to %p\n", son, this);
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
	//printf("Processing mouse %p\n", this);
	if (!visible) {
		return false;
	}

	if (mousePolicy & MOUSE_FALL_THROUGH) {

		for(int i=children.size()-1;i>=0;i--){
			auto son = children[i];
			bool hit=son->processMouse(mouse);
			if (hit) {
				return true;
			}
		}
	}

	if (mousePolicy & MOUSE_ACCEPT) {
		if (inRect(getRenderPosition(), size, getMousePosition())) {
			click();
			return true;
		}
	}

	return false;
}

void Actor::centerAt(VecI center)
{
	position = center - size / 2;
}

void Actor::click() {
	//do nothing
}
