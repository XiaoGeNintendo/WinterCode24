#pragma once
#include "Actor.h"
#include "Texture.h"
#include "Utility.h"
#include <math.h>
#include "GraphicsConstant.h"
#include <string>
#include "Label.h"
#include <functional>

using namespace std;

class LabelButton :public Label {
private:
	bool hit = false;
	int largeSize = 0;
	int lastSize = 0;

public:

	bool showBg = true;

	LabelButton(string font, int fontSize, string text, SDL_Color color) :Label(font, fontSize, text, color) {
		mousePolicy = MOUSE_ACCEPT;
		largeSize = fontSize + 3;
	}

	function<void()> fClick;

	void click() override {
		am.playSE("click");
		fClick();
	}

	void render(SDL_Renderer* renderer) override {

		if (color.a == 0) {
			return;
		}

		if (lastSize != fontSize) {
			markDirty(); //regenerate string
			lastSize = fontSize;
		}

		const auto MAGIC = VecI(20,0);

		
		bool newhit = inRect(getRenderPosition(), size, getMousePosition());
		if (!hit && newhit) {
			actions.add(new ActionInterpolation<int>(fontSize,15, fontSize, largeSize, SINE));
		}
		if (hit && !newhit) {
			actions.add(new ActionInterpolation<int>(fontSize, 15, fontSize, largeSize-3, SINE));
		}
		hit = newhit;

		if (showBg) {
			//render background
			if (color.a != 255) {
				SDL_SetTextureBlendMode(am["button"]->texture, SDL_BLENDMODE_BLEND);
				SDL_SetTextureAlphaMod(am["button"]->texture, color.a);
			}

			if (abs(rotation) <= 1e-6 && !flipX && !flipY) {
				am["button"]->render(renderer, getRenderPosition() - MAGIC, size + MAGIC * 2);
			}
			else {
				am["button"]->render(renderer, getRenderPosition() - MAGIC, size + MAGIC * 2, rotation, rotationCenter, flipX, flipY);
			}

			if (color.a != 255) {
				SDL_SetTextureAlphaMod(am["button"]->texture, 255);
			}
		}

		//render label
		Label::render(renderer);
	}
};