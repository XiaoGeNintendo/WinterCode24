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
using namespace std;

#define pii pair<int,int>
#define distType pair<long long,pair<int,int>>

class GameScene : public Scene {
private:
	void tickEnemy() {

		//ticking enemies
		for (int i = 0; i < enemyInstances.size(); i++) {
			EnemyInstance& enemy = enemyInstances[i];

			if (enemy.noProcess) {
				continue;
			}

			//check dead first
			if (enemy.hp <= 0) {
				//TODO Enemy dead

				playerGold += enemy.data->dropCoin;
				enemySprites[enemy.id]->removeFromParent();
				delete enemySprites[enemy.id];
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

			//behaviour
			if (enemy.state == ENEMY_WALKING) {
				//calculate speed
				enemy.speed = i2d(enemy.path[enemy.nextPoint] - enemy.path[enemy.nextPoint - 1]).normalize() * enemy.data->speed;
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
					printf("Soldier %d hp - %d = %d\n", enemy.fighting[0], enemy.data->attack, atk.hp);
				}
			}

			//linking sprites
			auto sp = enemySprites[enemy.id];
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

			if (soldier.noProcess) {
				continue;
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
					auto path = pathfind(soldier.locator->position, d2i(enemy.position) - VecI(20 + rand() % 5, rand() % 10), false);
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
					printf("Enemy %d hp - %d = %d\n", soldier.fighting, soldier.atk - atk.data->defense, atk.hp);
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
			if (soldier.state == SOLDIER_IDLE) {
				soldier.locator->setAnimation(am.animation("soldier_w", 1, 2), 1e9);
			}
			else if (soldier.state == SOLDIER_MOVING || soldier.state==SOLDIER_RETREATING) {
				soldier.locator->flipX = (soldier.steps.size() > 0 && soldier.steps.back().x > soldier.locator->position.x);
				soldier.locator->setAnimation(am.animation("soldier_w", 1, 2), 20);
			}
			else if (soldier.state == SOLDIER_FIGHTING) {
				soldier.locator->setAnimation(am.animation("soldier_a", 1, 2), 20);
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

	void tickWave() {

		nextWaveCountdown--;
		if (nextWaveCountdown == 0) {
			currentWave++;
			printf("Started wave %d\n", currentWave);
			nextEnemy = 0;
			nextWaveCountdown = currentWave == lvl.waves.size() ? 1e9 : lvl.waves[currentWave + 1].length;
			nextEnemyCountdown = lvl.waves[currentWave].delay;
		}
		nextEnemyCountdown--;
		if (nextEnemyCountdown == 0) {
			if (currentWave != lvl.waves.size() && nextEnemy != lvl.waves[currentWave].enemies.size()) {
				printf("Generating %s\n", lvl.waves[currentWave].enemies[nextEnemy].c_str());
				//generate enemy
				auto instance = EnemyInstance();
				instance.data = &lvl.enemies[lvl.waves[currentWave].enemies[nextEnemy]];
				instance.hp = instance.data->maxhp;
				instance.path = lvl.path[instance.data->randomPath ? instance.data->pathId + rand() % 3 : instance.data->pathId];
				instance.position = i2d(instance.path[0]);
				instance.nextPoint = 1;
				instance.state = ENEMY_WALKING;
				instance.id = enemyInstances.size();
				enemyInstances.push_back(instance);

				//generate sprite
				auto enemySprite = new Sprite(am.animation(instance.data->id + "_w", 1, instance.data->wac), 20);
				enemySprites[instance.id] = enemySprite;
				enemySpriteGroup->addChild(enemySprite);

				nextEnemy++;
				nextEnemyCountdown = lvl.waves[currentWave].delay;
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

					//set opening ui first
					this->openingUi = __opening;

					auto it = towerInstances[__opening];
					assert(it != NULL);

					if (it->id == "soldier") {
						upgradeIcons[2]->visible = true;
						for (int i = 0; i < 4; i++) {
							double angle = 2 * PI / 4 * i;
							double radius = 70;

							auto standbyPos = VecI(cos(angle) * radius, sin(angle) * radius);
							actions.add(amove(upgradeIcons[i], 30, { 0,0 }, standbyPos));
						}

					}
					else {
						for (int i = 0; i < 3; i++) {
							double angle = 2 * PI / 3 * i;
							double radius = 70;

							auto standbyPos = VecI(cos(angle) * radius, sin(angle) * radius);
							actions.add(amove(upgradeIcons[i], 30, { 0,0 }, standbyPos));
						}

						upgradeIcons[2]->visible = false;
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
		for (int i = 0; i < lvl.deployPosition.size(); i++) {
			deployer[i] = new Sprite(am["signpost"]);
			deployer[i]->pivot = { 0.5,1 };
			deployer[i]->position = lvl.deployPosition[i];
			deployer[i]->setClick([=]() {
				if (openingUi == i) {
					buildUi->visible=false;
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

				playerGold -= ti->upgradeCost[ti->level - 1];
				ti->level++;


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
							path = pathfind(s.locator->position, st->assemblyPosition + VecI(rand() % 30 - 15, rand() % 30 - 15), false);
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

	Actor* towerSpriteGroup;
	Actor* enemySpriteGroup;
	Actor* projectileSpriteGroup;

	Sprite* tooltipWindow;
	Label* tooltipText;

	Sprite* radiusCircle;
	Sprite* assemblyIndicator;
	Sprite* assemblyCircle;

	int playerHp = 20;
	int playerGold = 0;

	int currentWave;
	int nextEnemy;
	int nextEnemyCountdown,nextWaveCountdown;
	int globalEnemyId = 0;

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
		towerSpriteGroup = new Actor();
		bgGroup->addChild(towerSpriteGroup);

		enemySpriteGroup = new Actor();
		bgGroup->addChild(enemySpriteGroup);

		projectileSpriteGroup = new Actor();
		bgGroup->addChild(projectileSpriteGroup);

		//build tooltip
		tooltipWindow = new Sprite(am["tooltip"]);
		tooltipWindow->zIndex = 10;
		tooltipWindow->size.x *= 2;
		tooltipText = new Label("global", 12, "ww", { 255,255,255,255 });
		tooltipText->position = { 8,8 };
		tooltipWindow->addChild(tooltipText);
		tooltipWindow->visible = false;
		fgGroup->addChild(tooltipWindow);

	}

	void tick() override {
		tickWave();

		tickEnemy();

		tickSoldier();

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