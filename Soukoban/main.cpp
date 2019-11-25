#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
	int check_list[HEIGHT][WIDTH] = {};

	for (int i = 0; i < 1; i++) {
		do {
			createStageGrid(stage);

			while (checkTile(stage) != 0) {}

		} while (checkSection(stage) == 1 && countPath(stage) > 8);

		initGoal(stage);
		initBox(stage);
		initPlayer(stage);

		printStage(stage);

		//printOutText(stage);

		//addStageList(stage);
	}
	//printStageList();

	return 0;
}