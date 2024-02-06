#pragma once
#include "Texture.h"
#include <vector>
using namespace std;

class Actor {
public:
	int x = 0, y = 0;
	int w = 0, h = 0;
	int id = 0;
	int zIndex = 0;
	vector<Actor*> children;
	bool receiveMouseEvent = false;
	Actor* parent = NULL;
	SDL_Color color = { 255,255,255,255 };
	bool flipX = false, flipY = false;
	double rotation=0.0;
	double rotateX=0.0, rotateY=0.0;

	virtual void render(SDL_Renderer* renderer);
	
	virtual void click();

	virtual void addChild(Actor* child);

	virtual void destroy();

	int getGlobalX();
	int getGlobalY();

};