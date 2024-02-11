#pragma once
#pragma once
#include "Scene.h"
#include "Sprite.h"
#include "Action.h"
#include "Label.h"
#include "LabelButton.h"
#include "MapScene.h"
#include "TowerInstance.h"
using namespace std;

class GameScene : public Scene {
private:
	Sprite* bgImg;
	LevelInfo lvl;

	vector<Sprite*> deployer;

	Sprite* heartBar;
	Label* heartDisplay;

	Sprite* goldBar;
	Label* goldDisplay;

	int openingUi;

	Sprite* buildIcons[TOWER_COUNT];
	Actor* buildUi;

	vector<TowerInstance*> towerInstances;

public:
	void init() override {
		bgGroup = new Actor();
		fgGroup = new Actor();
		lvl = levels[currentLevel];
		towerInstances.resize(lvl.deployPosition.size());

		//bg img
		bgImg = new Sprite(am[lvl.background]);
		bgGroup->addChild(bgImg);
		bgImg->setClick([=]() {
			printf("{%d,%d},", getMousePosition().x - bgImg->getGlobalPosition().x, getMousePosition().y - bgImg->getGlobalPosition().y);
			});

		//deploy position
		deployer.resize(lvl.deployPosition.size());
		for (int i = 0; i < lvl.deployPosition.size(); i++) {
			deployer[i] = new Sprite(am["signpost"]);
			deployer[i]->pivot = { 0.5,1 };
			deployer[i]->position = lvl.deployPosition[i];
			deployer[i]->setClick([=]() {
				openingUi = i;
				buildUi->position = lvl.deployPosition[i];
				buildUi->visible = true;
				});
			bgGroup->addChild(deployer[i]);
		}

		//ui heart bar
		heartBar = new Sprite(am["heartBar"]);
		heartBar->position = { 30,30 };
		fgGroup->addChild(heartBar);

		heartDisplay = new Label("global", 20, "20", { 255,255,255,255 });
		heartDisplay->position = { 60,5 };
		heartBar->addChild(heartDisplay);

		//ui coin bar
		goldBar = new Sprite(am["goldBar"]);
		goldBar->position = { 165,30 };
		fgGroup->addChild(goldBar);

		goldDisplay = new Label("global", 20, "0", { 255,255,255,255 });
		goldDisplay->position = { 60,5 };
		goldBar->addChild(goldDisplay);

		//building menu
		buildUi = new Actor();
		for (int i = 0; i < TOWER_COUNT; i++) {
			Tower* t = towers[i];
			double angle = 2 * PI / TOWER_COUNT * i;
			double radius = 100;

			buildIcons[i] = new Sprite(am[t->id]);
			buildIcons[i]->position = VecI(cos(angle) * radius, sin(angle) * radius);
			buildIcons[i]->pivot = { 0.5,0.5 };
			buildUi->addChild(buildIcons[i]);
		}
		buildUi->visible = false;
		bgGroup->addChild(buildUi);

	}

	void tick() override {
		const int SPEED = 9;

		auto mouse = getMousePosition();
		if (mouse.x >= SCREEN_WIDTH - 50) {
			if (bgImg->size.x + bgGroup->position.x - SPEED> SCREEN_WIDTH) {
				bgGroup->position.x-=SPEED;
			}
		}
		if (mouse.x <= 50) {
			if (bgGroup->position.x<0) {
				bgGroup->position.x += SPEED;
			}
		}
		if (mouse.y >= SCREEN_HEIGHT - 50) {
			if (bgImg->size.y + bgGroup->position.y - SPEED> SCREEN_HEIGHT) {
				bgGroup->position.y -= SPEED;
			}
		}
		if (mouse.y <= 50) {
			if (bgGroup->position.y < 0) {
				bgGroup->position.y += SPEED;
			}
		}
	}

	void returned() override {
		
	}

	void back() override {

	}

	void forward() override {
		
	}
};