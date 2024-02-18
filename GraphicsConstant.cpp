#include "GraphicsConstant.h"
#include <iostream>
#include <fstream>
#include <cassert>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int FRAMERATE = 60;
const int MS_PER_FRAME = 1000 / FRAMERATE;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
TTF_Font* gFont = NULL;

bool appCloseFlag = false;

ActionManager actions = ActionManager();
SceneManager scenes = SceneManager(NULL);
AssetManager am = AssetManager();

SaveFile save = SaveFile();

void loadSave() {
	save = SaveFile();
	ifstream is("save.txt");

	if (!is.is_open()) {
		saveSave();
		is = ifstream("save.txt");
	}

	assert(is.is_open());
	for (int i = 0; i < LEVEL_COUNT; i++) {
		for (int j = 0; j < 4; j++) {
			is >> save.pass[i][j];
		}
	}
	is >> save.bgm >> save.se;

	is.close();

	printf("Loaded save file!\n");
}

void saveSave() {
	ofstream os("save.txt");
	assert(os.is_open());
	for (int i = 0; i < LEVEL_COUNT; i++) {
		for (int j = 0; j < 4; j++) {
			os << save.pass[i][j]<<" ";
		}
		os << endl;
	}

	os << save.bgm << " " << save.se << endl;
	os.close();

	printf("Saved file!\n");
}

bool passHistory(int lvl) {
	for (int i = 0; i < 4; i++) {
		if (save.pass[lvl][i]) {
			return true;
		}
	}
	return false;
}