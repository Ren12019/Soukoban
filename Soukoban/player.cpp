#include "player.h"
#include "define.h"
#include "choice.h"

void initPlayer(int stage[][WIDTH]) {
	int x = 0, y = 0;

	while (true) {
		x = choiceX();
		y = choiceY();
		if (stage[y][x] == PATH) {
			stage[y][x] = MAN;
			break;
		}
		else if (stage[y][x] == GOAL) {
			stage[y][x] = MAN_ON_GOAL;
			break;
		}
	}
}