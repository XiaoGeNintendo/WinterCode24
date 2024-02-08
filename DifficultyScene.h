#pragma once
#pragma once
#pragma once
#include "Scene.h"
#include "Sprite.h"
#include "Action.h"
#include "Label.h"
#include "LabelButton.h"
#include "GraphicsConstant.h"
#include "LevelInfo.h"

class DifficultyScene : public Scene {
private:

	Label* titleLabel;

	LabelButton* backBtn;

	Sprite* bg;
public:
	void init() override {
		bgGroup = new Actor();
		fgGroup = new Actor();

		//add a cool background
		
		bg = new Sprite(am["dialog"]);
		bg->size = {400,500};
		bg->pivot = { 0,0.5 };
		bg->position = { SCREEN_WIDTH+10,SCREEN_HEIGHT / 2 };
		fgGroup->addChild(bg);

		//add title
		titleLabel = new Label("global", 48, "Level "+to_string(currentLevel), { 233,233,233,255 });
		titleLabel->pivot = { 0,0 };
		titleLabel->position = {50,-200};
		bg->addChild(titleLabel);

		//back button
		backBtn = new LabelButton("global", 20, "Back", { 111,111,111,255 });
		backBtn->pivot = { 0,1 };
		backBtn->position = { 50,220};
		backBtn->fClick = [&]() {
			scenes.back();
		};
		bg->addChild(backBtn);

		//add animation
		actions.add(amove(bg, 30, { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 }));
	}

	void tick() override {

	}

	const VecI MAGIC = VecI(200, 300);

	void returned() override {
	}

	void back() override {
		actions.add(amove(bg, 30, { SCREEN_WIDTH + 10,SCREEN_HEIGHT / 2 }));
	}

	void forward() override {
	}
};