#pragma once
#pragma once
#include "Scene.h"
#include "Sprite.h"
#include "Action.h"
#include "Label.h"
#include "LabelButton.h"
#include "GraphicsConstant.h"
#include "LevelInfo.h"

class MapScene : public Scene {
private:
	Sprite* bgImg;

	Label* titleLabel;

	LabelButton* backBtn;

	Sprite* levelButton[LEVEL_COUNT];

	VecI levelPos[LEVEL_COUNT] = { VecI(249,481),VecI(382,343),VecI(658,213) };

public:
	void init() override {
		bgGroup = new Actor();

		//add background
		bgImg = new Sprite(am["map"]);
		bgImg->color.a = 0;
		bgGroup->addChild(bgImg);

		fgGroup = new Actor();

		//add title
		titleLabel = new Label("global", 48, "Select Level", { 23,33,66,255 });
		titleLabel->pivot = { 0.5,0.5 };
		titleLabel->position = { 0,SCREEN_HEIGHT / 7 };
		titleLabel->color.a = 0;
		fgGroup->addChild(titleLabel);

		//back button
		backBtn = new LabelButton("global", 20, "Back", { 111,111,111,255 });
		backBtn->pivot = { 0.5,0.5 };
		backBtn->position = { 50,SCREEN_HEIGHT / 7 * 6 };
		backBtn->color.a = 0;
		backBtn->fClick = [&]() {
			scenes.back();
		};
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
				//TODO level scene
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

	void returned() override {
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
	}
};