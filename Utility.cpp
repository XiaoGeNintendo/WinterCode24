#include "Utility.h"

bool isEmptyColor(SDL_Color a) {
	return a.r == 0 && a.g == 0 && a.b == 0 && a.a == 255;
}