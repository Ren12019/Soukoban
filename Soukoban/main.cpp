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
	clock_t t = clock();

	int stage[HEIGHT][WIDTH] = {};
	int checklist[HEIGHT][WIDTH] = {};

	while (true) {
		do {
			createStageGrid(stage);

			while (checkTile(stage) != 0) {}

		} while (checkSection(stage) == 1 && countSpace(stage) > 1);

		initGoal(stage);
		initBox(stage);
		//initPlayer(stage);

		printStage(stage);

		if (!searchBreadthFirstTwo(stage)) {
			head = NULL;
			tail = NULL;
			if (searchBreadthFirstTwo(stage)) {
				printf("CLRAR!\n");
			}
			else printf("NOT FOUND ROUTE");
		}
		else printf("CLRAR!\n");

		printf("Runtime:%dsec\n", (clock() - t)/1000);
		printf("Please push key\n");
		printf("CONTINUE:Space		FINISH:Esc\n");

		while (true) {
			if (_kbhit()) {
				if (_getch() == 0x1b) {//Esc
					return 0;
				}
				if (_getch() == 0x20) {//Space
					freeStageList();
					head = NULL;
					tail = NULL;
					t = clock();
					break;
				}
			}
		}

		//printOutText(stage);
		//addStageList(stage);
	}
	//printStageList();

	return 0;
}