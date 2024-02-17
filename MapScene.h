#pragma once
#pragma once
#include "Scene.h"
#include "Sprite.h"
#include "Action.h"
#include "Label.h"
#include "LabelButton.h"
#include "GraphicsConstant.h"
#include "LevelInfo.h"
#include "DifficultyScene.h"

class MapScene : public Scene {
private:
	Sprite* bgImg;

	Label* titleLabel;

	Sprite* backBtn;

	Sprite* levelButton[LEVEL_COUNT];

	VecI levelPos[LEVEL_COUNT] = { VecI(249,481),VecI(382,343),VecI(658,213) };

public:

	void recoverState();

	void init() override {
		bgGroup = new Actor();

		//add background
		bgImg = new Sprite(am["map"]);
		bgImg->position = { -200,-200 };
		bgImg->color.a = 0;
		bgGroup->addChild(bgImg);

		fgGroup = new Actor();

		//add title
		titleLabel = new Label("global", 48, "Select Level", { 23,33,66,255 });
		titleLabel->pivot = { 0.5,0.5 };
		titleLabel->position = { 0,SCREEN_HEIGHT / 7 };
		titleLabel->color.a = 0;
		fgGroup->addChild(titleLabel);

		//back button does not work anymore

		//back button
		backBtn = new Sprite(am["back"]);
		backBtn->pivot = { 0,1 };
		backBtn->position = { 50,SCREEN_HEIGHT / 7 * 6 };
		backBtn->setClick([&]() {
			recoverState();
		});
		fgGroup->addChild(backBtn);

		//add level button
		auto imgs = am.animation("flag",1,4);

		for (int i = 0; i < LEVEL_COUNT; i++) {
			auto x = new Sprite(imgs,20);
			x->size = x->size * 4;
			x->pivot = { 0.5,0.5 };
			x->color.a = 0;
			x->position = levelPos[i];
			x->setClick([=]() {
				currentLevel = i;
				scenes.add(new DifficultyScene());
				});


			levelButton[i] = x;
			fgGroup->addChild(x);
			actions.add(aalpha(x, 30, 255));
		}


		//animation
		actions.add(apara(
			{
				aalpha(bgImg,30,200),
				apara({
					aalpha(titleLabel,30,255),
					amove(titleLabel,30,VecI(SCREEN_WIDTH / 2,SCREEN_HEIGHT / 7)),
					aalpha(backBtn,30,255),
				})
			}
		));
	}

	void tick() override {

	}

	const VecI MAGIC = VecI(200, 300);

	void returned() override {
		actions.add(apara({
			amove(titleLabel,30, VecI(SCREEN_WIDTH / 2,SCREEN_HEIGHT / 7)),
			afont(titleLabel, 30, 48),
			amove(bgGroup,30,{0,0}),
			aalpha(backBtn,30,255)
			}));

		for(int i=0;i<LEVEL_COUNT;i++){
			actions.add(amove(levelButton[i], 30, levelPos[i]));
			actions.add(aalpha(levelButton[i], 30, 255));
		}
	}

	void back() override {
		actions.add(apara({
			aalpha(bgImg,30,0),
			aalpha(titleLabel,30,0),
			aalpha(backBtn,30,0)
			}));

		for (auto x : levelButton) {
			actions.add(aalpha(x, 30, 0));
		}
	}

	void forward() override {
		actions.add(apara({
			amove(titleLabel,30,VecI(400,25)),
			afont(titleLabel, 30, 20),
			amove(bgGroup,30,MAGIC-levelPos[currentLevel]),
			aalpha(backBtn,30,0)
			}));

		for(int i=0;i<LEVEL_COUNT;i++){
			actions.add(amove(levelButton[i], 30, MAGIC - levelPos[currentLevel] + levelButton[i]->position));
			if (i != currentLevel) {
				actions.add(aalpha(levelButton[i], 30, 0));
			}
		}
	}
};