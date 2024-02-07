#pragma once
#include "Texture.h"
#include <vector>
#include "Vector2.h"
using namespace std;

class Actor {
public:

	VecI position, size;

	int id = 0;
	int zIndex = 0;
	vector<Actor*> children;
	bool receiveMouseEvent = false;
	Actor* parent = NULL;
	SDL_Color color = { 255,255,255,255 };
	bool flipX = false, flipY = false;
	double rotation=0.0;

	VecI rotationCenter;

	virtual void render(SDL_Renderer* renderer);
	
	virtual void click();

	virtual void addChild(Actor* child);

	virtual void destroy();

	VecI getGlobalPosition();

};