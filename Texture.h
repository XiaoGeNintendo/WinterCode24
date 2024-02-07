#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "Vector2.h"

class Texture {

	public:
		SDL_Texture* texture;
		int h, w;

		int render(SDL_Renderer* renderer, int x, int y);
		int render(SDL_Renderer* renderer, int x, int y, int w, int h); 
		int render(SDL_Renderer* renderer, int x, int y, int w, int h, double rotate, int centerX, int centerY, bool flipX, bool flipY);

		int render(SDL_Renderer* renderer, VecI xy);
		int render(SDL_Renderer* renderer, VecI xy, VecI wh);
		int render(SDL_Renderer* renderer, VecI xy, VecI wh, double rotate, VecI rotationCenter, bool flipX, bool flipY);

		void close();
		VecI size();
};