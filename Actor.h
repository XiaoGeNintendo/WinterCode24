#pragma once
#include "Texture.h"
#include <vector>
using namespace std;

class Actor {
public:
	int x, y;
	int w, h;
	int id;
	int zIndex;
	vector<Actor*> children;
	bool receiveMouseEvent;

	virtual void render(SDL_Renderer* renderer);
	
	virtual void click();

	virtual void addChild(Actor* child);
};