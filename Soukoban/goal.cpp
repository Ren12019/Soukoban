#include "goal.h"
#include <stdlib.h>
#include "define.h"
#include "check.h"
#include "choice.h"
#include "count.h"

void initGoal(std::vector <std::vector<int>> stage) {
	int x = 0, y = 0;
	 std::vector <std::vector<int>> checklist(HEIGHT, std::vector<int>(WIDTH, 0));

	checkCarryInArea(stage, checklist);

	for (int num_box = NUMBER_OF_BOX; num_box != 0; num_box--){
		while (true) {
			x = choiceX();
			y = choiceY();
			if (stage[y][x] == PATH && checklist[y][x] != CHECK) {
				stage[y][x] = GOAL;
				break;
			}
		}
	}
}