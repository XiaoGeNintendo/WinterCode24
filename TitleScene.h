#pragma once
#include "Scene.h"
#include "Sprite.h"
#include "Action.h"
#include "Label.h"
#include "LabelButton.h"
#include "MapScene.h"

class TitleScene : public Scene {
private:
	Sprite* bgImg;

	Label* titleLabel;

	LabelButton* startBtn;
	LabelButton* exitBtn;

	Sprite* mus;
	Sprite* se;
	Sprite* musoff;
	Sprite* seoff;

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
			scenes.set(new MapScene());
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

		//mus and se
		mus = new Sprite(am["music"]);
		mus->position = { SCREEN_WIDTH / 18 * 14,SCREEN_HEIGHT / 13 * 12 };
		mus->pivot = { 0.5,0.5 };
		mus->setClick([=]() {
			save.bgm = !save.bgm;
			musoff->visible = !save.bgm;
			if (save.bgm) {
				Mix_VolumeMusic(MIX_MAX_VOLUME);
			}
			else {
				Mix_VolumeMusic(0);
			}
			saveSave();
		});
		fgGroup->addChild(mus);


		se = new Sprite(am["se"]);
		se->position = { SCREEN_WIDTH / 18 * 16,SCREEN_HEIGHT / 13 * 12 };
		se->pivot = { 0.5,0.5 };
		se->setClick([=]() {
			save.se = !save.se;
			seoff->visible = !save.se;
			if (save.se) {
				Mix_Volume(-1, MIX_MAX_VOLUME);
				am.playSE("click");
			}
			else {
				Mix_Volume(-1, 0);
			}
			saveSave();
		});
		fgGroup->addChild(se);

		musoff = new Sprite(am["no"]);
		musoff->position = { SCREEN_WIDTH / 18 * 14-10,SCREEN_HEIGHT / 13 * 12-10 };
		musoff->pivot = { 0.5,0.5 };
		musoff->visible = !save.bgm;
		fgGroup->addChild(musoff);


		seoff = new Sprite(am["no"]);
		seoff->position = { SCREEN_WIDTH / 18 * 16-10,SCREEN_HEIGHT / 13 * 12-10 };
		seoff->pivot = { 0.5,0.5 };
		seoff->visible = !save.se;
		fgGroup->addChild(seoff);

		//animation
		actions.add(aseq(
			{
				aalpha(bgImg, 60, 0, 255),
				apara(
					{
						aalpha(titleLabel,60,0,255),
						amove(titleLabel,60,{SCREEN_WIDTH / 2,SCREEN_HEIGHT / 4}),
					}
				),
				apara({
					aalpha(startBtn,30,0,255),
					aseq({adelay(15),aalpha(exitBtn,30,0,255)})
				})
			}
		));

		//play music
		am.playMus("title");
	}

	void tick() override {

	}

	void returned() override {
		actions.add(
			apara({
				amove(titleLabel,30,VecI(SCREEN_WIDTH/2,SCREEN_HEIGHT/4)),
				afont(titleLabel,30,48),
				aalpha(startBtn,30,255),
				aalpha(exitBtn,30,255),
				aalpha(bgImg,30,255)
			})
		);
	}

	void back() override {

	}

	void forward() override{
		actions.add(
			apara({
				amove(titleLabel,30,VecI(150,25)),
				afont(titleLabel,30,16),
				aalpha(startBtn,30,0),
				aalpha(exitBtn,30,0),
				aalpha(bgImg,30,0)
				})
		);
	}
};