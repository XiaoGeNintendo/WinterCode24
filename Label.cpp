#include "Label.h"
#include "GraphicsConstant.h"
#include "Utility.h"

Label::Label(string font, int fontSize, string text, SDL_Color color): font(font), fontSize(fontSize), text(text) {
		this->color = color;
		markDirty();
}

void Label::markDirty() {
	texture = am.generateString(font, fontSize, text, color);
	size = texture->size();
	lastFontSize = fontSize;
}

void Label::render(SDL_Renderer* renderer) {

	if (fontSize != lastFontSize) {
		markDirty();
	}

	if (color.a != 255) {
		SDL_SetTextureBlendMode(texture->texture, SDL_BLENDMODE_BLEND);
		SDL_SetTextureAlphaMod(texture->texture, color.a);
	}

	if (abs(rotation) <= 1e-6 && !flipX && !flipY) {
		texture->render(renderer, getRenderPosition(), size);
	}
	else {
		texture->render(renderer, getRenderPosition(), size, rotation, rotationCenter, flipX, flipY);
	}

	if (color.a != 255) {
		SDL_SetTextureAlphaMod(texture->texture, 255);
	}

	Actor::render(renderer);
}
