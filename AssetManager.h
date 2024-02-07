#pragma once
#include "Texture.h"
#include <map>
#include <string>
#include <SDL_ttf.h>

using namespace std;

class AssetManager {
public:
	map<string, Texture*> loadedTexture;
	map<string, TTF_Font*> loadedFont;
	map<string, string> fontLookup;

	void preloadFont(string name, string path);
	Texture* generateString(string font, int size, string text, SDL_Color color);

	void load(string name, string path);
	void close();

	Texture* operator[](string name);
};