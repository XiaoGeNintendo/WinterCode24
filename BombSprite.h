#pragma once
#include "Sprite.h"

/*
A sprite that goes bezier 2 for a given time before igniting
*/
class BombSprite : public Sprite {
public:
	VecD realPosition;

	VecD start;
	VecD end;
	VecD middle;

	int igniteTime;
	int now;

	function<void()> ignite;
	bool ignited;

	BombSprite(Texture* t, VecD start, VecD end, VecD middle, int igniteTime) :Sprite(t), start(start), end(end),middle(middle), igniteTime(igniteTime), now(0), ignited(false) {}

	void render(SDL_Renderer* renderer) override {
		if (!ignited) {
			now++;
			double t = 1.0 * now / igniteTime;
			realPosition = start * (1 - t) * (1 - t) + middle * 2 * (1 - t) * t + end * t * t;

			now++;
			if (now>=igniteTime) {
				ignite();
				ignited = true;
			}
		}
		position = d2i(realPosition);
		Sprite::render(renderer);
	}
};