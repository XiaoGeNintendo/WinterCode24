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
#include "GameScene.h"

class DifficultyScene : public Scene {
private:

	Label* titleLabel;
	vector<Label*> descLabel;

	Sprite* bg;

	Sprite* preview;

	Sprite* fightBtn;
	LabelButton* backBtn;

	LabelButton* diffBtn[4];

	Sprite* diffMarker;

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

		//add description
		descLabel.resize(levels[currentLevel].desc.size());
		for (int i = 0; i < levels[currentLevel].desc.size(); i++) {
			descLabel[i] = new Label("global", 12, levels[currentLevel].desc[i], { 255,255,255,255 });
			descLabel[i]->position = { 30,-120+20*i };
			bg->addChild(descLabel[i]);
		}

		//add preview picture
		preview = new Sprite(am[levels[currentLevel].background]);
		preview->position = { 200,30 };
		preview->size = { 160,90 };
		bg->addChild(preview);

		//add difficulty button
		string diff[] = { "Easy","Normal","Hard","Lunatic" };
		SDL_Color color[] = { {84,255,159,255} ,{100,149,237,255},{178,34,34,255},{148,0,211,255} };

		for (int i = 0; i < 4; i++) {
			diffBtn[i] = new LabelButton("global", 16, diff[i],color[i]);
			diffBtn[i]->showBg = false;
			diffBtn[i]->position = { 60,30 + 20 * i };
			diffBtn[i]->fClick = [=]() {
				currentDifficulty = i;
				diffMarker->position = { 40,30 + 20 * i };
			};
			bg->addChild(diffBtn[i]);
		}

		//add marker
		diffMarker = new Sprite(am["difficultyMarker"]);
		diffMarker->position = { 40,30 };
		diffMarker->pivot = { 0.5,0.5 };
		bg->addChild(diffMarker);

		//start button
		fightBtn = new Sprite(am["fightBtn"]);
		fightBtn->pivot = { 0,1 };
		fightBtn->position = { 50,220 };
		fightBtn->setClick([&]() {
			scenes.set(new GameScene());
		});
		bg->addChild(fightBtn);

		//back button
		backBtn = new LabelButton("global", 20, "Back", { 111,111,111,255 });
		backBtn->pivot = { 0,1 };
		backBtn->position = { 150,220};
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