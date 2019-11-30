#include "box.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
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

void moveBox(int stage[][WIDTH], std::vector<SQUARE> current_pos, std::vector<SQUARE> next) {
	for (int cnt_box = 0; cnt_box < NUMBER_OF_BOX; cnt_box++) {
		//現在地が荷物
		if (stage[current_pos[cnt_box].y][current_pos[cnt_box].x] == BOX) {
			if (stage[next[cnt_box].y][next[cnt_box].x] == PATH) {
				//移動先が空き
				stage[next[cnt_box].y][next[cnt_box].x] = BOX;
				//移動前に居た場所を空きに
				stage[current_pos[cnt_box].y][current_pos[cnt_box].x] = PATH;
			}
			else if (stage[next[cnt_box].y][next[cnt_box].x] == GOAL) {
				//移動先がゴール
				stage[next[cnt_box].y][next[cnt_box].x] = BOX_ON_GOAL;
				//移動前に居た場所を空きに
				stage[current_pos[cnt_box].y][current_pos[cnt_box].x] = PATH;
			}
			else if (stage[next[cnt_box].y][next[cnt_box].x] == BOX) {
				continue;
			}
			else {
				std::cout << "error" << std::endl;
				std::cout << stage[next[cnt_box].y][next[cnt_box].x] << "," << next[cnt_box].x << "," << next[cnt_box].y << std::endl;
			}
		}
		else if (stage[current_pos[cnt_box].y][current_pos[cnt_box].x] == BOX_ON_GOAL) {
			if (stage[next[cnt_box].y][next[cnt_box].x] == PATH) {
				//移動先が空き
				stage[next[cnt_box].y][next[cnt_box].x] = BOX;
				//移動前に居た場所を空きに
				stage[current_pos[cnt_box].y][current_pos[cnt_box].x] = GOAL;
			}
			else if (stage[next[cnt_box].y][next[cnt_box].x] == GOAL) {
				//移動先がゴール
				stage[next[cnt_box].y][next[cnt_box].x] = BOX_ON_GOAL;
				//移動前に居た場所を空きに
				stage[current_pos[cnt_box].y][current_pos[cnt_box].x] = GOAL;
			}
			else if (stage[next[cnt_box].y][next[cnt_box].x] == BOX) {
				continue;
			}
			else {
				std::cout << "error" << std::endl;
				std::cout << stage[next[cnt_box].y][next[cnt_box].x] << "," << next[cnt_box].x << "," << next[cnt_box].y << std::endl;
			}
		}
	}
}
