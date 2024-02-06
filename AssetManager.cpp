#include "AssetManager.h"
#include "GraphicsConstant.h"
#include "ErrorConstant.h"
#include <cassert>

void AssetManager::load(string name, string path) {
	Texture* t=new Texture();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Fatal: Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
		exit(ERR_LOAD_IMAGE);
	}
	else
	{
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Fatal: Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
			exit(ERR_PARSE_TEXTURE);
		}
		else
		{
			//Get image dimensions
			t->w = loadedSurface->w;
			t->h = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//Return success
	t->texture = newTexture;
	assert(newTexture != NULL);

	//Add to map
	printf("SUCCESS: Loaded asset with name %s.\n", name.c_str());
	loadedTexture[name] = t;
}

void AssetManager::close() {
	for (auto x : loadedTexture) {
		printf("Closing texture %s\n", x.first.c_str());
		x.second->close();
		delete x.second;
	}
	loadedTexture.clear();
}

Texture* AssetManager::operator[](string name) {
	return loadedTexture[name];
}