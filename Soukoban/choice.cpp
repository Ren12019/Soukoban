#include "choice.h"
#include <stdlib.h>
#include "define.h"

int choiceX() {
	return (rand() % (WIDTH - 2)) + 1;
}

int choiceY() {
	return (rand() % (HEIGHT - 2)) + 1;
}

int choiceWallOrPath(void) {
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

int choiceDirection(void) {
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

int choiceFlip(void) {
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

int choiceTemplateNum(void) {
	return rand() % NUMBER_OF_GRID_TEMPLATE;
}