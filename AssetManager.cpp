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
	if (surface == NULL) {
		printf("Fatal: Cannot load text due to %s.", TTF_GetError());
		assert(false);
	}

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

void AssetManager::loadSE(string name, string path)
{
	auto x = Mix_LoadWAV(path.c_str());
	if (x == NULL)
	{
		printf("Fatal: Failed to load sound effect %s! SDL_mixer Error: %s\n", name.c_str(),Mix_GetError());
		exit(ERR_LOAD_SE);
	}

	printf("SUCCESS: Loaded SE:%s\n", name.c_str());
	se[name] = x;
}

void AssetManager::loadMusic(string name, string path)
{
	auto x = Mix_LoadMUS(path.c_str());
	if (x == NULL)
	{
		printf("Fatal: Failed to load sound effect %s! SDL_mixer Error: %s\n", name.c_str(), Mix_GetError());
		exit(ERR_LOAD_MUSIC);
	}

	printf("SUCCESS: Loaded Music:%s\n", name.c_str());
	music[name] = x;
}

Mix_Music* AssetManager::m(string name)
{
	assert(music.count(name));
	return music[name];
}

Mix_Chunk* AssetManager::s(string name)
{
	assert(se.count(name));
	return se[name];
}

void AssetManager::playSE(string name)
{
	Mix_PlayChannel(-1, se[name], 0);
}

void AssetManager::playMus(string name) {
	if (Mix_PlayingMusic()) {
		Mix_HaltMusic();
	}

	Mix_PlayMusic(music[name],-1);
}

void AssetManager::stopMus() {
	if (Mix_PlayingMusic()) {
		Mix_FadeOutMusic(500);
	}
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
	SDL_SetTextureBlendMode(newTexture, SDL_BLENDMODE_BLEND);
	t->texture = newTexture;
	assert(newTexture != NULL);

	//Add to map
	printf("SUCCESS: Loaded asset with name %s.\n", name.c_str());
	loadedTexture[name] = t;
}

void AssetManager::loadEnemy(string prefix, int wac, int aac, bool thumbnail)
{
	for (int i = 1; i <= wac; i++) {
		string n = prefix + "_w" + to_string(i);
		load(n, "img/" + n + ".png");
	}
	for (int i = 1; i <= aac; i++) {
		string n = prefix + "_a" + to_string(i);
		load(n, "img/" + n + ".png");
	}

	if (thumbnail) {
		load(prefix+"_t", "img/" + prefix + "_t.png");
	}
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
	for (auto m : music) {
		Mix_FreeMusic(m.second);
	}
	for (auto s : se) {
		Mix_FreeChunk(s.second);
	}
	loadedFont.clear();
	fontLookup.clear();
}

vector<Texture*> AssetManager::animation(string prefix, int start, int end)
{
	vector<Texture*> v;
	for (int i = start; i <= end; i++) {
		string now = prefix + to_string(i);
		assert(loadedTexture.count(now));
		v.push_back(loadedTexture[now]);
	}
	return v;
}


Texture* AssetManager::operator[](string name) {
	assert(loadedTexture.count(name));
	return loadedTexture[name];
}