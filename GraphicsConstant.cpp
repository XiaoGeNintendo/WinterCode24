#include "GraphicsConstant.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int FRAMERATE = 60;
const int MS_PER_FRAME = 1000 / FRAMERATE;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

bool appCloseFlag = false;
