#pragma once
#include "Sprite.h"
#include "Random.h"
class ParticleSprite :public Sprite {
public:
	VecD realPosition;
	VecD speed;

	ParticleSprite(Texture* texture) : Sprite(texture) {
		speed = VecD(randDouble()*3 - 1.5, -2);
	}

	ParticleSprite(vector<Texture*> animation, int delay) : Sprite(animation,delay) {
		speed = VecD(randDouble()*3 - 1.5, -2);
	}

	void render(SDL_Renderer* renderer) override {
		realPosition = realPosition + speed;
		speed = speed + VecD(0, 5) / 60;
		position = d2i(realPosition);

		Sprite::render(renderer);
	}
};