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
	if (!scenes.empty()) {
		scenes.back()->forward();
		scenes.back()->bgGroup->mousePolicy = 0;
		scenes.back()->fgGroup->mousePolicy = 0;
	}

	//add child
	stage->addChild(scene->bgGroup);
	stage->addChild(scene->fgGroup);

	//add to scenes
	scenes.push_back(scene);
}

void SceneManager::back() {
	scenes.back()->back();
	scenes.back()->bgGroup->mousePolicy = 0;
	scenes.back()->fgGroup->mousePolicy = 0;

	if (scenes.size() == 1) {
		appCloseFlag = true;
		return;
	}
	scenes[scenes.size() - 2]->returned();


	//give you one sec
	auto toDelete = scenes.back();
	scenes.pop_back();
	actions.add(new ActionSequence(
		{ new ActionDelay(90), new ActionRunnable([=]() {
			toDelete->fgGroup->removeFromParent();
			toDelete->bgGroup->removeFromParent();
			delete toDelete;
			})
		}
	));

	//recover mouse
	scenes.back()->bgGroup->mousePolicy = MOUSE_FALL_THROUGH;
	scenes.back()->fgGroup->mousePolicy = MOUSE_FALL_THROUGH;
}

void SceneManager::tick() {
	if (!scenes.empty()) {
		scenes.back()->tick();
	}
}