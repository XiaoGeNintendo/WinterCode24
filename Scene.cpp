#include "Scene.h"
#include "GraphicsConstant.h"
SceneManager::SceneManager(Stage* stage) :stage(stage) {}

void SceneManager::add(Scene* scene) {
	//init new scene
	scene->manager = this;
	scene->init();
	scene->bgGroup->zIndex = -1000;
	scene->fgGroup->zIndex = 0;

	//call out old scene
	scenes.back()->forward();

	//add child
	stage->addChild(scene->bgGroup);
	stage->addChild(scene->fgGroup);

	//add to scenes
	scenes.push_back(scene);
}

void SceneManager::back() {
	scenes.back()->back();
	scenes[scenes.size() - 1]->returned();

	//give you one sec
	auto toDelete = scenes.back();
	scenes.pop_back();
	actions.add(new ActionSequence(
		{ new ActionDelay(90), new ActionRunnable([&]() {
			toDelete->fgGroup->removeFromParent();
			toDelete->bgGroup->removeFromParent();
			delete toDelete;
			})
		}
	));
}