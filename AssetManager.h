#pragma once
#include "Texture.h"
#include <map>
#include <string>
#include <SDL_ttf.h>
#include <vector>
#include <SDL_mixer.h>

using namespace std;

class AssetManager {
public:
	map<string, Texture*> loadedTexture;
	map<string, TTF_Font*> loadedFont;
	map<string, string> fontLookup;

	map<string, Mix_Music*> music;
	map<string, Mix_Chunk*> se;

	void preloadFont(string name, string path);
	Texture* generateString(string font, int size, string text, SDL_Color color);

	void loadSE(string name, string path);
	void loadMusic(string name, string path);

	Mix_Music* m(string name);
	Mix_Chunk* s(string name);

	void playSE(string name);
	void playMus(string name);

	void stopMus();

	void load(string name, string path);
	void loadEnemy(string prefix, int wac, int aac, bool thumbnail=false);
	void close();
	vector<Texture*> animation(string prefix, int start, int end);

	Texture* operator[](string name);
};