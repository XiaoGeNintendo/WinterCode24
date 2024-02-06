#pragma once
#include "Actor.h"
#include "Texture.h"
#include "Utility.h"
#include <math.h>

class Sprite :public Actor {
public:
	Texture* texture;

	Sprite(Texture* texture) : texture(texture) {
		w = texture->w;
		h = texture->h;
		rotateX = w / 2.0;
		rotateY = h / 2.0;
	};

	virtual	void render(SDL_Renderer* renderer) {
		Actor::render(renderer);
		
		if (!isEmptyColor(color)) {
			SDL_SetTextureBlendMode(texture->texture, SDL_BLENDMODE_BLEND);
			SDL_SetTextureColorMod(texture->texture, color.r, color.g, color.b);
			SDL_SetTextureAlphaMod(texture->texture, color.a);
		}

		if (abs(rotation) <= 1e-6 && !flipX && !flipY) {
			texture->render(renderer, getGlobalX(), getGlobalY(), w, h);
		}
		else {
			texture->render(renderer, getGlobalX(), getGlobalY(), w, h, rotation, rotateX, rotateY, flipX, flipY);
		}

		if (!isEmptyColor(color)) {
			SDL_SetTextureBlendMode(texture->texture, SDL_BLENDMODE_NONE);
			SDL_SetTextureColorMod(texture->texture, 255, 255, 255);
			SDL_SetTextureAlphaMod(texture->texture, 255);
		}
	}
};