#include "LevelInfo.h"
#include "Towers.h"
#include "Utility.h"
#include <string>
#include <cassert>
using namespace std;

int currentLevel = 0;
int currentDifficulty = 0;
int currentHero = 0;

LevelInfo levels[LEVEL_COUNT];
//sample towers
Tower* towers[TOWER_COUNT];
HeroData heros[HERO_COUNT];
function<Tower* ()> towersCreateFunction[TOWER_COUNT];

string difficultyString(int a) {
	switch (a) {
	case 0:
		return "Easy";
	case 1:
		return "Normal";
	case 2:
		return "Hard";
	case 3:
		return "Lunatic";
	default:
		assert(false);
	}
}

int difficultySelect(int e, int n, int h, int l)
{
	switch (currentDifficulty) {
	case 0:
		return e;
	case 1:
		return n;
	case 2:
		return h;
	case 3:
		return l;
	default:
		assert(false);
	}
}


void loadLevelInfos() {

	//load hero
	heros[0] = HeroData("momiji","Momiji",45,550,3,3);
	heros[1] = HeroData("nitori", "Nitori", 15, 250, 3, 3);

	//load towers
	towers[0] = new SoldierTower();
	towersCreateFunction[0] = []() {return new SoldierTower(); };
	towers[1] = new ArcherTower();
	towersCreateFunction[1] = []() {return new ArcherTower(); };
	towers[2] = new BomberTower();
	towersCreateFunction[2] = []() {return new BomberTower(); };
	towers[3] = new MageTower();
	towersCreateFunction[3] = []() {return new MageTower(); };

	//load level 1
	{
		auto goblin = EnemyData("goblin", "Goblin", 1, 10, 0, 60, 30, 0, 100, true, 2, 3, false);
		auto wolf = EnemyData("wolf", "Wolf", 2, 8, 2, 40, 30, 0, 80, true, 2, 2, true);
		levels[0].enemies["gb"] = goblin;
		levels[0].enemies["wf"] = wolf;
		levels[0].desc = { "Enemies are attacking the kingdom in all directions!","Use hero and towers to protect yourself!","Time to get familiar with the towers!!" };
		levels[0].background = "level0";
		levels[0].deployPosition = { {272,363},{164,417},{73,570},{84,664},{104,778},{149,867},{1296,657},{1128,651},{964,658},{821,656},{694,653},{515,617},{621,485},{679,338}, };
		levels[0].waves.resize(6);
		for (int i = 0; i < 3; i++) {
			levels[0].waves[i].length = 60 * 30;
			levels[0].waves[i].enemies = repeat(string("gb"), (i + 1) * 5);
			levels[0].waves[i].delay = 60 - 20 * i;
		}
		levels[0].waves[3].length = 60 * 30;
		levels[0].waves[3].enemies = repeat("wf", 5);
		levels[0].waves[3].delay = 60;
		levels[0].waves[4].length = 60 * 30;
		levels[0].waves[4].enemies = repeat("gb", 10) + repeat("wf", 5);
		levels[0].waves[4].delay = 30;
		levels[0].waves[5].length = 60 * 30;
		levels[0].waves[5].enemies = repeatv({ "gb","wf" }, 15);
		levels[0].waves[5].delay = 20;

		levels[0].enemyMarks = { {515,42} };
		levels[0].path[0] = { {429,12},{419,351},{331,441},{254,490},{172,576},{166,720},{207,812},{314,868},{467,885},{1435,895}, };
		levels[0].path[1] = { {514,13},{513,370},{290,572},{281,719},{533,811},{1383,807}, };
		levels[0].path[2] = { {601,14},{594,400},{403,547},{399,689},{511,762},{1435,742}, };
		levels[0].obstacles = { {{0,0},{423,291}},{{618,118},{1435,272}},{{1028,611},{1412,241}} };
	}
	//load level 2
	{
		levels[1].desc = { "More monsters are joining in this battle!","Moreover, they have chosen a more complex terrain.","Be careful! Some enemies may ambush!","BTW you will not know where they are coming..","Show your wisdom to the king!" };
		levels[1].background = "level1";

		auto goblin = EnemyData("goblin", "Goblin", 1, 10, 0, 60, 25, 0, 100, true, 2, 3, false);
		auto goblin2 = EnemyData("goblin", "Goblin", 1, 10, 0, 60, 25, 3, 100, true, 2, 3, false);
		auto goblin3 = EnemyData("goblin", "Goblin", 1, 10, 0, 60, 25, 8, 100, true, 2, 3, false);
		auto wolf = EnemyData("wolf", "Wolf", 2, 8, 2, 40, 30, 0, 80, true, 2, 2, true);
		auto wolf2 = EnemyData("wolf", "Wolf", 2, 8, 2, 40, 30, 3, 80, true, 2, 2, true);
		auto wolf3 = EnemyData("wolf", "Wolf", 2, 8, 2, 40, 30, 8, 80, true, 2, 2, true);
		auto dk = EnemyData("dk", "Dark Knight", 0.8, 20, 50, 60, 40, 8, 150, true, 2, 2, false);
		auto ske = EnemyData("ske", "Skeleton", 1, 25, -10, 90, 15, 6, 20, false, 2,5, false);
		auto ske2 = EnemyData("ske", "Skeleton", 1, 25, -10, 90, 15, 7, 20, false, 2, 5, false);
		auto ghost = EnemyData("ghost", "Ghost", 1.5, 0, 0, 0, 35, 8, 75, true, 4, 2, false);
		ghost.blockable = false;

		levels[1].enemies["gb1"] = goblin;
		levels[1].enemies["gb2"] = goblin2;
		levels[1].enemies["gb3"] = goblin3;
		levels[1].enemies["w1"] = wolf;
		levels[1].enemies["w2"] = wolf2;
		levels[1].enemies["w3"] = wolf3;
		levels[1].enemies["s1"] = ske;
		levels[1].enemies["s2"] = ske2;
		levels[1].enemies["dk"] = dk;
		levels[1].enemies["g"] = ghost;

		levels[1].waves.resize(12);

		levels[1].waves[0].delay = 20;
		levels[1].waves[0].length = 60 * 30;
		levels[1].waves[0].enemies = repeat("gb1", 5);

		levels[1].waves[1].delay = 15;
		levels[1].waves[1].length = 60 * 30;
		levels[1].waves[1].enemies = repeatv({"gb1","gb2"}, 5);

		levels[1].waves[2].delay = 10;
		levels[1].waves[2].length = 60 * 30;
		levels[1].waves[2].enemies = repeatv({ "w1","gb1","w2","gb2"}, 2);

		levels[1].waves[3].delay = 15;
		levels[1].waves[3].length = 60 * 30;
		levels[1].waves[3].enemies = repeatv({ "gb3","w3"}, 5);

		levels[1].waves[4].delay = 15;
		levels[1].waves[4].length = 60 * 30;
		levels[1].waves[4].enemies = repeatv({ "w1","w2","w3"}, 5);

		levels[1].waves[5].delay = 5;
		levels[1].waves[5].length = 60 * 30;
		levels[1].waves[5].enemies = repeatv({ "s1","s2"}, 15);

		levels[1].waves[6].delay = 20;
		levels[1].waves[6].length = 60 * 30;
		levels[1].waves[6].enemies = repeat("dk",1)+repeatv({"gb1","w2"}, 3);

		levels[1].waves[7].delay = 10;
		levels[1].waves[7].length = 60 * 30;
		levels[1].waves[7].enemies = repeat("w3", 5) + repeat("gb3", 5) + repeat("dk", 3);

		levels[1].waves[8].delay = 10;
		levels[1].waves[8].length = 60 * 30;
		levels[1].waves[8].enemies = repeatv({ "dk","s1","s2" }, 5);

		levels[1].waves[9].delay = 20;
		levels[1].waves[9].length = 60 * 30;
		levels[1].waves[9].enemies = repeatv({ "g","w3" }, 5);

		levels[1].waves[10].delay = 7;
		levels[1].waves[10].length = 60 * 30;
		levels[1].waves[10].enemies = repeatv({ "g","gb1","gb3","w2","dk","w3" }, 4);

		levels[1].waves[11].delay = 5;
		levels[1].waves[11].length = 60 * 30;
		levels[1].waves[11].enemies = repeatv({ "g","gb1","gb2","gb3","w2","w1","dk","w3","s1","s2","s1","s2","s2"}, 4);

		levels[1].deployPosition = { {304,103},{158,191},{661,162},{548,221},{384,295},{420,390},{573,399},{711,454},{806,513},{1207,450},{1240,569},{1257,737},{1058,593},{918,665},{911,760},{907,874},{1116,682},{1109,794},{1125,911},{612,931},{612,844},{610,746},{546,670},{458,603},{380,613},{388,733},{381,848},{140,582},{134,698},{138,816}, };
		levels[1].path[0] = { {517,23},{509,89},{388,147},{296,178},{218,225},{189,325},{185,440},{199,562},{196,663},{194,732},{193,825},{196,900}, };
		levels[1].path[1] = { {587,9},{565,83},{275,251},{252,427},{248,941}, };
		levels[1].path[2] = { {637,10},{636,83},{511,173},{330,228},{318,344},{317,467},{325,938}, };
		levels[1].path[3] = { {523,7},{510,98},{185,270},{174,480},{376,552},{502,554},{613,610},{682,709},{685,818},{688,932} };
		levels[1].path[4] = { {585,5},{572,99},{244,270},{240,466},{552,519},{755,669},{763,932}, };
		levels[1].path[5] = { {649,8},{634,83},{310,251},{311,427},{465,467},{664,493},{800,589},{848,941}, };
		levels[1].path[6] = { {1067,361},{1063,466},{1145,483},{1195,946}, }; //special path 1
		levels[1].path[7] = { { 1065,367 }, { 1060,468 }, { 961,495 }, { 822,621 }, { 819,941 } };  //special path 2
		levels[1].path[8] = { {24,411},{156,403},{359,559},{515,561},{684,700},{681,940}, };
		levels[1].path[9] = { {4,345},{152,337},{250,336},{264,492},{539,507},{766,714},{774,935}, };
		levels[1].path[10] = { {6,276},{160,270},{345,451},{635,465},{792,600},{819,945}, };
		levels[1].enemyMarks = { {587,9},{4,345}, {1067,361} };
		levels[1].obstacles = { {{832,2},{1437,281}},{{981,182},{741,400}} };
	}

	//load level 3
	{
		levels[2].desc = { "This is a boss level.","Boss may have extra skills, be careful!"};
		levels[2].background = "level2";

		auto goblin = EnemyData("goblin", "Goblin", 1, 10, 0, 60, 25, 0, 100, true, 2, 3, false);
		auto goblin2 = EnemyData("goblin", "Goblin", 1, 10, 0, 60, 25, 3, 100, true, 2, 3, false);
		auto wolf = EnemyData("wolf", "Wolf", 2, 8, 2, 40, 30, 0, 80, true, 2, 2, true);
		auto wolf2 = EnemyData("wolf", "Wolf", 2, 8, 2, 40, 30, 3, 80, true, 2, 2, true);
		auto dk = EnemyData("dk", "Dark Knight", 0.8, 20, 50, 60, 40, 0, 150, true, 2, 2, false);
		auto dk2 = EnemyData("dk", "Dark Knight", 0.8, 20, 50, 60, 40, 3, 150, true, 2, 2, false);
		auto ske = EnemyData("ske", "Skeleton", 1, 25, -10, 90, 15, 6, 20, false, 2, 5, false);
		auto ghost = EnemyData("ghost", "Ghost", 1.5, 0, 0, 0, 35, 6, 75, false, 4, 2, false);
		auto kitsune = EnemyData("fox", "Kitsune", 0.5, 120, 25, 60, 1000, 7, 10000, false, 3, 2, true);
		ghost.blockable = false;
		kitsune.boss = true;

		levels[2].enemies["gb"] = goblin;
		levels[2].enemies["gb2"] = goblin2;
		levels[2].enemies["w"] = wolf;
		levels[2].enemies["w2"] = wolf2;
		levels[2].enemies["dk"] = dk;
		levels[2].enemies["dk2"] = dk2;
		levels[2].enemies["s"] = ske;
		levels[2].enemies["g"] = ghost;
		levels[2].enemies["k"] = kitsune;
		levels[2].waves.resize(15);

		levels[2].waves[0].length = 60 * 30;
		levels[2].waves[0].delay = 20;
		levels[2].waves[0].enemies = repeat("gb", 5);

		levels[2].waves[1].length = 60 * 15;
		levels[2].waves[1].delay = 20;
		levels[2].waves[1].enemies = repeatv({ "gb","gb2" }, 3);

		levels[2].waves[2].length = 60*20;
		levels[2].waves[2].delay = 20;
		levels[2].waves[2].enemies = { "k" };

		levels[2].waves[3].length = 60 * 5;
		levels[2].waves[3].delay = 20;
		levels[2].waves[3].enemies = repeatv({ "w","w2","gb"}, 4);

		levels[2].waves[4].length = 60 * 20;
		levels[2].waves[4].delay = 20;
		levels[2].waves[4].enemies = repeatv({"gb","gb2","w","w2"}, 3);

		levels[2].waves[5].length = 60 * 30;
		levels[2].waves[5].delay = 20;
		levels[2].waves[5].enemies = repeatv({ "dk","gb","gb2"}, 3);

		levels[2].waves[6].length = 60 * 20;
		levels[2].waves[6].delay = 20;
		levels[2].waves[6].enemies = repeatv({ "dk2","w","gb" }, 3);

		levels[2].waves[7].length = 60 * 20;
		levels[2].waves[7].delay = 5;
		levels[2].waves[7].enemies = repeatv({ "s"}, 15);

		levels[2].waves[8].length = 60 * 20;
		levels[2].waves[8].delay = 5;
		levels[2].waves[8].enemies = repeatv({ "s","s","gb"}, 5);

		levels[2].waves[9].length = 60 * 20;
		levels[2].waves[9].delay = 5;
		levels[2].waves[9].enemies = repeatv({ "dk2","s","s","s","w2"}, 5);

		levels[2].waves[10].length = 60 * 30;
		levels[2].waves[10].delay = 5;
		levels[2].waves[10].enemies = repeatv({ "g","g","dk"}, 4);

		levels[2].waves[11].length = 60 * 30;
		levels[2].waves[11].delay = 5;
		levels[2].waves[11].enemies = repeatv({ "dk","dk2"}, 5);

		levels[2].waves[12].length = 60 * 30;
		levels[2].waves[12].delay = 5;
		levels[2].waves[12].enemies = repeatv({ "g","s","g","s","gb"}, 5);

		levels[2].waves[13].length = 60 * 40;
		levels[2].waves[13].delay = 20;
		levels[2].waves[13].enemies = repeatv({ "gb","w","gb2","w","w2","dk","dk2","w2"}, 5);

		levels[2].waves[14].length = 60 * 60;
		levels[2].waves[14].delay = 10;
		levels[2].waves[14].enemies = repeatv({ "gb","s","s","s","w","gb2","w","g","g","w2","dk","dk2","g","w2" }, 4);

		levels[2].deployPosition = { {629,224},{508,237},{378,263},{302,386},{307,510},{314,643},{397,756},{553,785},{880,811},{989,806},{1132,762},{1190,664},{1197,551},{1138,291},{1012,255},{814,234},{730,455},{858,450},{985,550},{868,662},{728,653},{586,610},{519,514},{619,402}, };

		levels[2].path[0] = { {705,11},{700,302},{482,305},{366,480},{364,619},{419,685},{494,713},{583,736},{651,743},{687,766},{690,939},};
		levels[2].path[1] = { {727,9},{720,328},{490,340},{439,407},{417,623},{476,686},{562,719},{679,728},{736,743},{747,943}, };
		levels[2].path[2] = { {752,14},{757,360},{498,369},{452,461},{451,616},{512,674},{669,703},{798,712},{804,938}, };
		levels[2].path[3] = { {697,7},{695,297},{959,303},{1126,387},{1130,604},{990,753},{800,752},{804,939}, };
		levels[2].path[4] = { {726,7},{723,328},{978,354},{1059,388},{1093,518},{1089,630},{1013,712},{936,735},{819,737},{750,738},{742,937}, };
		levels[2].path[5] = { {761,7},{754,365},{956,365},{1025,390},{1056,431},{1066,469},{1056,631},{1045,671},{985,705},{887,715},{792,712},{680,775},{680,943}, };
		levels[2].path[6] = { {1229,330},{1225,400},{1093,456},{1078,644},{1028,702},{953,732},{764,735},{736,934}, }; //special path
		//boss path
		levels[2].path[7] = { {724,6},{730,341},{484,344},{428,396},{407,513},{404,621},{437,673},{598,721},{695,733},{775,729},{921,736},{1036,700},{1081,612},{1087,503},{1068,421},{1021,352},{872,333},{742,334},{643,334},{530,335},{467,372},{443,445},{418,533},{421,621},{446,678},{560,721},{657,724},{781,744},{1019,709},{1080,601},{1105,489},{1063,363},{939,344},{812,327},{710,337},{568,314},{442,376},{413,440},{409,550},{412,639},{485,683},{572,708},{711,734},{841,735},{969,728},{1062,690},{1073,579},{1081,436},{1034,359},{937,336},{782,330},{655,324},{546,333},{451,362},{425,426},{407,527},{413,611},{457,672},{531,696},{655,719},{732,740},{752,790},{749,854},{756,931},{743,941}, };
		
		levels[2].obstacles = { {{1196,333},{1437,3}},{{174,5},{0,944}} };
		levels[2].enemyMarks = { {727,9},{1229,330} };
	}
}