// ==================== 暗黑地牢 ==================== //
#include <ctime>
#include <cstdlib>
#include <iostream>
#define random(x) rand()%x   //产生随机数, 随机数范围在0~x间
using namespace std;
// ==================== 全局变量 ==================== //
int FLOOR           = 1;     // 当前探索层数
int MAX_SEARCH_TURN = 10;    // 最大探索次数
int SEARCH_TURNS    = 0;     // 当前探索层数
int FATE_LIST[25]   = {      // 命运列表，数字含义如下:
	1, 1, 1, 1, 2,           // 1.战斗    
	1, 1, 3, 1, 1,           // 2.治疗    
	1, 3, 1, 1, 1,           // 3.陷阱
	1, 1, 1, 1, 4,           // 4.加强(随机)
	1, 1, 1, 1, 1
};
// ==================== 工具函数 ==================== //
int DamageCounter(int Min, int Max) {
	// 用于计算伤害值(由于单一任务原则，只计算伤害值)
	cout << rand() % (Max - Min + 1) + Min << endl;
	return rand() % (Max - Min + 1) + Min;
}

// ==================== 测试函数 ==================== //
void TestFunction() {
	DamageCounter(40, 50);
}

// 主函数
int main() {
	srand((unsigned)time(NULL));
	//主函数
	system("title 暗黑地牢");
    cout << "暗黑地牢 V1.0\n";
	TestFunction();
	return 520;
}
