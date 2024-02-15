#include "Towers.h"
#include "GameScene.h"
#include "BombSprite.h"
#include "Random.h"

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
		newSoldier->position = this->position + VecI(randInt(-20,20),randInt(10,20));
		
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

	if (lastArrow <= 0) {

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
				arrow->rotation = atan2(enemy.position.y - arrow->position.y, enemy.position.x - arrow->position.x) / PI * 180 + 180;

				EnemyInstance* e = &enemy;
				actions.add(aseq({
					amove(arrow,10,d2i(enemy.position+enemy.speed*5)),
					new ActionRunnable([=]() {
						if (!e->noProcess) {
							int dmg = (5 + 5 * level) - e->data->defense;
							e->hp -= dmg; //remove enemy hp
							sc->displayDamage(e->position, dmg);
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

void BomberTower::upgrade() {
	radius = 130 + 20 * level;
}

void BomberTower::tick()
{
	if (lastBomb > 0) {
		lastBomb--;
	}

	GameScene* sc = (GameScene*)scene;

	if (lastBomb <= 0) {
		for (auto& enemy : sc->enemyInstances) {
			if (enemy.noProcess) {
				continue;
			}

			double dist = (enemy.position - i2d(this->position)).magnitude();
			if (dist > radius) {
				continue;
			}

			//shoot bomb at it
			VecD shootPos = i2d(this->position - VecI(0, 120));

			const int IGNITE = 120;
			
			auto bomb = new BombSprite(am["bomb"], shootPos, enemy.position + enemy.speed * IGNITE / 2, VecD(shootPos.x, 0), IGNITE);
			bomb->realPosition = shootPos;
			bomb->size = bomb->size * 2;
			bomb->ignite = [=]() {
				for (auto& enemy : sc->enemyInstances) {
					if (enemy.noProcess) {
						continue;
					}


					double dist = (enemy.position - bomb->realPosition).magnitude();
					if (dist > BOMB_RADIUS) {
						continue;
					}

					int dmg = (60 + 10 * level) * (BOMB_RADIUS - dist) / BOMB_RADIUS - enemy.data->defense;
					if (dmg > 0) {
						enemy.hp -= dmg;
						sc->displayDamage(enemy.position, dmg);
					}
				}

				bomb->texture = am["boom"];
				bomb->size = am["boom"]->size();
				actions.add(aseq({ aalpha(bomb,30,0),new ActionRunnable([=]() {bomb->removeFromParent(); delete bomb; }) }));

			};
			sc->projectileSpriteGroup->addChild(bomb);
			lastBomb = 60 * 3 - 30 * level;
			break;
		}
	}
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
