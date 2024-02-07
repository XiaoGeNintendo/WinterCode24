#pragma once
#include "Actor.h"
#include "Texture.h"
#include "Utility.h"
#include <math.h>
#include "GraphicsConstant.h"
#include <string>
using namespace std;

class Label :public Actor {
public:
	Texture* texture;
	string font;
	int fontSize;
	string text;

	Label(string font, int fontSize, string text, SDL_Color color) : font(font),fontSize(fontSize),text(text) {
		this->color = color;
		markDirty();
	};

	void markDirty() {
		texture = am.generateString(font, fontSize, text, color);
		size = texture->size();
	}

	virtual	void render(SDL_Renderer* renderer) {
		Actor::render(renderer);
		
		if (color.a!=255) {
			SDL_SetTextureBlendMode(texture->texture, SDL_BLENDMODE_BLEND);
			SDL_SetTextureAlphaMod(texture->texture, color.a);
		}

		if (abs(rotation) <= 1e-6 && !flipX && !flipY) {
			texture->render(renderer, getGlobalPosition(), size);
		}
		else {
			texture->render(renderer, getGlobalPosition(), size , rotation, rotationCenter, flipX, flipY);
		}

		if (color.a!=255) {
			SDL_SetTextureAlphaMod(texture->texture, 255);
		}
	}
};