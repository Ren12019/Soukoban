#include "grid.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "define.h"

//コンストラクタ
Grid::Grid()
{
}
//デストラクタ
Grid::~Grid()
{
}
//乱数により壁か道かを決定する
int choiceWallOrPath() {
	int choice = (rand() % 2) + 1;
	switch (choice) {
	case 1:
		return WALL;
	case 2:
		return PATH;
	default:
		break;
	}
	return WALL;
}
//乱数により使用するテンプレートの番号を選ぶ
int choiceTemplateNum() {
	return rand() % NUMBER_OF_GRID_TEMPLATE;
}
//設置するグリッドを回転させる方向を選ぶ
int choiceDirection() {
	switch (rand() % 4 + 1) {
	case FRONT:
		return FRONT;
	case BACK:
		return BACK;
	case RIGHT:
		return RIGHT;
	case LEFT:
		return LEFT;
	default:
		break;
	}

	return FRONT;
}
//設置するグリッドを反転させるか決定する
int choiceFlip() {
	switch (rand() % 2) {
	case VERTICAL:
		return VERTICAL;
	case HORIZONTAL:
		return HORIZONTAL;
	default:
		return 0;
	}

	return 0;
}
//グリッドを回転させる
void Grid::rotateGrid() {
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
//グリッドを反転させる
void Grid::flipGrid() {
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
		break;
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
		break;
	}
	default:
		break;
	}

	if (flip != 0) {
		for (int y = 0; y < GRID_SIZE; y++) {
			for (int x = 0; x < GRID_SIZE; x++) {
				grid[y][x] = flipped_grid[y][x];
			}
		}
	}
}
//乱数でグリッド内の配置を決定する
void Grid::setGrid() {
	for (int y = 0; y < GRID_SIZE; y++) {
		for (int x = 0; x < GRID_SIZE; x++) {
			grid[y][x] = choiceWallOrPath();
		}
	}
	rotateGrid();
	flipGrid();
}
//テンプレートの配置を利用しグリッドを作成する
void Grid::setGridTemplate() {
	FILE *fp;
	int grid_num = choiceTemplateNum();
	char filename[100];
	char str[16];

	sprintf_s(filename, 100, "GridTemplate/grid%d.txt", grid_num);

	if (fopen_s(&fp, filename, "r") != 0) {
		printf("ファイルを開けませんでした。\n");
		return;
	}

	for (int y = 0; y < GRID_SIZE; y++) {
		fgets(str, 16, fp);
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

	rotateGrid();
	flipGrid();
}
//グリッドを表示する
void Grid::printGrid() {
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