#pragma once
#include "Actor.h"
#include "Texture.h"
#include "Utility.h"
#include <math.h>
#include <vector>
#include <string>
using namespace std;

class Sprite :public Actor {
private:
	function<void()> fClick;
	function<void()> fMouseEnter;
	function<void()> fMouseExit;

	bool doHover = false;
	int frameCounter = 0;
	bool lastIn = false;

public:
	Texture* texture;

	vector<Texture*> animation;
	int animationCounter = 0;
	int animationDelay = -1;

	Sprite(Texture* texture) : texture(texture) {
		size = texture->size();

		rotationCenter = size / 2;
	};

	Sprite(vector<Texture*> animation, int animationDelay) : animation(animation), texture(animation[0]),  animationDelay(animationDelay) {
		size = texture->size();
		rotationCenter = size / 2;
	}

	void setClick(function<void()> fClick) {
		this->fClick = fClick;
		mousePolicy = MOUSE_ACCEPT;
	}

	void setHover(function<void()> fMouseEnter,function<void()> fMouseExit) {
		this->fMouseEnter = fMouseEnter;
		this->fMouseExit = fMouseExit;
		doHover = true;
	}

	void click() override {
		fClick();
	}

	virtual	void render(SDL_Renderer* renderer) {
		
		if (doHover) {
			auto m = getMousePosition();
			auto r = getRenderPosition();
			bool thisIn = (m.x >= r.x && m.x <= r.x + size.x && m.y >= r.y && m.y <= r.y + size.y);
			if (!lastIn && thisIn) {
				fMouseEnter();
			}
			if (lastIn && !thisIn) {
				fMouseExit();
			}
			lastIn = thisIn;
		}

		//animation
		if (animationDelay > 0) {
			//is animation
			frameCounter++;
			if (frameCounter == animationDelay) {
				frameCounter = 0;
				animationCounter++;
				if (animationCounter == animation.size()) {
					animationCounter = 0;
				}

				texture = animation[animationCounter];
			}
		}

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

		Actor::render(renderer);
	}
};