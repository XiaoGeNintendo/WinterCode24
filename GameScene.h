#pragma once
#include "Scene.h"
#include "Sprite.h"
#include "Action.h"
#include "Label.h"
#include "LabelButton.h"
#include "MapScene.h"
#include "Tower.h"
#include "EnemyInstance.h"
#include <cassert>
#include "Towers.h"
#include <queue>
#include "Random.h"
#include "ParticleSprite.h"
#include "DamageLabel.h"
using namespace std;

#define pii pair<int,int>
#define distType pair<long long,pair<int,int>>

class GameScene : public Scene {
private:


	void tickEnemy() {

		//ticking enemies
		for (int i = 0; i < enemyInstances.size(); i++) {
			EnemyInstance& enemy = enemyInstances[i];

			auto sp = enemySprites[enemy.id];
			//high priority job!
			if (sp!=NULL && sp->children.size() != 0) {
				assert(sp->children[0]->children.size() != 0);
				sp->children[0]->children[0]->size.x = max(1.0 * 20 * enemy.hp / enemy.maxhp, 0.0);
			}

			if (enemy.noProcess) {
				continue;
			}

			//check dead first
			if (enemy.hp <= 0) {
				//TODO Enemy dead

				auto& s = enemySprites[enemy.id];

				for (int j = 0; j < enemy.data->dropCoin/10; j++) {
					auto coin = new ParticleSprite(am.animation("coin",1,4),10);
					coin->realPosition = enemy.position;

					projectileSpriteGroup->addChild(coin);
					actions.add(aseq({ adelay(45),aalpha(coin,15,0),aremove(coin) }));
				}

				actions.add(aseq({ aalpha(s,30,0),aremove(s)}));

				playerGold += enemy.data->dropCoin;
				enemySprites[enemy.id] = NULL;
				enemy.noProcess = true;
				continue;
			}

			//remove dead/retreating fighter
			for (int i = 0; i < enemy.fighting.size(); i++) {
				if (soldiers[enemy.fighting[i]].noProcess || soldiers[enemy.fighting[i]].state==SOLDIER_RETREATING) {
					enemy.fighting.erase(i + enemy.fighting.begin());
					i--;
				}
			}

			//check should switch behaviour
			if (enemy.fighting.size() == 0) {
				enemy.state = ENEMY_WALKING;
			}

			//checking effects
			if (enemy.slowdownTimer > 0) {
				enemy.slowdownTimer--;
			}

			//behaviour
			if (enemy.state == ENEMY_WALKING) {
				//calculate speed
				enemy.speed = i2d(enemy.path[enemy.nextPoint] - enemy.path[enemy.nextPoint - 1]).normalize() * enemy.data->speed;
				if (enemy.slowdownTimer > 0) {
					enemy.speed = enemy.speed / 2;
				}
				enemy.position = enemy.position + enemy.speed;

				//check if in bound
				auto a = enemy.path[enemy.nextPoint];
				auto b = enemy.path[enemy.nextPoint - 1];
				VecI mn = { min(a.x,b.x),min(a.y,b.y) };
				VecI mx = { max(a.x,b.x),max(a.y,b.y) };
				if (!inRect(mn, mx - mn, enemy.position)) {
					enemy.position = i2d(a);
					enemy.nextPoint++;
					if (enemy.nextPoint == enemy.path.size()) {
						playerHp--;
						enemySprites[enemy.id]->removeFromParent();
						delete enemySprites[enemy.id];
						enemySprites[enemy.id] = NULL;

						enemy.noProcess = true;
						continue;
						//TODO player hp 0
					}
				}
			}
			else if (enemy.state == ENEMY_PAUSING) {
				//do nothing
			}
			else if (enemy.state == ENEMY_FIGHTING) {
				enemy.attackTimer--;
				if (enemy.attackTimer < 0) {
					enemy.attackTimer = enemy.data->attackDelay;
					auto& atk = soldiers[enemy.fighting[0]];
					atk.hp -= enemy.data->attack;
					displayDamage(atk.locator->position, enemy.data->attack);
				}
			}

			//linking sprites
			sp->position = d2i(enemy.position);
			sp->flipX = (enemy.speed.x > 0);
			if (enemy.state == ENEMY_WALKING) {
				sp->setAnimation(am.animation(enemy.data->id + "_w", 1, enemy.data->wac), 20);
			}
			else if (enemy.state == ENEMY_PAUSING) {
				sp->setAnimation(am.animation(enemy.data->id + "_w", 1, enemy.data->wac), 1e9);
			}
			else if (enemy.state == ENEMY_FIGHTING) {
				sp->setAnimation(am.animation(enemy.data->id + "_a", 1, enemy.data->wac), 20);
				sp->flipX = false;
			}

		}
	}

	void tickSoldier() {
		//ticking soldiers
		for (int i = 0; i < soldiers.size(); i++) {
			auto& soldier = soldiers[i];

			auto sp = soldier.locator;
			if (soldier.noProcess) {
				continue;
			}

			//high priority job!
			if (sp != NULL && sp->children.size() != 0) {
				if (sp->children[0]->children.size() != 0) {
					sp->children[0]->children[0]->size.x = max(1.0 * 20 * soldier.hp / soldier.maxhp, 0.0);
				}
				else {
					printf("WARNING: ticking glitched soldier hp bar on %d which has hp %d\n", i, soldier.hp);
				}
			}

			//test for dead first
			if (soldier.hp <= 0) {
				//TODO soldier dead

				soldier.locator->removeFromParent();
				delete soldier.locator;
				soldier.noProcess = true;
				continue;
			}

			//switch behaviour
			if ((soldier.fighting == -1 || enemyInstances[soldier.fighting].noProcess) && soldier.state!=SOLDIER_RETREATING) {
				soldier.state = SOLDIER_IDLE;
				soldier.fighting = -1;
			}

			//behaviour
			if (soldier.state == SOLDIER_IDLE) {
				pair<int, int> bestEnemy = { 100,-1 };

				for (auto& enemy : enemyInstances) {
					double dist = (enemy.position - i2d(soldier.assemblyPoint)).magnitude();
					if (dist > ASSEMBLY_RANGE) { //too far
						continue;
					}

					if (enemy.fighting.size() >= 2) {
						continue;
					}

					bestEnemy = min(bestEnemy, make_pair((int)(enemy.fighting.size()), enemy.id));

				}

				if (bestEnemy.second != -1) { //find appropriate enemy
					auto& enemy = enemyInstances[bestEnemy.second];
					//pathfind soldier
					auto path = pathfind(soldier.locator->position, d2i(enemy.position) - VecI(randInt(20,25), randInt(-20,20)), false);
					if (path.size() == 0) {
						continue;
					}

					//add to target list
					enemy.fighting.push_back(soldier.id);
					soldier.fighting = enemy.id;

					if (enemy.state == ENEMY_WALKING) { //still walking
						enemy.state = ENEMY_PAUSING;
					}
					soldier.state = SOLDIER_MOVING;
					soldier.steps = path;
				}

			}
			else if (soldier.state == SOLDIER_MOVING) {
				soldier.locator->position = soldier.steps.back();
				soldier.steps.pop_back();
				if (soldier.steps.size() == 0) {
					soldier.state = SOLDIER_FIGHTING;
					enemyInstances[soldier.fighting].state = ENEMY_FIGHTING;
				}
			}
			else if (soldier.state == SOLDIER_FIGHTING) {
				soldier.attackTimer--;
				if (soldier.attackTimer < 0) {
					//TODO damage enemy effect
					soldier.attackTimer = 60;
					auto& atk = enemyInstances[soldier.fighting];
					atk.hp -= soldier.atk - atk.data->defense;
					displayDamage(atk.position, soldier.atk - atk.data->defense);
				}
			}
			else if (soldier.state == SOLDIER_RETREATING) {
				soldier.locator->position = soldier.steps.back();
				soldier.steps.pop_back();
				if (soldier.steps.size() == 0) {
					soldier.state = SOLDIER_IDLE;
				}
			}

			//update sprite
			string prefix = soldier.from == -1 ? "farmer" : "soldier";

			if (soldier.state == SOLDIER_IDLE) {
				soldier.locator->setAnimation(am.animation(prefix+"_w", 1, 2), 1e9);
			}
			else if (soldier.state == SOLDIER_MOVING || soldier.state==SOLDIER_RETREATING) {
				soldier.locator->flipX = (soldier.steps.size() > 0 && soldier.steps.back().x > soldier.locator->position.x);
				soldier.locator->setAnimation(am.animation(prefix + "_w", 1, 2), 20);
			}
			else if (soldier.state == SOLDIER_FIGHTING) {
				soldier.locator->setAnimation(am.animation(prefix + "_a", 1, 2), 20);
				soldier.locator->flipX = true;
			}
		}
	}

	bool fine(VecI x) {
		for (auto obs : lvl.obstacles) {
			VecI mn = VecI(min(obs.first.x, obs.second.x), min(obs.first.y, obs.second.y));
			VecI mx = VecI(max(obs.first.x, obs.second.x), max(obs.first.y, obs.second.y)); 
			if (inRect(mn, mx-mn , x)) {
				return false;
			}
		}
		return true;
	}


	void tickWave() {

		nextWaveCountdown--;
		waveBar2->size.x = WAVE_BAR_LENGTH * nextWaveCountdown / (currentWave == lvl.waves.size()-1 ? 1e9 : lvl.waves[currentWave + 1].length);

		if (nextWaveCountdown <= 0) {
			currentWave++;
			printf("Started wave %d\n", currentWave);

			actions.add(aseq({
				apara({amove(waveLabel,30,VecI(0,0),VecI(-50,0)),aalpha(waveLabel,30,0)}),
				new ActionRunnable([=]() {waveLabel->text = "Attack | Wave " + to_string(currentWave+1) + "/" + to_string(lvl.waves.size()); waveLabel->markDirty(); waveLabel->position = VecI(50,0); }),
				apara({amove(waveLabel,30,VecI(50,0),VecI(0,0)),aalpha(waveLabel,30,0,255)})
				}));

			nextEnemy = 0;
			nextWaveCountdown = currentWave == lvl.waves.size()-1 ? 1e9 : lvl.waves[currentWave + 1].length;
			nextEnemyCountdown = lvl.waves[currentWave].delay;
		}
		nextEnemyCountdown--;
		if (nextEnemyCountdown <= 0) {
			if (currentWave != lvl.waves.size() && nextEnemy != lvl.waves[currentWave].enemies.size()) {
				printf("Generating %s\n", lvl.waves[currentWave].enemies[nextEnemy].c_str());
				//generate enemy
				auto instance = EnemyInstance();
				instance.data = &lvl.enemies[lvl.waves[currentWave].enemies[nextEnemy]];
				instance.maxhp = instance.hp = instance.data->maxhp;
				instance.path = lvl.path[instance.data->randomPath ? instance.data->pathId + randInt(0,2) : instance.data->pathId];
				instance.position = i2d(instance.path[0]);
				instance.nextPoint = 1;
				instance.state = ENEMY_WALKING;
				instance.id = enemyInstances.size();
				enemyInstances.push_back(instance);

				//generate sprite
				auto enemySprite = new Sprite(am.animation(instance.data->id + "_w", 1, instance.data->wac), 20);
				enemySprite->color.a = 0;
				actions.add(aseq({ aalpha(enemySprite, 60, 255), new ActionRunnable([=]() {
					enemySprite->addChild(generateHpBar()); }) }));
				
				enemySprites[instance.id] = enemySprite;
				
				enemySpriteGroup->addChild(enemySprite);

				nextEnemy++;
				nextEnemyCountdown = lvl.waves[currentWave].delay;

				if (nextEnemy == lvl.waves[currentWave].enemies.size()) {
					actions.add(aseq({
						apara({amove(waveLabel,30,VecI(0,0),VecI(-50,0)),aalpha(waveLabel,30,0)}),
						new ActionRunnable([=]() {waveLabel->text = "Interlude | Wave " + to_string(currentWave + 1) + "/" + to_string(lvl.waves.size()); waveLabel->markDirty(); waveLabel->position = VecI(50,0); }),
						apara({amove(waveLabel,30,VecI(50,0),VecI(0,0)),aalpha(waveLabel,30,0,255)})
					}));
				}
			}
		}
	}

	void initBuildMenu() {

		buildUi = new Actor();
		for (int i = 0; i < TOWER_COUNT; i++) {
			Tower* t = towers[i];
			double angle = 2 * PI / TOWER_COUNT * i;
			double radius = 70;

			auto standbyPos = VecI(cos(angle) * radius, sin(angle) * radius);

			buildIcons[i] = new Sprite(am[t->id]);
			buildIcons[i]->position = standbyPos;
			buildIcons[i]->pivot = { 0.5,0.5 };
			buildIcons[i]->setHover([=]() {
				tooltipText->text = "[" + to_string(t->price) + "G] " + t->name;
				tooltipText->markDirty();
				tooltipWindow->visible = true;
				}, [=]() {
					tooltipWindow->visible = false;
				});

			buildIcons[i]->setClick([=]() {
				assert(openingUi != -1);
				assert(towerInstances[openingUi] == NULL);

				if (playerGold < towers[i]->price) {
					auto now = standbyPos;

					actions.add(aseq(
						{
							amove(buildIcons[i],5,now, now + VecI(5,0)),
							amove(buildIcons[i],5,now + VecI(5,0), now - VecI(5,0)),
							amove(buildIcons[i],5,now - VecI(5,0), now + VecI(5,0)),
							amove(buildIcons[i],5,now + VecI(5,0), now),
						}
					));
					return;
				}

				playerGold -= towers[i]->price;

				towerInstances[openingUi] = towersCreateFunction[i]();
				towerInstances[openingUi]->level = 1;
				towerInstances[openingUi]->position = lvl.deployPosition[openingUi];
				towerInstances[openingUi]->scene = this;
				towerInstances[openingUi]->at = openingUi;
				towerInstances[openingUi]->init();
				towerSprites[openingUi] = new Sprite(am[towers[i]->id + "_b"]);
				towerSprites[openingUi]->position = lvl.deployPosition[openingUi] - VecI(0, 50);
				towerSprites[openingUi]->pivot = { 0.5,0.9 };
				towerSprites[openingUi]->color.a = 0;
				int __opening = openingUi;
				towerSprites[openingUi]->setClick([=]() {
					if (__opening == this->openingUi) {
						return;
					}

					//close previous menu
					buildUi->visible = false;
					assemblyIndicator->visible = false;
					//set opening ui first
					this->openingUi = __opening;

					auto it = towerInstances[__opening];
					assert(it != NULL);

					if (it->id == "soldier") {
						upgradeIcons[3]->visible = true;
						for (int i = 0; i < 4; i++) {
							double angle = 2 * PI / 4 * i;
							double radius = 70;

							auto standbyPos = VecI(cos(angle) * radius, sin(angle) * radius);
							actions.add(amove(upgradeIcons[i], 30, { 0,0 }, standbyPos));
						}

					}
					else {
						upgradeIcons[3]->visible = false;
						for (int i = 0; i < 3; i++) {
							double angle = 2 * PI / 3 * i;
							double radius = 70;

							auto standbyPos = VecI(cos(angle) * radius, sin(angle) * radius);
							actions.add(amove(upgradeIcons[i], 30, { 0,0 }, standbyPos));
						}

					}
					upgradeUi->position = it->position;
					upgradeUi->visible = true;

					//show radius circle
					actions.add(asize(radiusCircle, 30, { 0,0 }, VecI(it->radius,it->radius)*2));
					radiusCircle->position = it->position;
					radiusCircle->visible = true;

					//show assembly position
					if (it->id == "soldier") {
						auto st = (SoldierTower*)it;
						assemblyIndicator->position = st->assemblyPosition;
						actions.add(asize(assemblyCircle, 30, { 0,0 }, VecI(ASSEMBLY_RANGE, ASSEMBLY_RANGE) * 2));
						assemblyIndicator->visible = true;

					}
					});

				actions.add(apara({
					aalpha(towerSprites[openingUi],30,255),
					amove(towerSprites[openingUi],30,lvl.deployPosition[openingUi]),
					}));

				towerSpriteGroup->addChild(towerSprites[openingUi]);
				deployer[openingUi]->visible = false;
				buildUi->visible = false;
				tooltipWindow->visible = false;
				openingUi = -1;
				});

			buildUi->addChild(buildIcons[i]);
		}
		buildUi->visible = false;
		buildUi->zIndex = 10;
		bgGroup->addChild(buildUi);
	}

	void initDeployer() {
		deployer.resize(lvl.deployPosition.size());
		towerMarks.resize(lvl.deployPosition.size());

		for (int i = 0; i < lvl.deployPosition.size(); i++) {
			deployer[i] = new Sprite(am["signpost"]);
			deployer[i]->pivot = { 0.5,1 };
			deployer[i]->position = lvl.deployPosition[i];
			deployer[i]->setClick([=]() {
				if (openingUi == i) {
					buildUi->visible=false;
					openingUi = -1;
					return;
				}
				//close previous menu
				upgradeUi->visible = false;
				radiusCircle->visible = false;
				assemblyIndicator->visible = false;

				openingUi = i;
				buildUi->position = lvl.deployPosition[i] - VecI(0, 16);
				buildUi->visible = true;

				for (int i = 0; i < TOWER_COUNT; i++) {
					Tower* t = towers[i];
					double angle = 2 * PI / TOWER_COUNT * i;
					double radius = 70;

					actions.add(amove(buildIcons[i], 15, { 0,0 }, VecI(cos(angle) * radius, sin(angle) * radius)));
				}
				});
			bgGroup->addChild(deployer[i]);
		}
	}

	void initUpgradeMenu() {
		upgradeUi = new Actor();

		//retreat
		{
			upgradeIcons[0] = new Sprite(am["retreat"]);
			upgradeIcons[0]->position = { 0,0 };
			upgradeIcons[0]->pivot = { 0.5,0.5 };
			upgradeIcons[0]->setClick([=]() {
				assert(towerInstances[openingUi] != NULL);

				//remove all soldiers
				for (auto& s : soldiers) {
					if (!s.noProcess && s.from == openingUi) {
						s.locator->removeFromParent();
						delete s.locator;
						s.noProcess = true;
					}
				}

				//remove tower mark
				if (towerMarks[openingUi] != NULL) {
					towerMarks[openingUi]->removeFromParent();
					delete towerMarks[openingUi];
					towerMarks[openingUi] = NULL;
				}

				int __opening = openingUi;

				actions.add(aseq({
					apara({
						amove(towerSprites[openingUi], 30, lvl.deployPosition[openingUi] - VecI(0, 50)),
						aalpha(towerSprites[openingUi], 30, 0),
					}),
					new ActionRunnable([=]() {
						playerGold += towerInstances[__opening]->price / 2 + (towerInstances[__opening]->level - 1) * 60;

						towerSprites[__opening]->removeFromParent();
						delete towerSprites[__opening];
						towerSprites[__opening] = NULL;
						towerInstances[__opening] = NULL;
						deployer[__opening]->visible = true;
					})
					}));

				//close ui
				upgradeUi->visible = false;
				radiusCircle->visible = false;
				assemblyIndicator->visible = false;
				tooltipWindow->visible = false;
				openingUi = -1;
				});
			upgradeIcons[0]->setHover([=]() {
				tooltipText->text = "[+" + to_string(towerInstances[openingUi]->price / 2 + (towerInstances[openingUi]->level - 1) * 60) + "G] Retreat";
				tooltipText->markDirty();
				tooltipWindow->visible = true;
				}, [=]() {
					tooltipWindow->visible = false;
				});

			upgradeUi->addChild(upgradeIcons[0]);
		}

		//upgrade
		{
			upgradeIcons[1] = new Sprite(am["upgrade"]);
			upgradeIcons[1]->position = { 0,0 };
			upgradeIcons[1]->pivot = { 0.5,0.5 };
			upgradeIcons[1]->setClick([=]() {
				assert(towerInstances[openingUi] != NULL);
				auto ti = towerInstances[openingUi];
				if (ti->level - 1 >= int(ti->upgradeCost.size())) {
					tooltipText->text = "Level Max!";
					tooltipText->markDirty();
					return;
				}
				if (playerGold < ti->upgradeCost[ti->level - 1]) {
					tooltipText->text = "Not enough gold!";
					tooltipText->markDirty();
					return;
				}

				//really upgrade
				playerGold -= ti->upgradeCost[ti->level - 1];
				ti->level++;
				ti->upgrade();

				//setup tower marks
				auto& pt = towerMarks[openingUi];
				if (pt == NULL) {
					pt = new Sprite(am["badge2"]);
					pt->position = ti->position-VecI(40,0);
					towerMarkSpriteGroup->addChild(pt);
				}
				else {
					pt->texture = am["badge" + to_string(ti->level)];
				}

				for (int i = 0; i < 5; i++) {
					auto particle = new ParticleSprite(am["badge" + to_string(ti->level)]);
					particle->realPosition = i2d(ti->position);
					towerMarkSpriteGroup->addChild(particle);
					actions.add(aseq({ adelay(45),aalpha(particle,15,0),aremove(particle) }));
				}

				//close ui
				upgradeUi->visible = false;
				radiusCircle->visible = false;
				assemblyIndicator->visible = false;
				tooltipWindow->visible = false;
				openingUi = -1;
				});
			upgradeIcons[1]->setHover([=]() {
				assert(towerInstances[openingUi] != NULL);
				auto ti = towerInstances[openingUi];
				if (ti->level - 1 >= int(ti->upgradeCost.size())) {
					tooltipText->text = "Cannot upgrade";
				}
				else {
					tooltipText->text = "[" + to_string(ti->upgradeCost[ti->level - 1]) + "G] Upgrade";
				}

				tooltipText->markDirty();
				tooltipWindow->visible = true;
				}, [=]() {
					tooltipWindow->visible = false;
				});

			upgradeUi->addChild(upgradeIcons[1]);
		}

		//close
		{
			upgradeIcons[2] = new Sprite(am["close"]);
			upgradeIcons[2]->position = { 0,0 };
			upgradeIcons[2]->pivot = { 0.5,0.5 };
			upgradeIcons[2]->setClick([=]() {
				//close ui
				upgradeUi->visible = false;
				radiusCircle->visible = false;
				assemblyIndicator->visible = false;
				tooltipWindow->visible = false;
				openingUi = -1;
				});
			upgradeIcons[2]->setHover([=]() {
				tooltipText->text = "Close Menu";
				tooltipText->markDirty();
				tooltipWindow->visible = true;
				}, [=]() {
					tooltipWindow->visible = false;
				});

			upgradeUi->addChild(upgradeIcons[2]);
		}

		//assemble
		{
			upgradeIcons[3] = new Sprite(am["assemble"]);
			upgradeIcons[3]->position = { 0,0 };
			upgradeIcons[3]->pivot = { 0.5,0.5 };
			upgradeIcons[3]->setClick([=]() {
				assert(towerInstances[openingUi] != NULL);
				auto ti = towerInstances[openingUi];
				if (ti->id != "soldier") {
					return;
				}

				auto st = (SoldierTower*)ti;

				upgradeUi->visible = false;
				assemblyIndicator->visible = false;
				tooltipText->text = "Set new point";
				tooltipText->markDirty();

				radiusCircle->setClick([=]() {
					
					auto expt = getMousePosition() - bgGroup->position;

					if (!fine(expt)) {
						tooltipText->text = "Invalid position!";
						tooltipText->markDirty();
						return;
					}

					st->assemblyPosition = expt;


					for (auto& s : soldiers) {
						if (s.noProcess) {
							continue;
						}
						if (s.from != openingUi) {
							continue;
						}

						vector<VecI> path;
						do {
							path = pathfind(s.locator->position, st->assemblyPosition + VecI(randInt(-SOLDIER_WANDER_RANGE, SOLDIER_WANDER_RANGE), randInt(-SOLDIER_WANDER_RANGE, SOLDIER_WANDER_RANGE)), false);
						} while (path.empty());
						s.state = SOLDIER_RETREATING;
						s.steps = path;
						s.assemblyPoint = st->assemblyPosition;
					}

					radiusCircle->visible = false;
					assemblyIndicator->position = st->assemblyPosition;
					assemblyIndicator->visible = true;
					actions.add(aseq({ adelay(30),aalpha(assemblyIndicator,30,0),new ActionRunnable([=]() {assemblyIndicator->visible = false; assemblyIndicator->color.a = 255; }) }));
					actions.add(aseq({ adelay(30),aalpha(assemblyCircle,30,0),new ActionRunnable([=]() {assemblyCircle->color.a = 80; }) }));

					radiusCircle->setClick([=]() {});
					radiusCircle->mousePolicy = MOUSE_FALL_THROUGH;
					tooltipWindow->visible = false;
					openingUi = -1;
				});
			});
			upgradeIcons[3]->setHover([=]() {
				tooltipText->text = "Set Assembly Point";
				tooltipText->markDirty();
				tooltipWindow->visible = true;
				}, [=]() {
					tooltipWindow->visible = false;
				});

			upgradeUi->addChild(upgradeIcons[3]);
		}


		upgradeUi->zIndex = 10;
		upgradeUi->visible = false;
		bgGroup->addChild(upgradeUi);
	}

	void initSkill() {
		fireballSkill = new Sprite(am["fireball"]);
		fireballSkill->position = VecI(50, 520);
		fireballSkill->setClick([=]() {
			if (fireballTime > 0) {
				return;
			}

			if (!globalMask->visible) {
				globalMask->visible = true;
				selectedSkill = 1;
				tooltipText->text = "Select Position";
				tooltipText->markDirty();
				tooltipWindow->visible = true;
			}
			else {
				globalMask->visible = false;
				tooltipWindow->visible = false;
			}
			});

		fireballSkill->setHover([=]() {
			if (!globalMask->visible) {
				tooltipText->text = "Fireball Skill";
				tooltipText->markDirty();
				tooltipWindow->visible = true;
			}
			}, [=]() {if (!globalMask->visible) { tooltipWindow->visible = false; } });
		auto mask = new Sprite(am["mask"]);
		mask->color.a = 180;
		fireballSkill->addChild(mask);
		fgGroup->addChild(fireballSkill);


		reinforceSkill = new Sprite(am["reinforce"]);
		reinforceSkill->position = VecI(120, 520);
		reinforceSkill->setClick([=]() {
			if (reinforceTime > 0) {
				return;
			}
			if (!globalMask->visible) {
				globalMask->visible = true;
				selectedSkill = 2;
				tooltipText->text = "Select Position";
				tooltipText->markDirty();
				tooltipWindow->visible = true;
			}
			else {
				globalMask->visible = false;
				tooltipWindow->visible = false;
			}
			});
		reinforceSkill->setHover([=]() {
			if (!globalMask->visible) {
				tooltipText->text = "Reinforce Skill";
				tooltipText->markDirty();
				tooltipWindow->visible = true;
			}
			}, [=]() {if (!globalMask->visible) { tooltipWindow->visible = false; } });
		mask = new Sprite(am["mask"]);
		mask->color.a = 180;
		reinforceSkill->addChild(mask);
		fgGroup->addChild(reinforceSkill);
	}

	void tickSkill() {
		if (fireballTime > 0) {
			fireballTime--;
		}
		if (reinforceTime > 0) {
			reinforceTime--;
		}
		if (skillLeft > 0) {
			skillLeft--;
			assert(selectedSkill == 1);
			
			vector<int> candidate;
			for(int i=0;i<enemyInstances.size();i++){
				auto& enemy = enemyInstances[i];
				if (enemy.noProcess) {
					continue;
				}

				double dist = (enemy.position - i2d(skillPos)).magnitude();
				if (dist <= FIREBALL_RADIUS) {
					candidate.push_back(i);
				}
			}

			if (!candidate.empty()) {
				auto& enemy = enemyInstances[candidate[randInt(0, candidate.size() - 1)]];
				//add new projectile
				Sprite* arrow = new Sprite(am["fireball_arrow"]);
				arrow->position = VecI(skillPos.x+300, -50);
				arrow->rotation = atan2(enemy.position.y - arrow->position.y, enemy.position.x - arrow->position.x) / PI * 180 + 180;

				EnemyInstance* e = &enemy;
				actions.add(aseq({
					amove(arrow,30,d2i(enemy.position + (enemy.state == ENEMY_WALKING ? enemy.speed * 30 : VecD(0,0)))),
					new ActionRunnable([=]() {
						if (!e->noProcess) {
							int dmg = 50;
							e->hp -= dmg; //remove enemy hp
							displayDamage(e->position, dmg);
						}

						arrow->removeFromParent();
						delete arrow;
					})
				}));

				projectileSpriteGroup->addChild(arrow);
			}
			else {
				//decorative :P
				auto tp = skillPos + VecI(randInt(-100, 100), randInt(-100, 100));

				Sprite* arrow = new Sprite(am["fireball_arrow"]);
				arrow->position = VecI(skillPos.x + 300, -50);
				arrow->rotation = atan2(tp.y - arrow->position.y, tp.x - arrow->position.x) / PI * 180 + 180;

				actions.add(aseq({
					amove(arrow,30,tp),
					new ActionRunnable([=]() {
						arrow->removeFromParent();
						delete arrow;
					})
				}));

				projectileSpriteGroup->addChild(arrow);
			}
		}
		fireballSkill->children[0]->size.y = 60 * fireballTime / FIREBALL_TIME;
		reinforceSkill->children[0]->size.y = 60 * reinforceTime / REINFORCE_TIME;
	}

	void initEnemyMark() {
		for (int i = 0; i < lvl.enemyMarks.size(); i++) {
			VecI pos = lvl.enemyMarks[i];
			auto x = new Sprite(am["enemymark"]);
			x->position = pos;
			x->setHover([=]() {
				tooltipText->text = "Next wave";
				tooltipText->markDirty();
				tooltipWindow->visible = true;
				}, [=]() {tooltipWindow->visible = false; });

			x->setClick([=]() {
				
				if (currentWave==-1 || currentWave!=lvl.waves.size() && nextEnemy==lvl.waves[currentWave].enemies.size() && currentWave != lvl.waves.size() - 1) {
					nextWaveCountdown = 0;
				}
			});
			enemyMarkSpriteGroup->addChild(x);
		}
	}
public:
	Sprite* bgImg;
	LevelInfo lvl;

	vector<Sprite*> deployer;

	Sprite* heartBar;
	Label* heartDisplay;

	Sprite* goldBar;
	Label* goldDisplay;

	int openingUi=-1;

	Sprite* buildIcons[TOWER_COUNT];
	Actor* buildUi;

	Actor* upgradeUi;
	Sprite* upgradeIcons[5];

	vector<Tower*> towerInstances;
	//all living enemy instances
	vector<EnemyInstance> enemyInstances;
	vector<Soldier> soldiers;

	map<int, Sprite*> enemySprites;
	vector<Sprite*> towerSprites;
	vector<Sprite*> towerMarks;

	Actor* towerSpriteGroup;
	Actor* enemySpriteGroup;
	Actor* projectileSpriteGroup;
	Actor* towerMarkSpriteGroup;
	Actor* enemyMarkSpriteGroup;

	Sprite* tooltipWindow;
	Label* tooltipText;

	Sprite* radiusCircle;
	Sprite* assemblyIndicator;
	Sprite* assemblyCircle;

	Sprite* fireballSkill;
	Sprite* reinforceSkill;
	int fireballTime;
	int reinforceTime;
	int selectedSkill;
	int skillLeft;
	VecI skillPos;
	Sprite* globalMask;

	//wave indicator
	Label* waveLabel;
	Sprite* waveBar1;
	Sprite* waveBar2;
	Actor* waveIndicator;

	int playerHp = 20;
	int playerGold = 0;

	int currentWave;
	int nextEnemy;
	int nextEnemyCountdown,nextWaveCountdown;
	int globalEnemyId = 0;

	const int WAVE_BAR_LENGTH = 400;

	Actor* generateHpBar() {
		auto bar1 = new Sprite(am["hpbar1"]);
		auto bar2 = new Sprite(am["hpbar2"]);
		bar1->position = VecI(6, -2);
		bar1->addChild(bar2);
		return bar1;
	}

	//yeah bfs is not suitable for this pathfinding as it's resource heavy but anyway
	//yeah bfs performs like shit. A* is gud use A* now.
	vector<VecI> pathfind(VecI start, VecI end, bool rev = true) {
		printf("Pathfinding... from %d %d to %d %d\n", start.x, start.y, end.x, end.y);

		//fast exit
		if (!fine(end) || !fine(start)) {
			return {};
		}

		int dx[] = { -1,0,1,1,1,0,-1,-1 };
		int dy[] = { -1,-1,-1,0,1,1,1,0 };

		map<pii, int> f, g;
		map<pii, pii> from;

		int n = bgImg->size.x;
		int m = bgImg->size.y;


		priority_queue <distType, vector<distType>, greater<distType>> q;

		g[start.p()] = 0;
		f[start.p()] = abs(end.x - start.x) + abs(end.y - start.y);

		q.push({ f[start.p()],start.p() });

		while (!q.empty()) {

			distType last = q.top();
			q.pop();

			//printf("%d %d %d %d\n", q.size(), last.first, last.second.first, last.second.second);

			if (last.first != f[last.second]) {
				continue;
			}
			if (last.second.first == end.x && last.second.second == end.y) {
				vector<VecI> path;

				VecI now = end;
				while (now != start) {
					path.push_back(now);
					now = from[now.p()];
				}

				if (rev) {
					reverse(path.begin(), path.end());
				}
				return path;
			}

			for (int i = 0; i < 8; i++) {
				pii nw;

				nw.first = last.second.first + dx[i];
				nw.second = last.second.second + dy[i];

				if (nw.first >= 0 && nw.first < n && nw.second >= 0 && nw.second < m && fine(VecI(nw))) {
					int myg = g[last.second] + 1;
					if (!g.count(nw) || myg < g[nw]) {
						from[nw] = last.second;
						g[nw] = myg;
						f[nw] = myg + abs(end.x - nw.first) + abs(end.y - nw.second);
						q.push({ f[nw],nw });
					}
				}
			}
		}

		return {};
	}

	void displayDamage(VecD pos, int amount) {

		DamageLabel* lbl;
		if (amount <= 0) {
			lbl = new DamageLabel("BLOCKED");
			lbl->color = { 33,33,33,150 };
			lbl->markDirty();
		}
		else {
			lbl = new DamageLabel(to_string(amount));
		}

		lbl->realPosition = pos;
		
		//should be pooling but i am lazy
		actions.add(aseq({ adelay(15),aalpha(lbl,15,0),aremove(lbl) }));
		projectileSpriteGroup->addChild(lbl);
	}

	void displayDamage(VecI pos, int amount) {
		displayDamage(i2d(pos), amount);
	}

	void init() override {
		bgGroup = new Actor();
		fgGroup = new Actor();
		lvl = levels[currentLevel];
		towerInstances.resize(lvl.deployPosition.size());
		towerSprites.resize(lvl.deployPosition.size());

		//setup basic stuff
		if (currentDifficulty == EASY) {
			playerHp = 20;
			playerGold = 500;
		}
		else if (currentDifficulty == NORMAL) {
			playerHp = 10;
			playerGold = 500;
		}
		else if (currentDifficulty == HARD) {
			playerHp = 3;
			playerGold = 350;
		}
		else if (currentDifficulty == LUNATIC) {
			playerHp = 1;
			playerGold = 300;
		}
		nextWaveCountdown = lvl.waves[0].length;
		nextEnemyCountdown = 1e9;
		currentWave = -1;

		//bg img
		bgImg = new Sprite(am[lvl.background]);
		bgGroup->addChild(bgImg);
		bgImg->setClick([=]() {
			printf("{%d,%d},", getMousePosition().x - bgImg->getGlobalPosition().x, getMousePosition().y - bgImg->getGlobalPosition().y);
			});

		//deploy position
		initDeployer();

		//ui heart bar
		heartBar = new Sprite(am["heartBar"]);
		heartBar->position = { 30,30 };
		fgGroup->addChild(heartBar);

		heartDisplay = new Label("global", 20, "20", { 255,255,255,255 });
		heartDisplay->position = { 60,5 };
		heartBar->addChild(heartDisplay);

		//ui coin bar
		goldBar = new Sprite(am["goldBar"]);
		goldBar->position = { 165,30 };
		fgGroup->addChild(goldBar);

		goldDisplay = new Label("global", 20, "0", { 255,255,255,255 });
		goldDisplay->position = { 60,5 };
		goldBar->addChild(goldDisplay);

		//radius circle
		radiusCircle = new Sprite(am["circle"]);
		radiusCircle->color.a = 80;
		radiusCircle->visible = false;
		radiusCircle->pivot = { 0.5,0.5 };
		radiusCircle->zIndex = 7;
		bgGroup->addChild(radiusCircle);

		//assembly indicator
		auto imgs = am.animation("flag", 1, 4);
		assemblyIndicator = new Sprite(imgs, 20);
		assemblyIndicator->size = assemblyIndicator->size * 2;
		assemblyIndicator->pivot = { 0.5,0.5 };
		assemblyIndicator->visible = false;
		assemblyIndicator->zIndex = 8;
		bgGroup->addChild(assemblyIndicator);

		assemblyCircle = new Sprite(am["circle2"]);
		assemblyCircle->color.a = 80;
		assemblyCircle->pivot = { 0.5,0.5 };
		assemblyIndicator->addChild(assemblyCircle);

		//building menu
		initBuildMenu();

		//upgrade menu
		initUpgradeMenu();


		//add sprite group
		enemySpriteGroup = new Actor();
		bgGroup->addChild(enemySpriteGroup);

		projectileSpriteGroup = new Actor();
		bgGroup->addChild(projectileSpriteGroup);

		towerSpriteGroup = new Actor();
		bgGroup->addChild(towerSpriteGroup);

		towerMarkSpriteGroup = new Actor();
		bgGroup->addChild(towerMarkSpriteGroup);

		enemyMarkSpriteGroup = new Actor();
		bgGroup->addChild(enemyMarkSpriteGroup);

		//build enemy mark
		initEnemyMark();

		//build tooltip
		tooltipWindow = new Sprite(am["tooltip"]);
		tooltipWindow->zIndex = 10;
		tooltipWindow->size.x *= 2;
		tooltipText = new Label("global", 12, "ww", { 255,255,255,255 });
		tooltipText->position = { 10,8 };
		tooltipWindow->addChild(tooltipText);
		tooltipWindow->visible = false;
		fgGroup->addChild(tooltipWindow);

		//building skills
		initSkill();

		//init wave indicator
		waveIndicator = new Actor();
		waveIndicator->position = VecI(350, 30);
		fgGroup->addChild(waveIndicator);

		waveLabel = new Label("global", 16, "Interlude | Wave 1/" + to_string(lvl.waves.size()), {0,0,0,255});
		waveIndicator->addChild(waveLabel);

		waveBar1 = new Sprite(am["hpbar1"]);
		waveBar1->position = VecI(0,20);
		waveBar1->size.x = WAVE_BAR_LENGTH;
		waveIndicator->addChild(waveBar1);

		waveBar2 = new Sprite(am["hpbar2"]);
		waveBar2->position = VecI(0, 20);
		waveBar2->size.x = WAVE_BAR_LENGTH;
		waveIndicator->addChild(waveBar2);

		//must put last. global mask
		globalMask = new Sprite(am["globalmask"]);
		globalMask->visible = false;
		globalMask->setClick([=]() {
			assert(selectedSkill != 0);
			if (selectedSkill == 1) {
				//fireball
				skillLeft = 60;
				skillPos = getMousePosition() - bgGroup->position;
				fireballTime = FIREBALL_TIME;
			}
			else if (selectedSkill == 2) {
				//reinforce
				skillPos = getMousePosition() - bgGroup->position;
				if (!fine(skillPos)) {
					tooltipText->text = "Invalid position!";
					tooltipText->markDirty();
					return;
				}

				//COPIED FROM TOWERS. CHANGE BOTH REFERENCE!!
				for (int i = 0; i < 3; i++) {
					//TODO Farmer
					auto newSoldier = new Sprite(am.animation("farmer_w", 1, 2), 1e9);

					auto targetPos = skillPos + VecI(randInt(-20, 20), randInt(10, 20));
					newSoldier->position = targetPos - VecI(0, 50);
					newSoldier->color.a = 0;

					newSoldier->addChild(generateHpBar());
					projectileSpriteGroup->addChild(newSoldier);

					auto soldier = Soldier();
					soldier.locator = newSoldier;
					soldier.maxhp = soldier.hp = SOLDIER_HP/2;
					soldier.state = SOLDIER_IDLE;
					soldier.assemblyPoint = skillPos;
					soldier.atk = 50;
					soldier.from = -1;
					soldier.id = soldiers.size();
					soldiers.push_back(soldier);

					int id = soldiers.size() - 1;

					actions.add(aseq({ apara({ aalpha(newSoldier,30,255),amove(newSoldier,30,targetPos) }) }));
				}

				reinforceTime = REINFORCE_TIME;
			}

			tooltipWindow->visible = false;
			globalMask->visible = false;

		});
		bgGroup->addChild(globalMask);
	}

	void tick() override {
		tickWave();

		tickEnemy();

		tickSoldier();

		tickSkill();
		//ticking towers
		for (auto x : towerInstances) {
			if (x != NULL) {
				x->tick();
			}
		}

		//setup health
		heartDisplay->text = to_string(playerHp);
		goldDisplay->text = to_string(playerGold);
		heartDisplay->markDirty();
		goldDisplay->markDirty();

		//move tooltip
		tooltipWindow->position = getMousePosition() + VecI(15,15);

		//move map with mouse
		const int SPEED = 9;
		auto mouse = getMousePosition();
		if (mouse.x >= SCREEN_WIDTH - 50) {
			if (bgImg->size.x + bgGroup->position.x - SPEED> SCREEN_WIDTH) {
				bgGroup->position.x-=SPEED;
			}
		}
		if (mouse.x <= 50) {
			if (bgGroup->position.x<0) {
				bgGroup->position.x += SPEED;
			}
		}
		if (mouse.y >= SCREEN_HEIGHT - 50) {
			if (bgImg->size.y + bgGroup->position.y - SPEED> SCREEN_HEIGHT) {
				bgGroup->position.y -= SPEED;
			}
		}
		if (mouse.y <= 50) {
			if (bgGroup->position.y < 0) {
				bgGroup->position.y += SPEED;
			}
		}
	}

	void returned() override {
		
	}

	void back() override {

	}

	void forward() override {
		
	}
};