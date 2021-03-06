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

enum definition {
	VERTICAL = 1,
	HORIZONTAL = 2
};

typedef struct square {
	int x;
	int y;
}SQUARE;