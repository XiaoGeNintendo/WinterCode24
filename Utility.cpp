#include "Utility.h"
using namespace std;

bool isEmptyColor(SDL_Color a) {
	return a.r == 0 && a.g == 0 && a.b == 0 && a.a == 255;
}

bool inRect(VecI xy, VecI wh, VecI test){
	return test.x >= xy.x && test.x <= xy.x + wh.x && test.y >= xy.y && test.y <= xy.y + wh.y;
}

bool inRect(VecI xy, VecI wh, VecD test) {
	return test.x >= xy.x && test.x <= xy.x + wh.x && test.y >= xy.y && test.y <= xy.y + wh.y;
}

VecI getMousePosition() {
	int x, y;
	SDL_GetMouseState(&x, &y);
	return { x,y };
}

vector<string> repeat(string x, int times)
{
	vector<string> a;
	for (int i = 0; i < times; i++) {
		a.push_back(x);
	}
	return a;
}


