#pragma once
#include "Texture.h"
#include <map>
#include <string>
using namespace std;

class AssetManager {
public:
	map<string, Texture*> loadedTexture;

	void load(string name, string path);
	void close();

	Texture* operator[](string name);
};