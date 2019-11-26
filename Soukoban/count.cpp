#include "count.h"
#include "define.h"

int countPath(int stage[][WIDTH]) {
	int count = 0;

	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (stage[y][x] != PATH) {
				count++;
			}
		}
	}

	return count;
}

int countAroundWall(int stage[][WIDTH], int x, int y) {
	int count = 0;

	if (stage[y - 1][x] == WALL)
		count++;
	if (stage[y + 1][x] == WALL)
		count++;
	if (stage[y][x - 1] == WALL)
		count++;
	if (stage[y][x + 1] == WALL)
		count++;

	return count;
}

int countStageSize(int stage[][WIDTH]) {
	int count = 0;

	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (stage[y][x] != WALL) {
				count++;
			}
		}
	}

	return count;
}

int countConnectionSquare(int stage[][WIDTH],SQUARE target) {
	int count = 0;

	if (stage[target.y - 1][target.x] == WALL)
		count++;
	if (stage[target.y + 1][target.x] == WALL)
		count++;
	if (stage[target.y][target.x - 1] == WALL)
		count++;
	if (stage[target.y][target.x + 1] == WALL)
		count++;

	return count;
}

int countConnectionAll(int stage[][WIDTH]) {
	int cnt_all_connection = 0;
	int cnt_square_connection = 0;
	SQUARE target;

	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (stage[y][x] != WALL) {
				target.x = x;
				target.y = y;
				cnt_square_connection = countConnectionSquare(stage,target);
				cnt_all_connection += cnt_square_connection;
			}
		}
	}

	return cnt_all_connection;
}