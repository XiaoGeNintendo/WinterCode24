#pragma once
#include "Actor.h"
#include "Texture.h"
#include "Utility.h"
#include <math.h>

class Sprite :public Actor {
private:
	function<void()> fClick;
public:
	Texture* texture;

	Sprite(Texture* texture) : texture(texture) {
		size = texture->size();

		rotationCenter = size / 2;
	};

	void setClick(function<void()> fClick) {
		this->fClick = fClick;
		mousePolicy = MOUSE_ACCEPT;
	}

	void click() override {
		fClick();
	}

	virtual	void render(SDL_Renderer* renderer) {
		Actor::render(renderer);
		
		if (!isEmptyColor(color)) {
			SDL_SetTextureColorMod(texture->texture, color.r, color.g, color.b);
			SDL_SetTextureAlphaMod(texture->texture, color.a);
		}

		if (abs(rotation) <= 1e-6 && !flipX && !flipY) {
			texture->render(renderer, getRenderPosition(), size);
		}
		else {
			texture->render(renderer, getRenderPosition(), size , rotation, rotationCenter, flipX, flipY);
		}

		if (!isEmptyColor(color)) {
			SDL_SetTextureColorMod(texture->texture, 255, 255, 255);
			SDL_SetTextureAlphaMod(texture->texture, 255);
		}
	}
};