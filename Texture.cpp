#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include "Texture.h"
#include <cassert>
using namespace std;

int Texture::render(SDL_Renderer* renderer, int x, int y) {
	return render(renderer, x, y, this->w, this->h);
}

int Texture::render(SDL_Renderer* renderer, int x, int y, int w, int h) {
	SDL_Rect rect = { x,y,w,h };
	return SDL_RenderCopy(renderer, texture, NULL, &rect);
}

int Texture::render(SDL_Renderer* renderer, int x, int y, int w, int h, double rotate, double centerX, double centerY, bool flipX, bool flipY) {
	SDL_Rect rect = { x,y,w,h };
	SDL_Point point = { centerX, centerY };
	SDL_RendererFlip flip=SDL_FLIP_NONE;

	assert(!(flipX && flipY));

	if (flipX) {
		flip = SDL_FLIP_HORIZONTAL;
	} 
	
	if(flipY){
		flip = SDL_FLIP_VERTICAL;
	}

	return SDL_RenderCopyEx(renderer, texture, NULL, &rect, rotate, &point, flip);
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