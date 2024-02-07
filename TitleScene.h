#pragma once
#include "Scene.h"
#include "Sprite.h"
#include "Action.h"

class TitleScene : public Scene {
private:
	Sprite* bgImg;
public:
	void init() override {
		bgGroup = new Actor();
		bgImg = new Sprite(am["title"]);
		actions.add(aalpha(bgImg, 60, 0, 255));
		bgGroup->addChild(bgImg);

		fgGroup = new Actor();
	}

	void returned() override {

	}

	void back() override {

	}

	void forward() override{

	}
};