#pragma once
#include "Texture.h"
#include <vector>
#include "Vector2.h"
using namespace std;

const int MOUSE_FALL_THROUGH = 1;
const int MOUSE_ACCEPT = 2;

class Actor {
public:

	VecI position, size;

	int id = 0;
	int zIndex = 0;
	vector<Actor*> children;

	/**
	Mask of MOUSE_FALL_THROUGH and MOUSE_ACCEPT
	*/
	int mousePolicy = MOUSE_FALL_THROUGH;
	Actor* parent = NULL;
	SDL_Color color = { 255,255,255,255 };
	bool flipX = false, flipY = false;
	double rotation=0.0;

	VecI rotationCenter;

	virtual void render(SDL_Renderer* renderer);
	
	virtual void click();

	virtual void addChild(Actor* child);

	virtual void destroy();

	virtual ~Actor() = default;

	VecI getGlobalPosition();

	/**
	This does NOT delete this instance
	*/
	void removeFromParent();

	/**
		Return value is whether rejected
	*/
	bool processMouse(VecI mouse);
};