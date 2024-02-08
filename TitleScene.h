#pragma once
#include "Scene.h"
#include "Sprite.h"
#include "Action.h"
#include "Label.h"
#include "LabelButton.h"

class TitleScene : public Scene {
private:
	Sprite* bgImg;

	Label* titleLabel;

	LabelButton* startBtn;
	LabelButton* exitBtn;

public:
	void init() override {
		bgGroup = new Actor();

		//add background
		bgImg = new Sprite(am["title"]);
		bgImg->color.a = 200;
		bgGroup->addChild(bgImg);
		
		fgGroup = new Actor();

		//add title
		titleLabel = new Label("global", 48, "Nanjing University Rush", {255,235,205,255});
		titleLabel->pivot = { 0.5,0.5 };
		titleLabel->position={ SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2 };
		titleLabel->color.a = 0;
		fgGroup->addChild(titleLabel);

		//add button
		startBtn = new LabelButton("global", 20, "Start Game", { 114,51,4,255 });
		startBtn->pivot = { 0.5,0.5 };
		startBtn->color.a = 0;
		startBtn->position = { SCREEN_WIDTH / 2,SCREEN_HEIGHT / 7 * 4 };
		startBtn->fClick = [&]() {
			printf("voo!\n");
		};
		fgGroup->addChild(startBtn);

		exitBtn = new LabelButton("global", 20, "Exit", { 88,88,88,255 });
		exitBtn->pivot = { 0.5,0.5 };
		exitBtn->color.a = 0;
		exitBtn->position = { SCREEN_WIDTH / 2,SCREEN_HEIGHT / 7 * 5 };
		exitBtn->fClick = [&]() {
			appCloseFlag = true;
		};
		fgGroup->addChild(exitBtn);


		//animation
		actions.add(new ActionSequence(
			{
				aalpha(bgImg, 60, 0, 255),
				new ActionParallel(
					{
						aalpha(titleLabel,60,0,255),
						amove(titleLabel,60,{SCREEN_WIDTH / 2,SCREEN_HEIGHT / 4}),
					}
				),
				new ActionParallel({
					aalpha(startBtn,30,0,255),
					new ActionSequence({new ActionDelay(15),aalpha(exitBtn,30,0,255)})
				})
			}
		));
	}

	void tick() override {

	}
	void returned() override {

	}

	void back() override {

	}

	void forward() override{

	}
};