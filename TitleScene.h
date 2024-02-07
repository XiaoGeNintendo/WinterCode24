#pragma once
#include "Scene.h"
#include "Sprite.h"
#include "Action.h"
#include "Label.h"

class TitleScene : public Scene {
private:
	Sprite* bgImg;

	Label* titleLabel;
public:
	void init() override {
		bgGroup = new Actor();

		//add background
		bgImg = new Sprite(am["title"]);
		bgImg->color.a = 200;
		bgGroup->addChild(bgImg);
		
		fgGroup = new Actor();

		//add title
		titleLabel = new Label("global", 48, "Nanjing University Rush", {123,45,67,255});
		titleLabel->centerAt({ SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2 });
		titleLabel->color.a = 0;
		fgGroup->addChild(titleLabel);

		//animation
		actions.add(new ActionSequence(
			{
				aalpha(bgImg, 60, 0, 255),
				new ActionParallel(
					{
						aalpha(titleLabel,60,0,255),
						amove(titleLabel,60,{SCREEN_WIDTH / 2,SCREEN_HEIGHT / 4},true),
					}
				)
			}
		));
	}

	void tick() override {
		int mouseX, mouseY;
		
		SDL_GetMouseState(&mouseX, &mouseY);

		fgGroup->position.x = - (mouseX-SCREEN_WIDTH/2) / 25;
		fgGroup->position.y = - (mouseY-SCREEN_HEIGHT/2) / 25;
	}
	void returned() override {

	}

	void back() override {

	}

	void forward() override{

	}
};