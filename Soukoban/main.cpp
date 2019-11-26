#include <stdio.h>
#include <stdlib.h>
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

int main(void) {
	srand((unsigned int)time(NULL));

	int stage[HEIGHT][WIDTH] = {};
	int checklist[HEIGHT][WIDTH] = {};

	while (true) {
		do {
			createStageGrid(stage);

			while (checkTile(stage) != 0) {}

		} while (checkSection(stage) == 1 && countPath(stage) > 8);

		initGoal(stage);
		initBox(stage);
		initPlayer(stage);

		printStage(stage);

		printf("%d\n", countConnectionAll(stage));

		printf("Please push key\n");
		printf("CONTINUE:Space		FINISH:Esc\n");

		while (true) {
			if (_kbhit()) {
				if (_getch() == 0x1b) {//Esc
					return 0;
				}
				if (_getch() == 0x20) {//Space
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