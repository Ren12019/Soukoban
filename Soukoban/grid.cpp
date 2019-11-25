#include "grid.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "define.h"
#include "choice.h"
#include "print.h"

void rotateGrid(int grid[][GRID_SIZE]) {
	int direction = choiceDirection();
	int rotated_grid[GRID_SIZE][GRID_SIZE] = {};

	while (direction != FRONT) {
		for (int y = 0; y < GRID_SIZE; y++) {
			for (int x = 0; x < GRID_SIZE; x++) {
				if (y == 0) {
					if (x == 0) {
						rotated_grid[y][2] = grid[y][x];
					}
					else if (x == 1) {
						rotated_grid[1][2] = grid[y][x];
					}
					else if (x == 2) {
						rotated_grid[2][x] = grid[y][x];
					}
				}
				else if (y == 1) {
					if (x == 0) {
						rotated_grid[0][1] = grid[y][x];
					}
					else if (x == 1) {
						rotated_grid[y][x] = grid[y][x];
					}
					else if (x == 2) {
						rotated_grid[2][1] = grid[y][x];
					}
				}
				else if (y == 2) {
					if (x == 0) {
						rotated_grid[0][x] = grid[y][x];
					}
					else if (x == 1) {
						rotated_grid[1][0] = grid[y][x];
					}
					else if (x == 2) {
						rotated_grid[y][0] = grid[y][x];
					}
				}
			}
		}

		for (int y = 0; y < GRID_SIZE; y++) {
			for (int x = 0; x < GRID_SIZE; x++) {
				grid[y][x] = rotated_grid[y][x];
			}
		}

		direction--;
	}
}

void flipGrid(int grid[][GRID_SIZE]) {
	int flip = choiceFlip();
	int flipped_grid[GRID_SIZE][GRID_SIZE] = {};

	switch (flip) {
	case VERTICAL: {
		for (int y = 0; y < GRID_SIZE; y++) {
			for (int x = 0; x < GRID_SIZE; x++) {
				if (y == 0) {
					flipped_grid[GRID_SIZE - 1][x] = grid[y][x];
				}
				else if (y == GRID_SIZE - 1) {
					flipped_grid[0][x] = grid[y][x];
				}
				else
					flipped_grid[y][x] = grid[y][x];
			}
		}
	}
	case HORIZONTAL: {
		for (int y = 0; y < GRID_SIZE; y++) {
			for (int x = 0; x < GRID_SIZE; x++) {
				if (x == 0) {
					flipped_grid[y][GRID_SIZE - 1] = grid[y][x];
				}
				else if (x == GRID_SIZE - 1) {
					flipped_grid[y][0] = grid[y][x];
				}
				else
					flipped_grid[y][x] = grid[y][x];
			}
		}
	}
	default:
		break;
	}


	for (int y = 0; y < GRID_SIZE; y++) {
		for (int x = 0; x < GRID_SIZE; x++) {
			grid[y][x] = flipped_grid[y][x];
		}
	}
}

void setGrid(int grid[][GRID_SIZE]) {
	for (int y = 0; y < GRID_SIZE; y++) {
		for (int x = 0; x < GRID_SIZE; x++) {
			grid[y][x] = choiceWallOrPath();
		}
	}
	rotateGrid(grid);
	flipGrid(grid);
}

void setGridTemplate(int grid[][GRID_SIZE]) {
	FILE *fp;
	int grid_num = choiceTemplateNum();
	char filename[100];
	char str[16];

	sprintf_s(filename, 100, "GridTemplate/grid%d.txt", grid_num);
	printf("%s\n", filename);

	if (fopen_s(&fp, filename, "r") != 0) {
		printf("ファイルを開けませんでした。\n");
		return;
	}

	for (int y = 0; y < GRID_SIZE; y++) {
		fgets(str, 16, fp);
		printf("%s", str);
		for (int i = 0, x = 0; i < (GRID_SIZE * 3) - 1; i++) {
			switch (str[i])
			{
			case ',': {
				break;
			}
			case '-': {
				grid[y][x] = WALL;
				x++;
				i++;
				break;
			}
			case '0': {
				grid[y][x] = PATH;
				x++;
				break;
			}
			default:
				break;
			}
		}
	}

	fclose(fp);

	printGrid(grid);

	//rotateGrid(grid);
	//flipGrid(grid);//問題あり

	printGrid(grid);
}