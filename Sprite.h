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
	int frameCounter = 0;
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

	void click() override {
		fClick();
	}

	virtual	void render(SDL_Renderer* renderer) {
		
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