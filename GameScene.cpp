#include "GameScene.h"
#include "MapScene.h"

void GameScene::recoverState() {
	scenes.set(new MapScene());
}

void GameScene::recoverState2()
{
	scenes.set(new GameScene());
}
