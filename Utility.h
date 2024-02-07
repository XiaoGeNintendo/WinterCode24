#pragma once

#include <SDL.h>
#include "Vector2.h"
/**
* Tests if the color is {0,0,0,255}
*/
bool isEmptyColor(SDL_Color color);

/**
* xy is top left corner, wh is width and height
*/
bool inRect(VecI xy, VecI wh, VecI test);