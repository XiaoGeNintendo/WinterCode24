#include "AssetManager.h"
#include "GraphicsConstant.h"
#include "ErrorConstant.h"
#include <cassert>

void AssetManager::preloadFont(string name, string path){
	fontLookup[name] = path;
	printf("Preloaded font:%s --> %s\n", name.c_str(), path.c_str());
}

Texture* AssetManager::generateString(string font, int size, string text, SDL_Color color)
{
	string textId = "f_" + font + "#" + to_string(size) + "@" + text;
	if (loadedTexture.count(textId)) {
		return loadedTexture[textId];
	}

	assert(fontLookup.count(font));

	string fontName = font + to_string(size);
	if (!loadedFont.count(fontName)) {
		auto newFont = TTF_OpenFont(fontLookup[font].c_str(), size);
		assert(newFont != NULL);
		loadedFont[fontName] = newFont;
	}

	auto surface=TTF_RenderText_Blended(loadedFont[fontName], text.c_str(), color);
	assert(surface != NULL);

	//Create texture from surface pixels
	auto mTexture = SDL_CreateTextureFromSurface(gRenderer, surface);
	assert(mTexture != NULL);

	//Create texture wrapper for saving
	Texture* texture = new Texture();
	texture->texture = mTexture;
	texture->w = surface->w;
	texture->h = surface->h;

	//Get rid of old surface
	SDL_FreeSurface(surface);
	
	printf("Loaded text texture: %s = %p\n", textId.c_str(), texture);
	loadedTexture[textId] = texture;
	return texture;
}


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
	for (auto x : loadedFont) {
		TTF_CloseFont(x.second);
	}
	loadedFont.clear();
	fontLookup.clear();
}

Texture* AssetManager::operator[](string name) {
	return loadedTexture[name];
}