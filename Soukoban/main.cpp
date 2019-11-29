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
	std::vector<SQUARE> movable_pos(NUMBER_OF_BOX);//û”[—p
	std::vector<SQUARE> current_pos(NUMBER_OF_BOX);//‰×•¨‚ÌŒ»İÀ•W

	//—š—ğì¬
	SQUARE set = { 1,2 };
	setStageTest(stage,set);
	addStageList(stage);
	set.x = 3;
	set.y = 6;
	setStageTest(stage, set);
	addStageList(stage);
	printStageList();
	set.x = 4;
	set.y = 1;
	current_pos[0].x = 4;
	current_pos[0].y = 1;
	setStageTest(stage, set);
	addStageList(stage);
	printStageList();

	movable_pos[0].x = 3;
	movable_pos[0].y = 6;
	if (checkStageList(stage, current_pos, movable_pos)) {
		std::cout << "—š—ğ‚É‘¶İ‚·‚é‚½‚ßˆÚ“®‚Å‚«‚Ü‚¹‚ñ" << std::endl;
	}
	else {
		std::cout << "ˆÚ“®‚µ‚Ü‚µ‚½" << std::endl;
		setStageTest(stage, movable_pos[0]);
		addStageList(stage);
		printStageList();
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