// ==================== 暗黑地牢 V1.0 ==================== //
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <Windows.h>
#define random(x) rand()%x   //产生随机数, 随机数范围在0~x间
using namespace std;
// ==================== 全局变量 ==================== //
static int FLOOR           = 1;     // 当前探索层数
static int MAX_SEARCH_TURN = 10;    // 最大探索次数
static int SEARCH_TURNS    = 0;     // 当前探索层数
static int FATE_LIST[25]   = {      // 命运列表，数字含义如下:
	1, 1, 1, 1, 2,           // 1.战斗    
	1, 1, 3, 1, 1,           // 2.治疗    
	1, 3, 1, 1, 1,           // 3.陷阱
	1, 1, 1, 1, 4,           // 4.加强(随机)
	1, 1, 1, 1, 1
};
static char V[40] = { "暗黑地牢V0.5.1 beta" };
// ==================== 工具函数 ==================== //
/*
*功能：
	输出彩色字符串
*输入参数：
	color:颜色选择，1 red, 2 green, 3 yellow, default white
	str: 彩色字符串
*/
void printColor(int color, const char *str)
{
	
	HANDLE hd;

	hd = GetStdHandle(STD_OUTPUT_HANDLE);
	switch (color)
	{
	case 1: //red
		SetConsoleTextAttribute(hd,
			FOREGROUND_RED |
			FOREGROUND_INTENSITY);
		break;
	case 2: //green
		SetConsoleTextAttribute(hd,
			FOREGROUND_GREEN |
			FOREGROUND_INTENSITY);
		break;
	case 3: //yellow
		SetConsoleTextAttribute(hd,
			FOREGROUND_RED |
			FOREGROUND_GREEN |
			FOREGROUND_INTENSITY);
		break;
	case 4: //blue
		SetConsoleTextAttribute(hd,
			FOREGROUND_BLUE |
			FOREGROUND_INTENSITY);
		break;
	default: //white
		SetConsoleTextAttribute(hd,
			FOREGROUND_RED |
			FOREGROUND_GREEN |
			FOREGROUND_BLUE);
	}

	printf("%s", str);
	SetConsoleTextAttribute(hd,
		FOREGROUND_RED |
		FOREGROUND_GREEN |
		FOREGROUND_BLUE);
}
int DamageCounter(int Min, int Max) {
	// 用于计算伤害值(由于单一任务原则，只计算伤害值)
	return rand() % (Max - Min + 1) + Min;
}
bool HideCheck(double hideRate) {
	// 计算是否闪避并返回bool
	int hTurns = 0;
	int notH = 0;
	int tmp2 = 100 * hideRate;
	// 100次，相对平均
	for (int t = 0; t < 100; t++) {
		int tmp = random(100);
		if (tmp2 > tmp) {
			hTurns++;
			continue;
		}
		notH++;
	}
	
	return hTurns > notH ? true : false;
}
bool CritCheck(double critRate) {
	// 计算是否暴击并返回bool
	int cTurns = 0;
	int notC = 0;
	int tmp2 = 100 * critRate;
	// 100次，相对平均
	for (int t = 0; t < 100; t++) {
		
		int tmp = random(100);
		if (tmp2 > tmp) {
			cTurns++;
			continue;
		}
		notC++;
	}

	return cTurns > notC ? true : false;
}
// ==================== 角色类 ==================== //
struct Origin {
	// 各种角色结构体的基本结构体 (Russian Doll)
	char NAME[100] = { "角色基类" };      // 名称
	int MAX_HEALTH = 100;  // 最大生命值
	int HEALTH = 100;       // 生命值
	int MAX_MAGIC = 100;    // 最大魔法值
	int MAGIC = 100;        // 魔法值
	int MIN_ATK = 1;      // 最小攻击
	int MAX_ATK = 10;      // 最大攻击
	int MAGIC_ATK = 10;    // 魔攻
	int DEFENSE = 0;      // 防御
	int SHIELD = 0;       // 护盾
	int EXP = 0;          // 经验
	int UP_EXP = 6;       // 升级所需经验
	int LEVEL = 1;        // 等级
	int gEXP = 1;
	double HIDE_RATE = 0.01;  // 闪避率
	double CRIT_RATE = 0.99;  // 暴击率
	void Attack(Origin &who) {
	bool IsHide = HideCheck(who.HIDE_RATE);
	bool IsCrit = CritCheck(who.CRIT_RATE);
	int atk;
	if (IsCrit) {
		printColor(3, NAME);
		printColor(3, "暴击!\n");
		atk = DamageCounter(MIN_ATK, MAX_ATK) * 2;
	}
	else {
		atk = DamageCounter(MIN_ATK, MAX_ATK);
	}
	if (IsHide) {
		cout << who.NAME << "闪避了" << NAME << "的攻击!" << endl;
		return;
	}
	else {
		if (who.SHIELD > 0) {
			// 当攻击目标有护盾的时候的判断
			if (who.SHIELD >= atk) {
				// 护盾大于等于攻击时护盾减掉攻击
				who.SHIELD = who.SHIELD - atk;
			}
			else if (who.SHIELD < atk) {
				// 护盾小于攻击，但是有的时候
				// 先把护盾抵掉一部分攻击
				// 之后把生命值扣去剩下的攻击
				// 完事儿
				atk = atk - who.SHIELD;
				who.SHIELD = 0;
				who.SHIELD = who.SHIELD - atk;
			}
			else {
				// 纯属娱乐
				cout << "Fuck You, Look What You Doing?";
			}
		}
		else {
			who.HEALTH = who.HEALTH - atk;
		}
		char temp[1000];
		sprintf_s(temp, "%d", atk);
		printColor(1, NAME);
		printColor(1, "攻击了");
		printColor(1, who.NAME);
		printColor(1, "并造成了");
		printColor(1, temp);
		printColor(1, "点伤害!\n");
	}
	
	
}
	void Show() {
		HANDLE hd;
		hd = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hd,
			FOREGROUND_RED |
			FOREGROUND_GREEN |
			FOREGROUND_INTENSITY);
		cout << NAME << ":" << endl;
		cout << "生命:" << HEALTH << "/" << MAX_HEALTH;
		cout << "  魔法:" << MAGIC << "/" << MAX_MAGIC << endl;
		cout << "攻击:" << MIN_ATK << "~" << MAX_ATK;
		cout << "  魔攻:" << MAGIC_ATK << endl;
		cout << "防御:" << DEFENSE << "  护盾剩余血量:" << SHIELD << endl;
		cout << "闪避率" << HIDE_RATE*100 << "%" << "  暴击率:" << CRIT_RATE*100 << "%" <<  endl;
		cout << "等级:" << LEVEL << "  经验:" << EXP << "/" << UP_EXP << endl;
		SetConsoleTextAttribute(hd,
			FOREGROUND_RED |
			FOREGROUND_GREEN |
			FOREGROUND_BLUE);
	}
	void LevelUp() {};
};
namespace Skills {
	// 技能的命名空间
	void Siphon() {};
	void BigBang() {};
	void BreakOff() {};
	void MageAtk() {};
	void LifeBlow() {};
	void HeavyBlow() {};
	void BerserkerRoar() {};
	void WarriorHeart() {};
	void MagicBurst() {};
	void WaterFall() {};
	void Baptism() {};
	void Sublime() {};
	void SafeShield() {};
	void SoulTear() {};
	void MagicWave() {};
	void HeavyKill() {};
	void ShadowJump() {};
	void StrikeWave() {};
	void HealthJump() {};
	void OneEgg() {};
	void ExcitedSoul() {};
	void NormalAtk() {};
	void DrinkForget() {};
	void HitIt() {};
	void Rolling() {};
	void MachineHeart() {};
	void PowerRound() {};
	void PowerShield() {};
	void ShieldPower() {};
	void AbsoluteDefense() {};
	void PowerBuff() {};
	void TotallyAtk() {};
	void AtkUp() {};
	void BackMagic() {};
	void Mad() {};
	void WindyDance() {};
	void BloodFav() {};
	void DanceStyle() {};
	void Alchemy() {};
	void DuccccckGettttt() {};
	// 接下来是Boss技能
	void HitGround() {};
	void FireOn() {};
	void BANG() {};
	void NinjaWay() {};
	void GodRoll() {};
	void DM_Duang() {};
	void EyeLight() {};
	void ElementFist() {};
	void BloodRise() {};
	void NoDefense() {};
}
namespace Monsters {
	// 怪物的命名空间
	Origin JackalSoldier;
	Origin UndeadSkeleton;
	Origin DarkElement;
	Origin FlashElement;
	Origin DM200;
	Origin DM1500;
	Origin DM30000;
	Origin DM_MINI1080;
	Origin WM_1000ALPHA;
	Origin WM30000;
	Origin WM10086;
	Origin DarkBeliever;
	Origin Treeist;
	Origin Shaman;
	Origin WarriorSoul;
	Origin IronScavenger;
	Origin SwampFrog;
	Origin Soul;
	Origin IceDragon;
	Origin BlackDragon;
	Origin DragonFly;
	Origin EaterFlower;
	Origin DwarfBoomer;
	Origin LostGhost;
	Origin DungeonWarden;
	Origin StrangeNinja;
	Origin MachineKiller;
	Origin JackalMage;
	Origin HomelessRanger;
	Origin SkeletonKing;
	Origin ZombieWarrior;
	Origin GodGuard;


}
namespace Bosses {
	// 头目的命名空间
	Origin StoneGiant;
	Origin DarkWarden;
	Origin DarkGod;
	Origin AncientGod;
	Origin Kraken;
	Origin DM30000WAR;
	Origin WM30000WAR;
	Origin Tengu;
	Origin DeathHero;
	Origin ElementMaster;


}
namespace Chacters {
	// 角色的命名空间
	Origin Warrior;
	Origin Mage;
	Origin Ranger;
	Origin Robot;
	Origin Alchemist;


}


// ==================== 测试函数 ==================== //
void TestFunction() {
	Origin t1;
	Origin t2;
	t1.Attack(t2);
	t2.Show();
}

// 主函数
int main() {
	srand((unsigned)time(NULL));
	//主函数
	system("title 暗黑地牢V0.5.1beta");
	cout << V << endl << endl;
	TestFunction();
	return 520;
}
