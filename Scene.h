#pragma once
#include "Actor.h"
#include "Stage.h"

//definition
struct SceneManager;

/**
@author XGN
*/
class Scene {
public:
	Actor* bgGroup;
	Actor* fgGroup;
	SceneManager* manager;

	/**
		This event is fired when the scene is about to be displayed, especially creating the bgGroup and fgGroup

		You do NOT need to implement changing the Z-index as manager will do it.
	*/
	virtual void init() = 0;
	/**
		This event is fired when the scene is about to be forwarded to a higher level scene
	*/
	virtual void forward() = 0;

	/**
		This event is fired when the scene is returned from a higher level scene
	*/
	virtual void returned() = 0;
	/**
		This event is fired when the scene is about to be deleted
	*/
	virtual void back() = 0;

	~Scene() {
		if (bgGroup != NULL) {
			delete bgGroup;
		}
		if (fgGroup != NULL) {
			delete fgGroup;
		}
	}
};


struct SceneManager {
	vector<Scene*> scenes;
	Stage* stage;

	SceneManager(Stage* stage);

	void add(Scene* scene);

	void back();
};
