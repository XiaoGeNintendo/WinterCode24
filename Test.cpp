#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <string>
#include "GraphicsConstant.h"
#include "ErrorConstant.h"
#include "AssetManager.h"
#include "Stage.h"
#include "Sprite.h"
#include "Action.h"
#include "ActionManager.h"
#include "Timer.h"
#include "TitleScene.h"
#include "GameScene.h"
using namespace std;

Stage st;
SDL_Event e;

bool init() {
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		printf("Fatal: SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		exit(ERR_SDL_INIT);
	}

	//Set texture filtering to linear
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
	{
		printf("Warning: Linear texture filtering not enabled!");
	}

	//Create window
	gWindow = SDL_CreateWindow("My SDL App", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (gWindow == NULL)
	{
		printf("Fatal: Window could not be created! SDL Error: %s\n", SDL_GetError());
		exit(ERR_WINDOW_INIT);
	}

	//Create renderer for window
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
	if (gRenderer == NULL)
	{
		printf("Fatal: Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		exit(ERR_RENDERER_INIT);
	}

	//Initialize renderer color
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("Fatal: SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		exit(ERR_IMAGE_INIT);
	}
	
	if (TTF_Init() == -1) {
		printf("Fatal: SDL_TTF could not initialize! Error:%s\n", TTF_GetError());
		exit(ERR_TTF_INIT);
	}

	//Initialize SDL_mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		printf("Fatal: SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		exit(ERR_MIX_INIT);
	}
	printf("Finished init!\n");
	return true;
}

void loadGame() {
	//load game save
	loadSave();

	//load assets
	am.load("zyq", "img/son.png");
	am.load("title", "img/title.png");
	am.load("button", "img/button.png");
	am.load("map", "img/map.png");
	am.load("dialog", "img/dialog.png");
	am.load("fightBtn", "img/fightBtn.png");
	am.load("level0", "img/level0.png");
	am.load("level1", "img/level1.png");
	am.load("level2", "img/level2.png");
	am.load("difficultyMarker", "img/difficultyMarker.png");
	am.load("signpost", "img/signpost.png");
	am.load("heartBar", "img/heartbar.png");
	am.load("goldBar", "img/goldbar.png");
	am.load("transition", "img/transition.png");
	am.load("bomber", "img/bomber.png");
	am.load("bomber_b", "img/bomber_b.png");
	am.load("archer", "img/archer.png");
	am.load("archer_b", "img/archer_b.png");
	am.load("soldier", "img/soldier.png");
	am.load("soldier_b", "img/soldier_b.png");
	am.load("mage", "img/mage.png");
	am.load("mage_b", "img/mage_b.png");
	am.load("tooltip", "img/tooltip.png");
	am.load("retreat", "img/retreat.png");
	am.load("upgrade", "img/upgrade.png");
	am.load("assemble", "img/assemble.png");
	am.load("circle", "img/circle.png");
	am.load("circle2", "img/circle2.png");
	am.load("close", "img/close.png");
	am.load("arrow", "img/arrow.png");
	am.load("bomb", "img/bomb.png");
	am.load("boom", "img/boom.png");
	am.load("hpbar1", "img/hpbar1.png");
	am.load("hpbar2", "img/hpbar2.png");
	am.load("fireball", "img/fireball.png");
	am.load("fireball_arrow", "img/fireball_arrow.png");
	am.load("reinforce", "img/reinforce.png");
	am.load("mask", "img/mask.png");
	am.load("globalmask", "img/globalmask.png");
	am.load("enemymark", "img/enemymark.png");
	am.load("back", "img/back.png");
	am.load("setting", "img/setting.png");
	am.load("null", "img/null.png");
	am.load("herobg", "img/herobg.png");

	for (int i = 1; i <= 4; i++) {
		am.load("flag" + to_string(i), "img/flag" + to_string(i) + ".png");
	}
	for (int i = 1; i <= 4; i++) {
		am.load("flagb" + to_string(i), "img/flagb" + to_string(i) + ".png");
	}
	for (int i = 1; i <= 4; i++) {
		am.load("magic" + to_string(i), "img/magic" + to_string(i) + ".png");
	}
	for (int i = 1; i <= 4; i++) {
		am.load("coin" + to_string(i), "img/coin" + to_string(i) + ".png");
	}
	for (int i = 2; i <= 4; i++) {
		am.load("badge" + to_string(i), "img/badge" + to_string(i) + ".png");
	}

	am.loadEnemy("goblin", 2, 3);
	am.loadEnemy("wolf", 2, 2, true);
	am.loadEnemy("soldier", 2, 2);
	am.loadEnemy("farmer", 2, 2);
	am.loadEnemy("dk", 2, 2);
	am.loadEnemy("ghost", 4, 2);
	am.loadEnemy("ske", 2, 5);
	am.loadEnemy("momiji", 3, 3,true);
	am.loadEnemy("nitori", 3, 3, true);

	//load fonts
	am.preloadFont("global", "font/font.ttf");
	am.preloadFont("damage", "font/damage.ttf");

	//load music
	am.loadMusic("title", "mus/title.wav");
	am.loadMusic("map", "mus/map.wav");

	am.loadSE("nope", "mus/nope.wav");
	am.loadSE("build", "mus/build.wav");
	am.loadSE("buildmenu", "mus/buildmenu.wav");
	am.loadSE("open", "mus/open.wav");
	am.loadSE("close", "mus/close.wav");
	am.loadSE("paper", "mus/paper.wav");
	am.loadSE("click", "mus/click.wav");
	am.loadSE("cancel", "mus/cancel.wav");
	am.loadSE("upgrade", "mus/upgrade.wav");

	am.loadSE("cv_start", "mus/cv_start.wav");
	am.loadSE("cv_fireball", "mus/cv_fireball.wav");
	am.loadSE("cv_reinforce", "mus/cv_reinforce.wav");
	am.loadSE("cv_assemble", "mus/cv_assemble.wav");
	/* //test code here
	Sprite* logo = new Sprite(am["zyq"]);
	logo->position = VecI(50,50);
	st.addChild(logo);

	actions.add(new ActionRepeatForever(new ActionInterpolation<int>(logo->position.x, 60, 200)));
	actions.add(new ActionRepeatForever(new ActionInterpolation<Uint8>(logo->color.a, 30, 0, SINE)));
	actions.add(new ActionRepeatForever(new ActionInterpolation<double>(logo->rotation, 90, 360)));
	*/

	//load scenes
	scenes = SceneManager(&st);

	//load game content
	loadLevelInfos();

	scenes.add(new TitleScene());
	//currentLevel = 1;
	//currentHero = 1;
	//scenes.add(new GameScene()); //test only


	printf("Game Loaded!\n");
}

void close() {

	//Destroy towers
	for (int i = 0; i < TOWER_COUNT; i++) {
		delete towers[i];
	}

	//Destroy AM & stage
	st.destroy();
	am.close();
	actions.close();

	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
	TTF_Quit();

	printf("Closed!\n");
}

void eventHandling() {

	//Handle events on queue
	while (SDL_PollEvent(&e) != 0)
	{
		//User requests quit
		if (e.type == SDL_QUIT)
		{
			appCloseFlag = true;
		}
		if (e.type == SDL_MOUSEBUTTONUP) {
			//printf("Mouse click: %d %d\n", getMousePosition().x, getMousePosition().y);

			st.processMouse(getMousePosition());
		}
	}

}


int main(int argc, char* args[])
{
	Timer timer = Timer();

	init();
	loadGame();

	//While application is running
	while (!appCloseFlag)
	{
		timer.start();

		eventHandling();

		//Clear screen
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gRenderer);

		//am["zyq"]->render(gRenderer, 50, 50);

		actions.tick();
		scenes.tick();
		st.render(gRenderer);

		//Update screen
		SDL_RenderPresent(gRenderer);

		//If frame finished early
		int frameTicks = timer.getTicks();
		if (frameTicks < MS_PER_FRAME)
		{
			//Wait remaining time
			SDL_Delay(MS_PER_FRAME - frameTicks);
		}
		else {
			printf("WARNING: Can't keep up! Is the server overloaded? Ticking took %d ms.\n",frameTicks);
		}
	}
		
	//Free resources and close SDL
	close();

	return 0;
}
