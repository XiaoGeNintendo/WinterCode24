#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include "GraphicsConstant.h"
#include "ErrorConstant.h"
#include "AssetManager.h"
#include "Stage.h"
#include "Sprite.h"
#include "Action.h"
#include "ActionManager.h"
using namespace std;

AssetManager am;
Stage st;
SDL_Event e;
ActionManager actions;

bool init() {
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
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
	
	printf("Finished init!\n");
	return true;
}

void loadGame() {
	am.load("zyq", "img/son.png");
	printf("%p",am["zyq"]);
	Sprite* logo = new Sprite(am["zyq"]);
	logo->x = 50;
	logo->y = 50;
	st.addChild(logo);

	actions.add(new ActionRepeatForever(new ActionInterpolation<int>(logo->x, 60, 200)));

	printf("Game Loaded!\n");
}

void close() {
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
	IMG_Quit();
	SDL_Quit();

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
	}

}


int main(int argc, char* args[])
{
	init();
	loadGame();

	//While application is running
	while (!appCloseFlag)
	{
		eventHandling();

		//Clear screen
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gRenderer);

		//am["zyq"]->render(gRenderer, 50, 50);
		
		st.render(gRenderer);
		actions.tick();

		// 
		//Update screen
		SDL_RenderPresent(gRenderer);
	}
		
	//Free resources and close SDL
	close();

	return 0;
}
