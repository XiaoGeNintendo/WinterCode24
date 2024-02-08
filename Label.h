#pragma once
#include "Actor.h"
#include "Texture.h"
#include <math.h>
#include <string>
using namespace std;

class Label :public Actor {
private:
	int lastFontSize = 0;
public:
	Texture* texture;
	string font;
	int fontSize;
	string text;

	Label(string font, int fontSize, string text, SDL_Color color);

	virtual void markDirty();

	virtual	void render(SDL_Renderer* renderer);
};