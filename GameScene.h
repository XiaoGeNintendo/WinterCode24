#pragma once
#pragma once
#include "Scene.h"
#include "Sprite.h"
#include "Action.h"
#include "Label.h"
#include "LabelButton.h"
#include "MapScene.h"
#include "TowerInstance.h"
#include <cassert>
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

	int openingUi=-1;

	Sprite* buildIcons[TOWER_COUNT];
	Actor* buildUi;

	vector<TowerInstance*> towerInstances;
	vector<Sprite*> towerSprites;

	Actor* towerSpriteGroup;

	Sprite* tooltipWindow;
	Label* tooltipText;

	int playerHp = 20;
	int playerGold = 0;

public:
	void init() override {
		bgGroup = new Actor();
		fgGroup = new Actor();
		lvl = levels[currentLevel];
		towerInstances.resize(lvl.deployPosition.size());
		towerSprites.resize(lvl.deployPosition.size());

		//setup basic stuff
		if (currentDifficulty == EASY) {
			playerHp = 20;
			playerGold = 500;
		}
		else if (currentDifficulty == NORMAL) {
			playerHp = 10;
			playerGold = 500;
		}
		else if (currentDifficulty == HARD) {
			playerHp = 3;
			playerGold = 350;
		}
		else if (currentDifficulty == LUNATIC) {
			playerHp = 1;
			playerGold = 300;
		}
		
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
				if (openingUi == i) {
					return;
				}
				openingUi = i;
				buildUi->position = lvl.deployPosition[i] - VecI(0, 16);
				buildUi->visible = true;

				for (int i = 0; i < TOWER_COUNT; i++) {
					Tower* t = towers[i];
					double angle = 2 * PI / TOWER_COUNT * i;
					double radius = 70;

					actions.add(amove(buildIcons[i], 15, { 0,0 }, VecI(cos(angle) * radius, sin(angle) * radius)));
				}
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
			double radius = 70;

			auto standbyPos = VecI(cos(angle) * radius, sin(angle) * radius);

			buildIcons[i] = new Sprite(am[t->id]);
			buildIcons[i]->position = standbyPos;
			buildIcons[i]->pivot = { 0.5,0.5 };
			buildIcons[i]->setHover([=]() {
				tooltipText->text = "[" + to_string(t->price) + "G] " + t->name;
				tooltipText->markDirty();
				tooltipWindow->visible = true;
			}, [=]() {
				tooltipWindow->visible = false;
			});

			buildIcons[i]->setClick([=]() {
				assert(openingUi != -1);
				assert(towerInstances[openingUi] == NULL);

				if (playerGold < towers[i]->price) {
					auto now = standbyPos;

					actions.add(aseq(
						{
							amove(buildIcons[i],5,now, now + VecI(5,0)),
							amove(buildIcons[i],5,now + VecI(5,0), now - VecI(5,0)),
							amove(buildIcons[i],5,now - VecI(5,0), now + VecI(5,0)),
							amove(buildIcons[i],5,now + VecI(5,0), now),
						}
					));
					return;
				}

				playerGold -= towers[i]->price;

				towerInstances[openingUi] = new TowerInstance();
				towerInstances[openingUi]->level = 1;
				towerInstances[openingUi]->tower = towers[i];
				towerSprites[openingUi] = new Sprite(am[towers[i]->id + "_b"]);
				towerSprites[openingUi]->position = lvl.deployPosition[openingUi] - VecI(0,50);
				towerSprites[openingUi]->pivot = { 0.5,0.9 };
				towerSprites[openingUi]->color.a = 0;

				actions.add(apara({
					aalpha(towerSprites[openingUi],30,255),
					amove(towerSprites[openingUi],30,lvl.deployPosition[openingUi]),
					}));

				towerSpriteGroup->addChild(towerSprites[openingUi]);
				deployer[openingUi]->visible = false;
				buildUi->visible = false;
				tooltipWindow->visible = false;
			});

			buildUi->addChild(buildIcons[i]);
		}
		buildUi->visible = false;
		buildUi->zIndex = 10;
		bgGroup->addChild(buildUi);

		//add tower sprite group
		towerSpriteGroup = new Actor();
		bgGroup->addChild(towerSpriteGroup);

		//build tooltip
		tooltipWindow = new Sprite(am["tooltip"]);
		tooltipWindow->zIndex = 10;
		tooltipWindow->size.x *= 2;
		tooltipText = new Label("global", 12, "ww", { 255,255,255,255 });
		tooltipText->position = { 8,8 };
		tooltipWindow->addChild(tooltipText);
		tooltipWindow->visible = false;
		fgGroup->addChild(tooltipWindow);

	}

	void tick() override {
		//setup health
		heartDisplay->text = to_string(playerHp);
		goldDisplay->text = to_string(playerGold);
		heartDisplay->markDirty();
		goldDisplay->markDirty();

		//move tooltip
		tooltipWindow->position = getMousePosition() + VecI(15,15);

		//move map with mouse
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