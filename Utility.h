#pragma once

#include <SDL.h>
#include "Vector2.h"
#include <vector>
#include <string>
using namespace std;

/**
* Tests if the color is {0,0,0,255}
*/
bool isEmptyColor(SDL_Color color);

/**
* xy is top left corner, wh is width and height
*/
bool inRect(VecI xy, VecI wh, VecI test);

/**
* xy is top left corner, wh is width and height
*/
bool inRect(VecI xy, VecI wh, VecD test);
VecI getMousePosition();

vector<string> repeat(string x, int times);
vector<string> repeatv(vector<string> x, int times);
vector<string> operator+(vector<string> a, vector<string> b);