#include "Towers.h"
#include "GameScene.h"

void SoldierTower::upgrade() {
	radius = 230 + 20 * level;
	lastSoldier = 0;
}

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

	if (scc < 3+level && lastSoldier<=0) {
		auto newSoldier = new Sprite(am.animation("soldier_w",1,2),1e9);
		newSoldier->position = this->position + VecI(rand()%10-5,rand()%20);
		
		sc->projectileSpriteGroup->addChild(newSoldier);

		auto soldier = Soldier();
		soldier.locator = newSoldier;
		soldier.hp = SOLDIER_HP;
		soldier.state = SOLDIER_IDLE;
		soldier.assemblyPoint = assemblyPosition;
		soldier.atk = 30 + 10 * level;
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

void ArcherTower::upgrade() {
	radius = 200 + 25 * level;
}

void ArcherTower::tick()
{
	if (lastArrow > 0) {
		lastArrow--;
	}

	GameScene* sc = (GameScene*)scene;

	if (lastArrow == 0) {

		int count = 0;

		for (auto& enemy : sc->enemyInstances) {
			if (enemy.noProcess) {
				continue;
			}

			double dist = (enemy.position - i2d(this->position)).magnitude();
			if (dist <= radius) {
				//add new projectile
				Sprite* arrow = new Sprite(am["arrow"]);
				arrow->position = this->position - VecI(0, 50);
				arrow->rotation = atan2(enemy.position.x - this->position.x, enemy.position.y - this->position.y) / PI * 180;

				EnemyInstance* e = &enemy;
				actions.add(aseq({
					amove(arrow,10,d2i(enemy.position)),
					new ActionRunnable([=]() {
						if (!e->noProcess) {
							e->hp -= (15 + 5 * level) - e->data->defense;
						}

						arrow->removeFromParent();
						delete arrow;
					})
				}));

				sc->projectileSpriteGroup->addChild(arrow);
				
				count++;
				if (count == 1 + level) {
					break;
				}
			}
		}
		lastArrow = 45 - 5 * level;
	}
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
