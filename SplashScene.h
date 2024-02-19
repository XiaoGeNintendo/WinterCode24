#pragma once
#pragma once
#include "Scene.h"
#include "Sprite.h"
#include "Action.h"
#include "Label.h"
#include "LabelButton.h"
#include "TitleScene.h"

class SplashScene : public Scene {
private:
	Sprite* bgImg;

public:
	void init() override {
		bgGroup = new Actor();

		//add background
		bgImg = new Sprite(am["splash"]);
		bgImg->color.a = 0;
		bgGroup->addChild(bgImg);
		bgImg->setClick([=]() {
			scenes.add(new TitleScene()); });

		actions.add(aalpha(bgImg, 60, 255));
		fgGroup = new Actor();
		
	}

	void tick() override {

	}

	void returned() override {
	}

	void back() override {

	}

	void forward() override {
		actions.add(aalpha(bgImg, 60, 0));
	}
};