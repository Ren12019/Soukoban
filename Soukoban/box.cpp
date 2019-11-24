#include "box.h"

#include <stdlib.h>
#include "define.h"
#include "check.h"
#include "choice.h"
#include "count.h"

void initBox(int stage[][WIDTH]) {
	int x = 0, y = 0;
	int check_list[HEIGHT][WIDTH] = {};

	for (int stage_size = countStageSize(stage); stage_size / RATE != 0; stage_size -= RATE) {
		checkPutBox(stage, check_list);

		while (true) {
			x = choiceX();
			y = choiceY();
			if ((stage[y][x] == PATH || stage[y][x] == GOAL )&& check_list[y][x] != CHECK) {
				if (checkDeadlock(stage,x,y)) {
					check_list[y][x] = CHECK;
				}
				else {
					if (stage[y][x] == PATH) {
						stage[y][x] = BOX;
						if (checkSection(stage))
						{
							stage[y][x] = PATH;
						}
						else {
							break;
						}
					}
					else if (stage[y][x] == GOAL) {
						stage[y][x] = BOX_ON_GOAL;
						if (checkSection(stage))
						{
							stage[y][x] = GOAL;
						}
						else {
							break;
						}
					}
				}
			}
		}
	}
}