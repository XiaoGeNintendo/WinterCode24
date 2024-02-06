#pragma once
#include <SDL.h>
#include <SDL_image.h>

class Texture {

	public:
		SDL_Texture* texture;
		int h, w;

		int render(SDL_Renderer* renderer, int x, int y);
		int render(SDL_Renderer* renderer, int x, int y, int w, int h);
		void close();
};