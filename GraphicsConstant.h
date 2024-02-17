#pragma once
#include <SDL.h>
#include "Action.h"
#include "ActionManager.h"
#include "Scene.h"
#include "AssetManager.h"
#include <SDL_ttf.h>

#define PI 3.141592653589
#define ASSEMBLY_RANGE 150
#define SOLDIER_HP 100
#define BOMB_RADIUS 85
#define SOLDIER_WANDER_RANGE 50
#define FIREBALL_TIME 600
#define REINFORCE_TIME 600
#define FIREBALL_RADIUS 200

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int FRAMERATE;
extern const int MS_PER_FRAME;

enum Difficulty {
	EASY,
	NORMAL,
	HARD,
	LUNATIC
};

//The window renderer
extern SDL_Renderer* gRenderer;

//The window we'll be rendering to
extern SDL_Window* gWindow;

extern TTF_Font* gFont;

//close flag
extern bool appCloseFlag;

extern ActionManager actions;
extern SceneManager scenes;
extern AssetManager am;
