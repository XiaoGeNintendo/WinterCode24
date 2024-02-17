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
	vector<Label*> descLabel;

	Sprite* bg;

	Sprite* preview;

	Sprite* fightBtn;
	Sprite* backBtn;

	LabelButton* diffBtn[4];

	Sprite* diffMarker;

public:
	void init() override;

	void tick() override {

	}

	void returned() override {
	}

	void back() override {
		actions.add(amove(bg, 30, { SCREEN_WIDTH + 10,SCREEN_HEIGHT / 2 }));
	}

	void forward() override {
	}
};