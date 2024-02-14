#include "Towers.h"
#include "GameScene.h"

void SoldierTower::tick()
{
	if (lastSoldier > 0) {
		lastSoldier--;
	}

	GameScene* sc = (GameScene*)scene;

	int scc = 0;
	for (auto soldier : sc->soldiers) {
		if (!soldier.noProcess && soldier.from == at) {
			scc++;
		}
	}

	if (scc < 3+level && lastSoldier==0) {
		auto newSoldier = new Sprite(am.animation("soldier_w",1,2),1e9);
		newSoldier->position = this->position + VecI(rand()%10-5,rand()%20);
		
		sc->projectileSpriteGroup->addChild(newSoldier);

		auto soldier = Soldier();
		soldier.locator = newSoldier;
		soldier.hp = SOLDIER_HP;
		soldier.state = SOLDIER_IDLE;
		soldier.assemblyPoint = assemblyPosition;
		soldier.atk = 3 + level;
		soldier.from = at;
		soldier.id = sc->soldiers.size();
		sc->soldiers.push_back(soldier);

		lastSoldier = 60 * (15 - level);
	}

}

void SoldierTower::init()
{
	assemblyPosition = position;
}

void ArcherTower::tick()
{
}

void ArcherTower::init()
{
}

void BomberTower::tick()
{
}

void BomberTower::init()
{
}

void MageTower::tick()
{
}

void MageTower::init()
{
}
