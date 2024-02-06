#pragma once
#include "Actor.h"
#include "Texture.h"

class Sprite :public Actor {
public:
	Texture* texture;

	Sprite(Texture* texture) : texture(texture) {
		w = texture->w;
		h = texture->h;
	};

	virtual	void render(SDL_Renderer* renderer) {
		Actor::render(renderer);
		texture->render(renderer, getGlobalX(), getGlobalY(), w, h);
	}
};