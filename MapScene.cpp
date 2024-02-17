#include "MapScene.h"
#include "TitleScene.h"

void MapScene::recoverState() {
	scenes.set(new TitleScene());
}