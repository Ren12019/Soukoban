#pragma once

enum setting {
	WIDTH = 11,
	HEIGHT = 11,

	GRID_SIZE = 3,

	RATE = 8
};

enum number {
	NUMBER_OF_BOX = 2,
	NUMBER_OF_GRID_VERTICAL = 2,
	NUMBER_OF_GRID_HORIZONTAL = 2,
	NUMBER_OF_GRID_TEMPLATE = 17
};

enum block_type {
	WALL = -1,
	MAN = 1,
	MAN_ON_GOAL = 3,
	BOX = 5,
	BOX_ON_GOAL = 7,
	GOAL = 2,
	PATH = 0,
	CHECK = 10
};

enum definition {
	VERTICAL = 1,
	HORIZONTAL = 2
};

typedef struct square {
	int x;
	int y;
}SQUARE;