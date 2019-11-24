#include "goal.h"
#include <stdlib.h>
#include "define.h"
#include "check.h"
#include "choice.h"
#include "count.h"

void initGoal(int stage[][WIDTH]) {
	int x = 0, y = 0;
	int check_list[HEIGHT][WIDTH] = {};

	checkCarryInArea(stage, check_list);

	for (int stage_size = countStageSize(stage); stage_size / RATE != 0; stage_size -= RATE){
		while (true) {
			x = choiceX();
			y = choiceY();
			if (stage[y][x] == PATH && check_list[y][x] != CHECK) {
				stage[y][x] = GOAL;
				break;
			}
		}
	}
}