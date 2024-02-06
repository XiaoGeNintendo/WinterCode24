#pragma once
#include <SDL.h>

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

//The window renderer
extern SDL_Renderer* gRenderer;

//The window we'll be rendering to
extern SDL_Window* gWindow;

//close flag
extern bool appCloseFlag;
