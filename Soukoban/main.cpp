#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <conio.h>
#include <windows.h>

#include "define.h"
#include "check.h"
#include "grid.h"
#include "stage.h"
#include "print.h"
#include "goal.h"
#include "box.h"
#include "count.h"
#include "player.h"
#include "search.h"

int main(void) {
	srand((unsigned int)time(NULL));

	int stage[HEIGHT][WIDTH] = {};
	int checklist[HEIGHT][WIDTH] = {};

	/*test*/
	std::vector<SQUARE> movable_pos(NUMBER_OF_BOX);//収納用
	std::vector<SQUARE> current_pos(NUMBER_OF_BOX);//荷物の現在座標

	//履歴作成
	SQUARE set = { 1,2 };
	setStageTest(stage,set);
	addStageList(stage);
	set.x = 3;
	set.y = 6;
	setStageTest(stage, set);
	addStageList(stage);
	set.x = 4;
	set.y = 1;
	current_pos[0].x = 4;
	current_pos[0].y = 1;
	setStageTest(stage, set);
	addStageList(stage);
	printStageList();

	movable_pos[0].x = 7;
	movable_pos[0].y = 5;
	if (checkStageList(stage, current_pos, movable_pos)) {
		std::cout << "履歴に存在するため移動できません" << std::endl;
	}
	else {
		std::cout << "移動しました" << std::endl;
		moveBox(stage,current_pos,movable_pos);
		addStageList(stage);
		printStageList();
	}
	if (checkClear(stage)) {
		std::cout << "クリア" << std::endl;
	}

	//while (true) {
	//	do {
	//		createStageGrid(stage);

	//		while (checkTile(stage) != 0) {}

	//	} while (checkSection(stage) == 1 && countSpace(stage) > 1);

	//	initGoal(stage);
	//	initBox(stage);
	//	//initPlayer(stage);

	//	printStage(stage);

	//	searchBreadthFirstTwo(stage);

	//	printf("Please push key\n");
	//	printf("CONTINUE:Space		FINISH:Esc\n");

	//	while (true) {
	//		if (_kbhit()) {
	//			if (_getch() == 0x1b) {//Esc
	//				return 0;
	//			}
	//			if (_getch() == 0x20) {//Space
	//				break;
	//			}
	//		}
	//	}

	//	//printOutText(stage);
	//	//addStageList(stage);
	//}
	////printStageList();

	return 0;
}