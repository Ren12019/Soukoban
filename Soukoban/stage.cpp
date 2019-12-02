#include "stage.h"
#include <stdlib.h>
#include <time.h>
#include "define.h"
#include "grid.h"
#include "print.h"
#include "choice.h"
#include "check.h"

Level::Level()
{
}

Level::~Level()
{
}

void Level::createLevel(){
	while (true)
	{
		do {
			//テンプレートで空部屋を生成
			createEmptyRoom();
			//意味のないマスを埋め整地
			fillBlindAlley();
		} while (checkSection() == 1 && countSpace() > 4);

		//ゴール、荷物、プレイヤーを配置
		if (!setGoal()) {
			continue;
		}
		if (!setBox()) {
			continue;
		}
		if (!setPlayer()) {
			continue;
		}

		break;
	}
}

void Level::createEmptyRoom() {
	int grid[GRID_SIZE][GRID_SIZE] = {};

	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			stage[y][x] = WALL;
		}
	}

	for (int grid_num = 0, v = 0, h = 0; grid_num < (NUMBER_OF_GRID_VERTICAL * NUMBER_OF_GRID_HORIZONTAL); grid_num++) {
		v = grid_num / NUMBER_OF_GRID_HORIZONTAL;
		h = grid_num % NUMBER_OF_GRID_HORIZONTAL;
		setGridTemplate(grid);
		
		for (int y = 0; y < GRID_SIZE; y++) {
			for (int x = 0; x < GRID_SIZE; x++) {
				stage[y + (v * GRID_SIZE) + 1][x + (h * GRID_SIZE) + 1] = grid[y][x];
			}
		}
	}
}

void Level::fillBlindAlley() {
	while (checkTile(stage) != 0) {}
}

bool Level::setGoal() {
	int x = 0, y = 0;
	int checklist[HEIGHT][WIDTH] = {};

	checkCarryInArea(stage, checklist);

	for (int num_box = NUMBER_OF_BOX; num_box != 0; num_box--) {
		while (true) {
			x = choiceX();
			y = choiceY();
			if (stage[y][x] == PATH && checklist[y][x] != CHECK) {
				stage[y][x] = GOAL;
				break;
			}
		}
	}
	return true;
}

bool Level::setBox(){
	clock_t t = clock();
	int x = 0, y = 0;
	int checklist[HEIGHT][WIDTH] = {};

	for (int num_box = NUMBER_OF_BOX; num_box != 0; num_box--) {
		checkPutBox(stage, checklist);

		while (true) {
			x = choiceX();
			y = choiceY();
			if ((stage[y][x] == PATH || stage[y][x] == GOAL) && checklist[y][x] != CHECK) {
				if (checkDeadlock(stage, x, y)) {
					checklist[y][x] = CHECK;
				}
				else {
					if (stage[y][x] == PATH) {
						stage[y][x] = BOX;
						if (checkSection())
						{
							stage[y][x] = PATH;
						}
						else {
							break;
						}
					}
					else if (stage[y][x] == GOAL) {
						stage[y][x] = BOX_ON_GOAL;
						if (checkSection())
						{
							stage[y][x] = GOAL;
						}
						else {
							break;
						}
					}
				}
			}

			if (checkChecklist(checklist)) {
				printf("Failed to create stage:Not Find Space\n");
				return false;
			}
			if (clock() - t > 2000) {
				printf("Failed to create stage:Time Out\n");
				return false;
			}
		}
	}

	return true;

}

bool Level::setPlayer() {
	int x = 0, y = 0;

	while (true) {
		x = choiceX();
		y = choiceY();
		if (stage[y][x] == PATH) {
			stage[y][x] = MAN;
			return true;
		}
		else if (stage[y][x] == GOAL) {
			stage[y][x] = MAN_ON_GOAL;
			return true;
		}
	}
}
void Level::printStage() {
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (stage[y][x] == WALL)
				printf("#");
			else if (stage[y][x] == MAN)
				printf("@");
			else if (stage[y][x] == MAN_ON_GOAL)
				printf("+");
			else if (stage[y][x] == BOX)
				printf("$");
			else if (stage[y][x] == BOX_ON_GOAL)
				printf("*");
			else if (stage[y][x] == GOAL)
				printf(".");
			else if (stage[y][x] == PATH)
				printf(" ");
			else if (stage[y][x] == CHECK)
				printf("x");
		}
		printf("\n");
	}
	printf("\n");
}

void Level::outputStage() {
	FILE *fp;

	if (fopen_s(&fp, "StageData/stage.txt", "w") != 0) {
		printf("ファイルを開けませんでした。\n");
		return;
	}
	//横幅を出力
	fprintf(fp, "%d\n", WIDTH);

	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (stage[y][x] == WALL)
				fprintf(fp, "#");
			else if (stage[y][x] == MAN)
				fprintf(fp, "@");
			else if (stage[y][x] == MAN_ON_GOAL)
				fprintf(fp, "+");
			else if (stage[y][x] == BOX)
				fprintf(fp, "$");
			else if (stage[y][x] == BOX_ON_GOAL)
				fprintf(fp, "*");
			else if (stage[y][x] == GOAL)
				fprintf(fp, ".");
			else if (stage[y][x] == PATH)
				fprintf(fp, " ");
		}
		fprintf(fp, "\n");
	}

	fclose(fp);
}

bool Level::checkSection() {
	int checksheet[HEIGHT][WIDTH] = {};
	int section = 1;
	int flag = 1;

	for (int y = 1; y < HEIGHT - 1; y++)
	{
		for (int x = 1; x < WIDTH - 1; x++)
		{
			if (stage[y][x] == PATH || stage[y][x] == GOAL)
			{
				if (checksheet[y - 1][x] != 0)
					checksheet[y][x] = checksheet[y - 1][x];
				else if (checksheet[y][x - 1] != 0)
					checksheet[y][x] = checksheet[y][x - 1];
				else
				{
					checksheet[y][x] = section;
					section++;
				}
			}
		}
	}

	while (flag)
	{
		flag = 0;

		for (int y = 1; y < HEIGHT - 1; y++)
		{
			for (int x = 1; x < WIDTH - 1; x++)
			{
				if (checksheet[y][x] != 0)
				{
					if (checksheet[y - 1][x] != 0 && checksheet[y][x] > checksheet[y - 1][x])
					{
						checksheet[y][x] = checksheet[y - 1][x];
						flag = 1;
					}
					if (checksheet[y][x - 1] != 0 && checksheet[y][x] > checksheet[y][x - 1])
					{
						checksheet[y][x] = checksheet[y][x - 1];
						flag = 1;
					}
					if (checksheet[y + 1][x] != 0 && checksheet[y][x] > checksheet[y + 1][x])
					{
						checksheet[y][x] = checksheet[y + 1][x];
						flag = 1;
					}
					if (checksheet[y][x + 1] != 0 && checksheet[y][x] > checksheet[y][x + 1])
					{
						checksheet[y][x] = checksheet[y][x + 1];
						flag = 1;
					}
				}
			}
		}
	}

	for (int y = 1; y < HEIGHT - 1; y++)
	{
		for (int x = 1; x < WIDTH - 1; x++)
		{
			if (checksheet[y][x] != 0 && checksheet[y][x] != 1)
			{
				return true;
			}
		}
	}

	return false;
}

int Level::countSpace() {
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

/*機能の試運転用*/
void setStageTest(int stage[][WIDTH],SQUARE set)
{
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (x == 0 || y == 0 || x == WIDTH - 1 || y == HEIGHT - 1) {
				stage[y][x] = WALL;
			}
			else if (x == set.x && y == set.y) {
				stage[y][x]= BOX;
			}
			else if (x == 7 && y == 5) {
				stage[y][x] = GOAL;
			}
			else if (x == 4 && y == 4) {
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

void freeStageList()
{
	STAGELIST *p =head;
	STAGELIST *temp;

	while (p != NULL) {
		temp = p->next_stage; //次のデータの参照を取っておく
		free(p);
		p = temp;       //tempを次の処理対象に
	}
}
