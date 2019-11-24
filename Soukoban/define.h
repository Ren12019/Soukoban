#pragma once

enum SETTING {
	WIDTH = 11,
	HEIGHT = 11,

	GRID_SIZE = 3,

	RATE = 6
};

enum NUMBER {
	NUMBER_OF_BOX = 1,
	NUMBER_OF_BLOCK_TYPE = 5,
	NUMBER_OF_GRID_VERTICAL = 2,
	NUMBER_OF_GRID_HORIZONTAL = 3
};

enum BLOCK_TYPE {
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

enum DIRECTION {
	FRONT = 1,
	RIGHT = 2,
	BACK = 3,
	LEFT = 4
};

typedef struct stagelist{
	struct stagelist *next_stage;
	int stage[HEIGHT][WIDTH];
}STAGELIST;

extern STAGELIST*head;
extern STAGELIST*tail;