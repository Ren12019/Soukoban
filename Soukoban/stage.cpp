#include "stage.h"
#include <stdlib.h>
#include "define.h"
#include "grid.h"
#include "print.h"

/*‹@”\‚ÌŽŽ‰^“]—p*/
void setStageTest(int stage[][WIDTH])
{
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (x == 0 || y == 0 || x == WIDTH - 1 || y == HEIGHT - 1) {
				stage[y][x] = WALL;
			}
			else if (x == 1 && y == 3) {
				stage[y][x]=BOX_ON_GOAL;
			}
			else if (x == 1 && y == 2) {
				stage[y][x] = BOX_ON_GOAL;
			}
			else {
				stage[y][x] = PATH;
			}
		}
	}
}

void set_x(int* point) {
	*point = rand() % (WIDTH - 2) + 1;
}

void set_y(int* point) {
	*point = rand() % (HEIGHT - 2) + 1;
}

void initStage(int stage[][WIDTH]) {
	int man_x, man_y;
	int box_x, box_y;
	int goal_x, goal_y;

	set_x(&man_x);
	set_y(&man_y);
	set_x(&box_x);
	set_y(&box_y);
	while (box_x == man_x && box_y == man_y) {
		set_x(&box_x);
		set_y(&box_y);
	}
	set_x(&goal_x);
	set_y(&goal_y);
	while ((goal_x == man_x && goal_y == man_y) || (goal_x == box_x && goal_y == box_y)) {
		set_x(&goal_x);
		set_y(&goal_y);
	}

	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (x == 0 || x == WIDTH - 1 || y == 0 || y == HEIGHT - 1)
				stage[y][x] = WALL;
			else if (x == man_x && y == man_y)
				stage[y][x] = MAN;
			else if (x == box_x && y == box_y)
				stage[y][x] = BOX;
			else if (x == goal_x && y == goal_y)
				stage[y][x] = GOAL;
			else
				stage[y][x] = PATH;
		}
	}
}

void createStageGrid(int stage[][WIDTH]) {
	int grid[GRID_SIZE][GRID_SIZE] = {};

	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
				stage[y][x] = WALL;
		}
	}

	for (int grid_num = 0, v = 0, h = 0; grid_num < (NUMBER_OF_GRID_VERTICAL * NUMBER_OF_GRID_HORIZONTAL); grid_num++) {
		v = grid_num / NUMBER_OF_GRID_HORIZONTAL;
		h = grid_num % NUMBER_OF_GRID_HORIZONTAL;
		//setGrid(grid);
		setGridTemplate(grid);

		for (int y = 0; y < GRID_SIZE; y++) {
			for (int x = 0; x < GRID_SIZE; x++) {
				stage[y + (v * GRID_SIZE) + 1][x + (h * GRID_SIZE) + 1] = grid[y][x];
			}
		}
	}
}

STAGELIST*createStageList(void) {
	STAGELIST*newStageList;

	newStageList = (STAGELIST*)malloc(sizeof(STAGELIST));

	return newStageList;
}

void addStageList(int stage[][WIDTH])
{
	STAGELIST *newStageList;

	newStageList = createStageList();

	for (int y = 0; y < HEIGHT; y++){
		for (int x = 0; x < WIDTH; x++){
			newStageList->stage[y][x] = stage[y][x];
		}
	}

	if ((head == NULL) && (tail == NULL)){
		head = newStageList;
		tail = newStageList;
	}
	else {
		tail->next_stage = newStageList;
		tail = newStageList;
	}

	newStageList->next_stage = NULL;
}
