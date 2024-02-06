#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include "Texture.h"
using namespace std;

int Texture::render(SDL_Renderer* renderer, int x, int y) {
	return render(renderer, x, y, this->w, this->h);
}

int Texture::render(SDL_Renderer* renderer, int x, int y, int w, int h) {
	SDL_Rect rect = { x,y,w,h };
	return SDL_RenderCopy(renderer, texture, NULL, &rect);
}

void Texture::close() {
	//Free texture if it exists
	if (texture != NULL)
	{
		SDL_DestroyTexture(texture);
		texture = NULL;
		w = h = 0;
	}
}