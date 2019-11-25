#include "box.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "define.h"
#include "check.h"
#include "choice.h"
#include "count.h"

void initBox(int stage[][WIDTH]) {
	clock_t t = clock();
	int x = 0, y = 0;
	int checklist[HEIGHT][WIDTH] = {};

	for (int num_box = NUMBER_OF_BOX; num_box != 0; num_box--) {
		checkPutBox(stage, checklist);

		while (true) {
			x = choiceX();
			y = choiceY();
			if ((stage[y][x] == PATH || stage[y][x] == GOAL )&& checklist[y][x] != CHECK) {
				if (checkDeadlock(stage,x,y)) {
					checklist[y][x] = CHECK;
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

			if (checkChecklist(checklist)) {
				printf("Failed to create stage:Not Find Space\n");
				return;
			}
			if (clock() - t > 2000) {
				printf("Failed to create stage:Time Out\n");
				return;
			}
		}
	}
}