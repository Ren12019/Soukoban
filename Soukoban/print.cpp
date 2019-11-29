#include "print.h"
#include <stdio.h>
#include "define.h"

void printStage(int stage[][WIDTH]) {
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (stage[y][x] == WALL)
				printf("#");
			else if (stage[y][x] == MAN)
				printf("@");
			else if (stage[y][x] == MAN_ON_GOAL)
				printf("+");
			else if (stage[y][x] == BOX)
				printf("$");
			else if (stage[y][x] == BOX_ON_GOAL)
				printf("*");
			else if (stage[y][x] == GOAL)
				printf(".");
			else if (stage[y][x] == PATH)
				printf(" ");
			else if (stage[y][x] == CHECK)
				printf("x");
		}
		printf("\n");
	}
	printf("\n");
}

void printGrid(int grid[][GRID_SIZE]) {
	for (int y = 0; y < GRID_SIZE; y++) {
		for (int x = 0; x < GRID_SIZE; x++) {
			if (grid[y][x] == WALL)
				printf("##");
			else if (grid[y][x] == MAN)
				printf("P ");
			else if (grid[y][x] == BOX)
				printf("■");
			else if (grid[y][x] == GOAL)
				printf("○");
			else if (grid[y][x] == PATH)
				printf("・");
		}
		printf("\n");
	}
	printf("\n");
}

void printStageList() {
	STAGELIST *nowStageList = head;

	if ((head == NULL) && (tail == NULL))
	{
		printf("リストは空です。\n");
	}

	while (true)
	{
		printStage(nowStageList->stage);

		if (nowStageList->next_stage == NULL)
			break;
		else
			nowStageList = nowStageList->next_stage;
	}
}

void printOutText(int stage[][WIDTH]) {
	FILE *fp;

	if (fopen_s(&fp, "StageData/stage.txt", "w") != 0) {
		printf("ファイルを開けませんでした。\n");
		return;
	}

	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (stage[y][x] == WALL)
				fprintf(fp,"#");
			else if (stage[y][x] == MAN)
				fprintf(fp, "@");
			else if (stage[y][x] == MAN_ON_GOAL)
				fprintf(fp, "+");
			else if (stage[y][x] == BOX)
				fprintf(fp, "$");
			else if (stage[y][x] == BOX_ON_GOAL)
				fprintf(fp, "*");
			else if (stage[y][x] == GOAL)
				fprintf(fp, ".");
			else if (stage[y][x] == PATH)
				fprintf(fp, " ");
		}
		fprintf(fp, "\n");
	}

	fclose(fp);
}