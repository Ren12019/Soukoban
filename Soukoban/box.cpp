#include "box.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include "define.h"
#include "check.h"
#include "choice.h"
#include "count.h"
#include "print.h"

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

int moveBox(int stage[][WIDTH],const std::vector<SQUARE> current_pos,const std::vector<SQUARE> next) {
	//test
	//printf("1__C:(%d,%d),N:(%d,%d)\n", current_pos[0].x, current_pos[0].y, next[0].x, next[0].y);
	//printf("2__C:(%d,%d),N:(%d,%d)\n", current_pos[1].x, current_pos[1].y, next[1].x, next[1].y);

	//移動があるか
	for (int cnt_box = 0; cnt_box < NUMBER_OF_BOX; cnt_box++) {
		//座標確認
		if (current_pos[cnt_box].x != next[cnt_box].x || current_pos[cnt_box].y != next[cnt_box].y) {
			break;
		}
		//最後の荷物も移動なしなら
		if (cnt_box == NUMBER_OF_BOX - 1) {
			return 0;
		}
	}

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
				std::cout << "errorbox1" << std::endl;
				std::cout << stage[next[cnt_box].y][next[cnt_box].x] << "," << next[cnt_box].x << "," << next[cnt_box].y << std::endl;
				printStage(stage);

				return 0;
			}
		}
		else if (stage[current_pos[cnt_box].y][current_pos[cnt_box].x] == BOX_ON_GOAL) {
			continue;
			//if (stage[next[cnt_box].y][next[cnt_box].x] == PATH) {
			//	//移動先が空き
			//	stage[next[cnt_box].y][next[cnt_box].x] = BOX;
			//	//移動前に居た場所を空きに
			//	stage[current_pos[cnt_box].y][current_pos[cnt_box].x] = GOAL;
			//}
			//else if (stage[next[cnt_box].y][next[cnt_box].x] == GOAL) {
			//	//移動先がゴール
			//	stage[next[cnt_box].y][next[cnt_box].x] = BOX_ON_GOAL;
			//	//移動前に居た場所を空きに
			//	stage[current_pos[cnt_box].y][current_pos[cnt_box].x] = GOAL;
			//}
			//else if (stage[next[cnt_box].y][next[cnt_box].x] == BOX) {
			//	continue;
			//}
			//else {
			//	std::cout << "errorbox2" << std::endl;
			//	std::cout << stage[next[cnt_box].y][next[cnt_box].x] << "," << next[cnt_box].x << "," << next[cnt_box].y << std::endl;
			//	printStage(stage);
			//	return 0;
			//}
		}
	}

	return 1;
}
