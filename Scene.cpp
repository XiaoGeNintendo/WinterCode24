#include "Scene.h"
#include "GraphicsConstant.h"
#include "Sprite.h"

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

void SceneManager::set(Scene* scene)
{
	am.stopMus();
	am.playSE("close");
	for (auto x : actions.actions) {
		delete x;
	}
	actions.actions.clear();

	//unbind mouse
	scenes.back()->bgGroup->mousePolicy = 0;
	scenes.back()->fgGroup->mousePolicy = 0;
	
	Sprite* left = new Sprite(am["transition"]);
	Sprite* right = new Sprite(am["transition"]);
	left->position = { -400,0 };
	right->position = { 800,0 };
	left->zIndex = right->zIndex = 1000;
	
	stage->addChild(left);
	stage->addChild(right);

	actions.add(aseq({
		apara({
			amove(left,30,{0,0}),
			amove(right,30,{400,0})
		}),
		new ActionRunnable([=]() {
			while (!scenes.empty()) {
				scenes.back()->bgGroup->removeFromParent();
				scenes.back()->fgGroup->removeFromParent();
				delete scenes.back();
				scenes.pop_back();
			}

			add(scene);
		}),
		adelay(60),
		new ActionRunnable([=]() {am.playSE("open"); }),
		apara({
			amove(left,30,{0,0}, {-400,0}), //NOTE: BUG in action lazy to fix though
			amove(right,30,{400,0}, {800,0})
		}),
		new ActionRunnable([=]() {
			left->removeFromParent();
			right->removeFromParent();
			delete left;
			delete right;
		})
	}));
}
