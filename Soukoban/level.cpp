#include "level.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include "grid.h"

//コンストラクタ
Level::Level()
{
}
//デストラクタ
Level::~Level()
{
}
//ランダムにx座標を返す
int choiceX() {
	return (rand() % (WIDTH - 2)) + 1;
}
//ランダムにy座標を返す
int choiceY() {
	return (rand() % (HEIGHT - 2)) + 1;
}
//対象マスが荷物を運びこむことが可能かを判定する
bool checkCarryInSquare(const int stage[][WIDTH], const int x, const int y) {
	if ((stage[y][x - 1] == PATH || stage[y][x - 1] == GOAL) && (stage[y][x + 1] == PATH || stage[y][x + 1] == GOAL)) {
		if (stage[y][x - 2] == WALL && stage[y][x + 2] == WALL && stage[y - 1][x] == WALL && stage[y + 1][x] == WALL) {
			return true;
		}
	}

	if ((stage[y - 1][x] == PATH || stage[y - 1][x] == GOAL) && (stage[y + 1][x] == PATH || stage[y + 1][x] == GOAL)) {
		if (stage[y - 2][x] == WALL && stage[y + 2][x] == WALL && stage[y][x - 1] == WALL && stage[y][x + 1] == WALL) {
			return true;
		}
	}

	return false;
}
//チェックリストに荷物が運び込めないエリアをチェックする
std::vector<SQUARE> checkCarryInArea(const int stage[][WIDTH]) {
	std::vector<SQUARE>checklist;
	SQUARE square;
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (stage[y][x] == PATH) {
				if (!checkCarryInSquare(stage, x, y)) {
					square.x = x;
					square.y = y;
					checklist.push_back(square);
				}
			}
		}
	}

	return checklist;
}
//周囲の壁の数を数える
int countAroundWall(const int stage[][WIDTH], const int x, const int y) {
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
//対象マスが荷物を運びこむことが可能かを判定する
bool checkSquare(int stage[][WIDTH])
{
	bool flag = false;

	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			if (stage[y][x] == PATH)
			{
				if (countAroundWall(stage, x, y) >= 3)
				{
					stage[y][x] = WALL;
					flag = true;
				}
			}
		}
	}

	return flag;
}
//対象マスが辺に接しているか判定する
bool checkNeighborhoodWall(const int stage[][WIDTH], const int x, const int y) {
	//Left
	if (stage[y - 1][x - 1] == WALL && stage[y][x - 1] == WALL && stage[y + 1][x - 1] == WALL) {
		return true;
	}
	//Right
	if (stage[y - 1][x + 1] == WALL && stage[y][x + 1] == WALL && stage[y + 1][x + 1] == WALL) {
		return true;
	}
	//Up
	if (stage[y - 1][x - 1] == WALL && stage[y - 1][x] == WALL && stage[y - 1][x + 1] == WALL) {
		return true;
	}
	//Down
	if (stage[y + 1][x - 1] == WALL && stage[y + 1][x] == WALL && stage[y + 1][x + 1] == WALL) {
		return true;
	}

	return false;
}
//対象マスが角であるか判定する
bool checkCornerSquare(const int stage[][WIDTH], const int x, const int y) {
	//┗
	if (stage[y][x - 1] == WALL && stage[y + 1][x] == WALL) {
		return true;
	}
	//┛
	if (stage[y][x + 1] == WALL && stage[y + 1][x] == WALL) {
		return true;
	}
	//┏
	if (stage[y - 1][x] == WALL && stage[y][x - 1] == WALL) {
		return true;
	}
	//┓
	if (stage[y - 1][x] == WALL && stage[y][x + 1] == WALL) {
		return true;
	}

	return false;
}
//荷物を配置できる座標をリスト化し返す
std::vector<SQUARE> checkPutBox(const int stage[][WIDTH]) {
	std::vector<SQUARE>checklist;
	SQUARE square;

	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (stage[y][x] == PATH) {
				/*if (checkNeighborhoodWall(stage, x, y)) {
					checklist[y][x] = CHECK;
				}
				else */
				//配置予定場所が角で出ないか
				if (!checkCornerSquare(stage, x, y)) {
					square.x = x;
					square.y = y;
					checklist.push_back(square);
				}
			}
		}
	}

	return checklist;
}
//そこに荷物を置くことで詰みが発生するか判定する
bool checkDeadlock(const int stage[][WIDTH], const int x, const int y) {
	/*four boxes*/
	if ((stage[y][x + 1] == BOX || stage[y][x + 1] == BOX_ON_GOAL) && (stage[y + 1][x] == BOX || stage[y + 1][x] == BOX_ON_GOAL) && (stage[y + 1][x + 1] == BOX || stage[y + 1][x + 1] == BOX_ON_GOAL)) {
		return true;
	}
	else if ((stage[y][x - 1] == BOX || stage[y][x - 1] == BOX_ON_GOAL) && (stage[y + 1][x] == BOX || stage[y + 1][x] == BOX_ON_GOAL) && (stage[y + 1][x - 1] == BOX || stage[y + 1][x - 1] == BOX_ON_GOAL)) {
		return true;
	}
	else if ((stage[y][x + 1] == BOX || stage[y][x + 1] == BOX_ON_GOAL) && stage[y - 1][x] == BOX && (stage[y - 1][x + 1] == BOX || stage[y - 1][x + 1] == BOX_ON_GOAL)) {
		return true;
	}
	else if ((stage[y][x - 1] == BOX || stage[y][x - 1] == BOX_ON_GOAL) && stage[y - 1][x] == BOX && (stage[y - 1][x - 1] == BOX || stage[y - 1][x - 1] == BOX_ON_GOAL)) {
		return true;
	}
	/*one box two wall*/
	else if (stage[y][x + 1] == WALL && stage[y - 1][x] == WALL) {
		return true;
	}
	else if (stage[y][x - 1] == WALL && stage[y - 1][x] == WALL) {
		return true;
	}
	else if (stage[y][x - 1] == WALL && stage[y + 1][x] == WALL) {
		return true;
	}
	else if (stage[y][x + 1] == WALL && stage[y + 1][x] == WALL) {
		return true;
	}
	/*two box two wall(□)*/
	else if ((stage[y][x + 1] == BOX || stage[y][x + 1] == BOX_ON_GOAL) && stage[y - 1][x] == WALL && stage[y - 1][x + 1] == WALL) {
		return true;
	}
	else if ((stage[y][x - 1] == BOX || stage[y][x - 1] == BOX_ON_GOAL) && stage[y - 1][x] == WALL && stage[y - 1][x - 1] == WALL) {
		return true;
	}
	else if ((stage[y][x + 1] == BOX || stage[y][x + 1] == BOX_ON_GOAL) && stage[y + 1][x] == WALL && stage[y + 1][x + 1] == WALL) {
		return true;
	}
	else if ((stage[y][x - 1] == BOX || stage[y][x - 1] == BOX_ON_GOAL) && stage[y + 1][x] == WALL && stage[y + 1][x - 1] == WALL) {
		return true;
	}
	//
	else if (stage[y - 1][x] == BOX && stage[y][x - 1] == WALL && stage[y - 1][x - 1] == WALL) {
		return true;
	}
	else if ((stage[y + 1][x] == BOX || stage[y + 1][x] == BOX_ON_GOAL) && stage[y][x - 1] == WALL && stage[y + 1][x - 1] == WALL) {
		return true;
	}
	else if (stage[y - 1][x] == BOX && stage[y][x + 1] == WALL && stage[y - 1][x + 1] == WALL) {
		return true;
	}
	else if ((stage[y + 1][x] == BOX || stage[y + 1][x] == BOX_ON_GOAL) && stage[y][x + 1] == WALL && stage[y + 1][x + 1] == WALL) {
		return true;
	}
	/*two box two wall(S)*/
	else if ((stage[y][x + 1] == BOX || stage[y][x + 1] == BOX_ON_GOAL) && stage[y - 1][x] == WALL && stage[y + 1][x + 1] == WALL) {
		return true;
	}
	else if ((stage[y][x + 1] == BOX || stage[y][x + 1] == BOX_ON_GOAL) && stage[y + 1][x] == WALL && stage[y - 1][x + 1] == WALL) {
		return true;
	}
	else if ((stage[y][x - 1] == BOX || stage[y][x - 1] == BOX_ON_GOAL) && stage[y - 1][x] == WALL && stage[y + 1][x - 1] == WALL) {
		return true;
	}
	else if ((stage[y][x - 1] == BOX || stage[y][x - 1] == BOX_ON_GOAL) && stage[y + 1][x] == WALL && stage[y - 1][x - 1] == WALL) {
		return true;
	}
	//
	else if ((stage[y + 1][x] == BOX || stage[y + 1][x] == BOX_ON_GOAL) && stage[y][x + 1] == WALL && stage[y + 1][x - 1] == WALL) {
		return 1;
	}
	else if ((stage[y + 1][x] == BOX || stage[y + 1][x] == BOX_ON_GOAL) && stage[y][x - 1] == WALL && stage[y + 1][x + 1] == WALL) {
		return true;
	}
	else if ((stage[y - 1][x] == BOX || stage[y - 1][x] == BOX_ON_GOAL) && stage[y - 1][x + 1] == WALL && stage[y][x - 1] == WALL) {
		return true;
	}
	else if ((stage[y - 1][x] == BOX || stage[y - 1][x] == BOX_ON_GOAL) && stage[y - 1][x - 1] == WALL && stage[y][x + 1] == WALL) {
		return true;
	}
	/*three boxes one wall*/
	else if (stage[y][x + 1] == WALL && (stage[y + 1][x] == BOX || stage[y + 1][x] == BOX_ON_GOAL) && (stage[y + 1][x + 1] == BOX || stage[y + 1][x + 1] == BOX_ON_GOAL)) {
		return true;
	}
	else if ((stage[y][x + 1] == BOX || stage[y][x + 1] == BOX_ON_GOAL) && stage[y + 1][x] == WALL && (stage[y + 1][x + 1] == BOX || stage[y + 1][x + 1] == BOX_ON_GOAL)) {
		return true;
	}
	else if ((stage[y][x + 1] == BOX || stage[y][x + 1] == BOX_ON_GOAL) && (stage[y + 1][x] == BOX || stage[y + 1][x] == BOX_ON_GOAL) && stage[y + 1][x + 1] == WALL) {
		return true;
	}
	else if (stage[y][x - 1] == WALL && (stage[y + 1][x - 1] == BOX || stage[y + 1][x - 1] == BOX_ON_GOAL) && stage[y + 1][x] == WALL) {
		return true;
	}
	/*three boxes two wall*/
	//1
	else if ((stage[y][x + 1] == BOX || stage[y][x + 1] == BOX_ON_GOAL) && (stage[y + 1][x + 1] == BOX || stage[y + 1][x + 1] == BOX_ON_GOAL) && stage[y - 1][x] == WALL && stage[y + 1][x + 2] == WALL) {
		return true;
	}
	else if ((stage[y][x - 1] == BOX || stage[y][x - 1] == BOX_ON_GOAL) && (stage[y + 1][x] == BOX || stage[y + 1][x] == BOX_ON_GOAL) && stage[y - 1][x - 1] == WALL && stage[y + 1][x + 1] == WALL) {
		return true;
	}
	else if ((stage[y + 1][x] == BOX || stage[y + 1][x] == BOX_ON_GOAL) && (stage[y - 1][x - 1] == BOX || stage[y - 1][x - 1] == BOX_ON_GOAL) && stage[y][x + 1] == WALL && stage[y - 2][x - 1] == WALL) {
		return true;
	}
	//2
	else if ((stage[y][x + 1] == BOX || stage[y][x + 1] == BOX_ON_GOAL) && (stage[y + 1][x] == BOX || stage[y + 1][x] == BOX_ON_GOAL) && stage[y - 1][x + 1] == WALL && stage[y + 1][x - 1] == WALL) {
		return true;
	}
	else if ((stage[y][x - 1] == BOX || stage[y][x - 1] == BOX_ON_GOAL) && (stage[y + 1][x - 1] == BOX || stage[y + 1][x - 1] == BOX_ON_GOAL) && stage[y - 1][x] == WALL && stage[y + 1][x - 2] == WALL) {
		return true;
	}
	else if (stage[y - 1][x] == BOX && (stage[y - 1][x + 1] == BOX || stage[y - 1][x + 1] == BOX_ON_GOAL) && stage[y - 2][x + 1] == WALL && stage[y][x - 1] == WALL) {
		return true;
	}
	//3
	else if ((stage[y][x + 1] == BOX || stage[y][x + 1] == BOX_ON_GOAL) && (stage[y - 1][x + 1] == BOX || stage[y - 1][x + 1] == BOX_ON_GOAL) && stage[y - 1][x + 2] == WALL && stage[y + 1][x] == WALL) {
		return true;
	}
	else if ((stage[y + 1][x] == BOX || stage[y + 1][x] == BOX_ON_GOAL) && (stage[y + 1][x - 1] == BOX || stage[y + 1][x - 1] == BOX_ON_GOAL) && stage[y][x + 1] == WALL && stage[y + 2][x - 1] == WALL) {
		return true;
	}
	else if ((stage[y][x - 1] == BOX || stage[y][x - 1] == BOX_ON_GOAL) && stage[y - 1][x] == BOX && stage[y - 1][x + 1] == WALL && stage[y + 1][x - 1] == WALL) {
		return true;
	}
	//4
	else if ((stage[y + 1][x] == BOX || stage[y + 1][x] == BOX_ON_GOAL) && (stage[y + 1][x + 1] == BOX || stage[y + 1][x + 1] == BOX_ON_GOAL) && stage[y][x - 1] == WALL && stage[y + 2][x + 1] == WALL) {
		return true;
	}
	else if (stage[y - 1][x] == BOX && (stage[y][x + 1] == BOX || stage[y][x + 1] == BOX_ON_GOAL) && stage[y - 1][x - 1] == WALL && stage[y + 1][x + 1] == WALL) {
		return true;
	}
	else if ((stage[y][x - 1] == BOX || stage[y][x - 1] == BOX_ON_GOAL) && (stage[y - 1][x - 1] == BOX || stage[y - 1][x - 1] == BOX_ON_GOAL) && stage[y + 1][x] == WALL && stage[y - 1][x - 2] == WALL) {
		return true;
	}
	return false;
}
//プレイヤーを配置できる座標をリスト化し返す
std::vector<SQUARE> checkPutPlayer(const int stage[][WIDTH]) {
	std::vector<SQUARE>checklist;
	SQUARE square;

	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (stage[y][x] == PATH) {
				square.x = x;
				square.y = y;
				checklist.push_back(square);
			}
		}
	}

	return checklist;
}

//ステージ生成を行う
void Level::createLevel() {
	while (true)
	{
		setEmptyRoom();

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
//空の部屋を作る
void Level::createEmptyRoom() {
	Grid grid;

	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			stage[y][x] = WALL;
		}
	}

	for (int grid_num = 0, v = 0, h = 0; grid_num < (NUMBER_OF_GRID_VERTICAL * NUMBER_OF_GRID_HORIZONTAL); grid_num++) {
		v = grid_num / NUMBER_OF_GRID_HORIZONTAL;
		h = grid_num % NUMBER_OF_GRID_HORIZONTAL;
		grid.setGridTemplate();

		for (int y = 0; y < GRID_SIZE; y++) {
			for (int x = 0; x < GRID_SIZE; x++) {
				stage[y + (v * GRID_SIZE) + 1][x + (h * GRID_SIZE) + 1] = grid.grid[y][x];
			}
		}
	}
}
//空の部屋を作る
void Level::setEmptyRoom() {
	do {
		//テンプレートで空部屋を生成
		createEmptyRoom();
		//意味のないマスを埋め整地
		fillBlindAlley();
	} while (checkSection() == 1 && countSpace() > 8);
}
//袋小路など意味のないスペースを埋める
void Level::fillBlindAlley() {
	while (checkSquare(stage) != 0) {}
}
//ゴールを設置する
bool Level::setGoal() {
	std::vector<SQUARE>checklist;
	//配置可能な座標をvectorに
	checklist = checkCarryInArea(stage);
	//配置できる場所が存在しない
	if (checklist.empty()) {
		printf("ゴールの配置に失敗しました。\n");
		return false;
	}
	//配置できる場所が足りない
	else if (checklist.size() < NUMBER_OF_BOX) {
		printf("ゴールの配置に失敗しました。\n");
		return false;
	}
	//リストの中からランダムに
	for (int num_box = NUMBER_OF_BOX; num_box != 0; num_box--) {
		SQUARE set_square = checklist[rand() % checklist.size()];
		int x = set_square.x;
		int y = set_square.y;
		if (stage[y][x] == PATH) 
			stage[y][x] = GOAL;
	}

	return true;
}
//ゴールを設置する
bool Level::setBoxOnGoal() {
	std::vector<SQUARE>checklist;
	//配置可能な座標をvectorに
	checklist = checkCarryInArea(stage);
	//配置できる場所が存在しない
	if (checklist.empty()) {
		printf("ゴール上の荷物の配置に失敗しました。\n");
		return false;
	}
	//配置できる場所が足りない
	else if (checklist.size() < NUMBER_OF_BOX) {
		printf("ゴール上の荷物の配置に失敗しました。\n");
		return false;
	}
	//リストの中からランダムに
	for (int num_box = NUMBER_OF_BOX; num_box != 0; num_box--) {
		SQUARE set_square = checklist[rand() % checklist.size()];
		int x = set_square.x;
		int y = set_square.y;
		if (stage[y][x] == PATH)
			stage[y][x] = BOX_ON_GOAL;
	}

	return true;
}
//荷物を設置する
bool Level::setBox() {
	for (int num_box = NUMBER_OF_BOX; num_box != 0; num_box--) {
		//配置可能な座標をvectorに
		std::vector<SQUARE> checklist = checkPutBox(stage);
		//配置できる場所が存在しない
		if (checklist.empty()) {
			printf("荷物の配置に失敗しました。\n");
			return false;
		}
		//配置できる場所が足りない
		else if (checklist.size() < NUMBER_OF_BOX) {
			printf("荷物の配置に失敗しました。\n");
			return false;
		}
		//リストの中からランダムに
		SQUARE set_square = checklist[rand() % checklist.size()];
		int x = set_square.x;
		int y = set_square.y;
		//詰みを作らなければ配置
		if (!checkDeadlock(stage, x, y)) {
			stage[y][x] = BOX;
		}
	}

	return true;
}
//プレイヤーを配置する
bool Level::setPlayer() {
	//配置可能な座標をvectorに
	std::vector<SQUARE> checklist = checkPutPlayer(stage);

	//配置できる場所が存在しない
	if (checklist.empty()) {
		printf("荷物の配置に失敗しました。\n");
		return false;
	}
	//リストの中からランダムに
	SQUARE set_square = checklist[rand() % checklist.size()];
	int x = set_square.x;
	int y = set_square.y;
	stage[y][x] = MAN;
	
	return true;
}
//ステージをコマンドプロンプトへ出力する
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
//ステージをテキストファイルに出力する
void Level::outputStage() {
	FILE *fp;

	if (fopen_s(&fp, "StageData/stage.txt", "w") != 0) {
		printf("ファイルを開けませんでした。\n");
		return;
	}

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
//途切れたスペースができていればtrueを返す
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
//ステージのマスをカウントし返す
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
//空の部屋はそのままで配置物をリセットする
void Level::resetStage() {
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (stage[y][x] != WALL) {
				stage[y][x] = PATH;
			}
		}
	}
}
//空の部屋に配置物をすべてセットする
void Level::setStage() {
	while (true)
	{
		//ゴール、荷物、プレイヤーを配置
		if (!setGoal()) {
			resetStage();
			printf("この形では配置できません。\n");
			printStage();
			setEmptyRoom();
			continue;
		}
		if (!setBox()) {
			resetStage();
			continue;
		}
		if (!setPlayer()) {
			resetStage();
			continue;
		}

		break;
	}
}
//ステージをstring型で出力
std::string Level::outputString() {
	std::string line = {};
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (stage[y][x] == WALL)
				line+="#";
			else if (stage[y][x] == MAN)
				line += "@";
			else if (stage[y][x] == MAN_ON_GOAL)
				line += "+";
			else if (stage[y][x] == BOX)
				line += "$";
			else if (stage[y][x] == BOX_ON_GOAL)
				line += "*";
			else if (stage[y][x] == GOAL)
				line += ".";
			else if (stage[y][x] == PATH)
				line += " ";
		}
		line += "\n";
	}

	return line;
}