#pragma once
#include <SDL.h>

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int FRAMERATE;
extern const int MS_PER_FRAME;

//The window renderer
extern SDL_Renderer* gRenderer;

//The window we'll be rendering to
extern SDL_Window* gWindow;

//close flag
extern bool appCloseFlag;
