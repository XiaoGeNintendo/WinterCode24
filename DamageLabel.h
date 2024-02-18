#pragma once
#include "Label.h"
#include "Random.h"
class DamageLabel :public Label {
public:
	VecD realPosition;
	VecD speed;

	DamageLabel(string text) :Label("damage", 12, text, { 255,0,0,180 }) {
		speed = VecD(randDouble(), -2);
	}

	DamageLabel(string text, SDL_Color c) :Label("damage", 12, text, c) {
		speed = VecD(randDouble(), -2);
	}

	void render(SDL_Renderer* renderer) override {
		realPosition = realPosition + speed;
		speed = speed + VecD(0, 5)/60;
		position = d2i(realPosition);

		Label::render(renderer);
	}
};