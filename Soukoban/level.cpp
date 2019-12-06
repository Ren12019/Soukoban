#include "level.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include "grid.h"

//コンストラクタ
Level::Level()
{
	std::vector<char> temp(WIDTH,'#');

	for (int y = 0; y < HEIGHT; y++) {
		stage.emplace_back(temp);
	}
}
//デストラクタ
Level::~Level()
{
}
//対象マスが荷物を運びこむことが可能かを判定する
bool Level::checkCarryInSquare(const int x, const int y) {
	if ((stage[y][x - 1] == ' ' || stage[y][x - 1] == '.') && (stage[y][x + 1] == ' ' || stage[y][x + 1] == '.')) {
		if (stage[y][x - 2] == '#' && stage[y][x + 2] == '#' && stage[y - 1][x] == '#' && stage[y + 1][x] == '#') {
			return true;
		}
	}

	if ((stage[y - 1][x] == ' ' || stage[y - 1][x] == '.') && (stage[y + 1][x] == ' ' || stage[y + 1][x] == '.')) {
		if (stage[y - 2][x] == '#' && stage[y + 2][x] == '#' && stage[y][x - 1] == '#' && stage[y][x + 1] == '#') {
			return true;
		}
	}

	return false;
}
// チェックリストに荷物が運び込めないエリアをチェックする
std::vector<SQUARE> Level::checkCarryInArea() {
	std::vector<SQUARE>checklist;
	SQUARE square;
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (stage[y][x] == ' ') {
				if (!checkCarryInSquare(x, y)) {
					square.x = x;
					square.y = y;
					checklist.push_back(square);
				}
			}
		}
	}

	return checklist;
}
//ゴールが置ける場所をキューする
std::vector<SQUARE> Level::decisionCanditdate() {
	std::vector<SQUARE>canditdate;
	SQUARE square;
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (stage[y][x] == ' ') {
				if (!checkCarryInSquare(x, y)) {
					square.x = x;
					square.y = y;
					canditdate.push_back(square);
				}
			}
		}
	}

	return canditdate;
}
//周囲の壁の数を数える
int Level::countAroundWall(const int x, const int y) {
	int count = 0;

	if (stage[y - 1][x] == '#')
		count++;
	if (stage[y + 1][x] == '#')
		count++;
	if (stage[y][x - 1] == '#')
		count++;
	if (stage[y][x + 1] == '#')
		count++;

	return count;
}
//対象マスが荷物を運びこむことが可能かを判定する
bool Level::checkSquare()
{
	bool flag = false;

	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			if (stage[y][x] == ' ')
			{
				if (countAroundWall(x, y) >= 3)
				{
					stage[y][x] = '#';
					flag = true;
				}
			}
		}
	}

	return flag;
}
//対象マスが角であるか判定する
bool Level::checkCornerSquare(const int x, const int y){
	//┗
	if (stage[y][x - 1] == '#' && stage[y + 1][x] == '#') {
		return true;
	}
	//┛
	if (stage[y][x + 1] == '#' && stage[y + 1][x] == '#') {
		return true;
	}
	//┏
	if (stage[y - 1][x] == '#' && stage[y][x - 1] == '#') {
		return true;
	}
	//┓
	if (stage[y - 1][x] == '#' && stage[y][x + 1] == '#') {
		return true;
	}

	return false;
}
//荷物を配置できる座標をリスト化し返す
std::vector<SQUARE> Level::checkPutBox() {
	std::vector<SQUARE>checklist;
	SQUARE square;

	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (stage[y][x] == ' ') {
				//配置予定場所が角で出ないか
				if (!checkCornerSquare(x, y)) {
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
bool Level::checkDeadlock(const int x, const int y) {
	/*four boxes*/
	if ((stage[y][x + 1] == '$' || stage[y][x + 1] == '*') && (stage[y + 1][x] == '$' || stage[y + 1][x] == '*') && (stage[y + 1][x + 1] == '$' || stage[y + 1][x + 1] == '*')) {
		return true;
	}
	else if ((stage[y][x - 1] == '$' || stage[y][x - 1] == '*') && (stage[y + 1][x] == '$' || stage[y + 1][x] == '*') && (stage[y + 1][x - 1] == '$' || stage[y + 1][x - 1] == '*')) {
		return true;
	}
	else if ((stage[y][x + 1] == '$' || stage[y][x + 1] == '*') && stage[y - 1][x] == '$' && (stage[y - 1][x + 1] == '$' || stage[y - 1][x + 1] == '*')) {
		return true;
	}
	else if ((stage[y][x - 1] == '$' || stage[y][x - 1] == '*') && stage[y - 1][x] == '$' && (stage[y - 1][x - 1] == '$' || stage[y - 1][x - 1] == '*')) {
		return true;
	}
	/*one box two wall*/
	else if (stage[y][x + 1] == '#' && stage[y - 1][x] == '#') {
		return true;
	}
	else if (stage[y][x - 1] == '#' && stage[y - 1][x] == '#') {
		return true;
	}
	else if (stage[y][x - 1] == '#' && stage[y + 1][x] == '#') {
		return true;
	}
	else if (stage[y][x + 1] == '#' && stage[y + 1][x] == '#') {
		return true;
	}
	/*two box two wall(□)*/
	else if ((stage[y][x + 1] == '$' || stage[y][x + 1] == '*') && stage[y - 1][x] == '#' && stage[y - 1][x + 1] == '#') {
		return true;
	}
	else if ((stage[y][x - 1] == '$' || stage[y][x - 1] == '*') && stage[y - 1][x] == '#' && stage[y - 1][x - 1] == '#') {
		return true;
	}
	else if ((stage[y][x + 1] == '$' || stage[y][x + 1] == '*') && stage[y + 1][x] == '#' && stage[y + 1][x + 1] == '#') {
		return true;
	}
	else if ((stage[y][x - 1] == '$' || stage[y][x - 1] == '*') && stage[y + 1][x] == '#' && stage[y + 1][x - 1] == '#') {
		return true;
	}
	//
	else if (stage[y - 1][x] == '$' && stage[y][x - 1] == '#' && stage[y - 1][x - 1] == '#') {
		return true;
	}
	else if ((stage[y + 1][x] == '$' || stage[y + 1][x] == '*') && stage[y][x - 1] == '#' && stage[y + 1][x - 1] == '#') {
		return true;
	}
	else if (stage[y - 1][x] == '$' && stage[y][x + 1] == '#' && stage[y - 1][x + 1] == '#') {
		return true;
	}
	else if ((stage[y + 1][x] == '$' || stage[y + 1][x] == '*') && stage[y][x + 1] == '#' && stage[y + 1][x + 1] == '#') {
		return true;
	}
	/*two box two wall(S)*/
	else if ((stage[y][x + 1] == '$' || stage[y][x + 1] == '*') && stage[y - 1][x] == '#' && stage[y + 1][x + 1] == '#') {
		return true;
	}
	else if ((stage[y][x + 1] == '$' || stage[y][x + 1] == '*') && stage[y + 1][x] == '#' && stage[y - 1][x + 1] == '#') {
		return true;
	}
	else if ((stage[y][x - 1] == '$' || stage[y][x - 1] == '*') && stage[y - 1][x] == '#' && stage[y + 1][x - 1] == '#') {
		return true;
	}
	else if ((stage[y][x - 1] == '$' || stage[y][x - 1] == '*') && stage[y + 1][x] == '#' && stage[y - 1][x - 1] == '#') {
		return true;
	}
	//
	else if ((stage[y + 1][x] == '$' || stage[y + 1][x] == '*') && stage[y][x + 1] == '#' && stage[y + 1][x - 1] == '#') {
		return 1;
	}
	else if ((stage[y + 1][x] == '$' || stage[y + 1][x] == '*') && stage[y][x - 1] == '#' && stage[y + 1][x + 1] == '#') {
		return true;
	}
	else if ((stage[y - 1][x] == '$' || stage[y - 1][x] == '*') && stage[y - 1][x + 1] == '#' && stage[y][x - 1] == '#') {
		return true;
	}
	else if ((stage[y - 1][x] == '$' || stage[y - 1][x] == '*') && stage[y - 1][x - 1] == '#' && stage[y][x + 1] == '#') {
		return true;
	}
	/*three boxes one wall*/
	else if (stage[y][x + 1] == '#' && (stage[y + 1][x] == '$' || stage[y + 1][x] == '*') && (stage[y + 1][x + 1] == '$' || stage[y + 1][x + 1] == '*')) {
		return true;
	}
	else if ((stage[y][x + 1] == '$' || stage[y][x + 1] == '*') && stage[y + 1][x] == '#' && (stage[y + 1][x + 1] == '$' || stage[y + 1][x + 1] == '*')) {
		return true;
	}
	else if ((stage[y][x + 1] == '$' || stage[y][x + 1] == '*') && (stage[y + 1][x] == '$' || stage[y + 1][x] == '*') && stage[y + 1][x + 1] == '#') {
		return true;
	}
	else if (stage[y][x - 1] == '#' && (stage[y + 1][x - 1] == '$' || stage[y + 1][x - 1] == '*') && stage[y + 1][x] == '#') {
		return true;
	}
	/*three boxes two wall*/
	//1
	else if ((stage[y][x + 1] == '$' || stage[y][x + 1] == '*') && (stage[y + 1][x + 1] == '$' || stage[y + 1][x + 1] == '*') && stage[y - 1][x] == '#' && stage[y + 1][x + 2] == '#') {
		return true;
	}
	else if ((stage[y][x - 1] == '$' || stage[y][x - 1] == '*') && (stage[y + 1][x] == '$' || stage[y + 1][x] == '*') && stage[y - 1][x - 1] == '#' && stage[y + 1][x + 1] == '#') {
		return true;
	}
	else if ((stage[y + 1][x] == '$' || stage[y + 1][x] == '*') && (stage[y - 1][x - 1] == '$' || stage[y - 1][x - 1] == '*') && stage[y][x + 1] == '#' && stage[y - 2][x - 1] == '#') {
		return true;
	}
	//2
	else if ((stage[y][x + 1] == '$' || stage[y][x + 1] == '*') && (stage[y + 1][x] == '$' || stage[y + 1][x] == '*') && stage[y - 1][x + 1] == '#' && stage[y + 1][x - 1] == '#') {
		return true;
	}
	else if ((stage[y][x - 1] == '$' || stage[y][x - 1] == '*') && (stage[y + 1][x - 1] == '$' || stage[y + 1][x - 1] == '*') && stage[y - 1][x] == '#' && stage[y + 1][x - 2] == '#') {
		return true;
	}
	else if (stage[y - 1][x] == '$' && (stage[y - 1][x + 1] == '$' || stage[y - 1][x + 1] == '*') && stage[y - 2][x + 1] == '#' && stage[y][x - 1] == '#') {
		return true;
	}
	//3
	else if ((stage[y][x + 1] == '$' || stage[y][x + 1] == '*') && (stage[y - 1][x + 1] == '$' || stage[y - 1][x + 1] == '*') && stage[y - 1][x + 2] == '#' && stage[y + 1][x] == '#') {
		return true;
	}
	else if ((stage[y + 1][x] == '$' || stage[y + 1][x] == '*') && (stage[y + 1][x - 1] == '$' || stage[y + 1][x - 1] == '*') && stage[y][x + 1] == '#' && stage[y + 2][x - 1] == '#') {
		return true;
	}
	else if ((stage[y][x - 1] == '$' || stage[y][x - 1] == '*') && stage[y - 1][x] == '$' && stage[y - 1][x + 1] == '#' && stage[y + 1][x - 1] == '#') {
		return true;
	}
	//4
	else if ((stage[y + 1][x] == '$' || stage[y + 1][x] == '*') && (stage[y + 1][x + 1] == '$' || stage[y + 1][x + 1] == '*') && stage[y][x - 1] == '#' && stage[y + 2][x + 1] == '#') {
		return true;
	}
	else if (stage[y - 1][x] == '$' && (stage[y][x + 1] == '$' || stage[y][x + 1] == '*') && stage[y - 1][x - 1] == '#' && stage[y + 1][x + 1] == '#') {
		return true;
	}
	else if ((stage[y][x - 1] == '$' || stage[y][x - 1] == '*') && (stage[y - 1][x - 1] == '$' || stage[y - 1][x - 1] == '*') && stage[y + 1][x] == '#' && stage[y - 1][x - 2] == '#') {
		return true;
	}
	return false;
}
//プレイヤーを配置できる座標をリスト化し返す
std::vector<SQUARE> Level::checkPutPlayer() {
	std::vector<SQUARE>checklist;
	SQUARE square;

	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (stage[y][x] == ' ') {
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
	} while (checkSection() == 1 || countSpace() < 8);
}
//袋小路など意味のないスペースを埋める
void Level::fillBlindAlley() {
	while (checkSquare() != 0) {}
}
//ゴールを設置する
bool Level::setGoal() {
	std::vector<SQUARE>checklist;
	//配置可能な座標をvectorに
	checklist = checkCarryInArea();
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
		if (stage[y][x] == ' ') 
			stage[y][x] = '.';
	}

	return true;
}
//ゴールを設置する
bool Level::setBoxOnGoal() {
	std::vector<SQUARE>checklist;
	//配置可能な座標をvectorに
	checklist = checkCarryInArea();
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
#if 1
	for (int num_box = NUMBER_OF_BOX; num_box != 0; num_box--) {
		SQUARE set_square = checklist[rand() % checklist.size()];
		int x = set_square.x;
		int y = set_square.y;
		if (stage[y][x] == ' ')
			stage[y][x] = '*';
	}
#else
#endif
	return true;
}
void Level::setBoxOnGoal(SQUARE set_pos) {
	int x = set_pos.x;
	int y = set_pos.y;
	if (stage[y][x] == ' ')
		stage[y][x] = '*';
}
//荷物を設置する
bool Level::setBox() {
	for (int num_box = NUMBER_OF_BOX; num_box != 0; num_box--) {
		//配置可能な座標をvectorに
		std::vector<SQUARE> checklist = checkPutBox();
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
		if (!checkDeadlock(x, y)) {
			stage[y][x] = '$';
		}
	}

	return true;
}
//プレイヤーを配置する
bool Level::setPlayer() {
	//配置可能な座標をvectorに
	std::vector<SQUARE> checklist = checkPutPlayer();

	//配置できる場所が存在しない
	if (checklist.empty()) {
		printf("荷物の配置に失敗しました。\n");
		return false;
	}
	//リストの中からランダムに
	SQUARE set_square = checklist[rand() % checklist.size()];
	int x = set_square.x;
	int y = set_square.y;
	stage[y][x] = '@';
	
	return true;
}
//ステージをコマンドプロンプトへ出力する
void Level::printStage() {
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (stage[y][x] == '#')
				printf("#");
			else if (stage[y][x] == '@')
				printf("@");
			else if (stage[y][x] == '+')
				printf("+");
			else if (stage[y][x] == '$')
				printf("$");
			else if (stage[y][x] == '*')
				printf("*");
			else if (stage[y][x] == '.')
				printf(".");
			else if (stage[y][x] == ' ')
				printf(" ");
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
			if (stage[y][x] == '#')
				fprintf(fp, "#");
			else if (stage[y][x] == '@')
				fprintf(fp, "@");
			else if (stage[y][x] == '+')
				fprintf(fp, "+");
			else if (stage[y][x] == '$')
				fprintf(fp, "$");
			else if (stage[y][x] == '*')
				fprintf(fp, "*");
			else if (stage[y][x] == '.')
				fprintf(fp, ".");
			else if (stage[y][x] == ' ')
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
			if (stage[y][x] == ' ' || stage[y][x] == '.')
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
			if (stage[y][x] != '#') {
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
			if (stage[y][x] != '#') {
				stage[y][x] = ' ';
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
			if (stage[y][x] == '#')
				line+="#";
			else if (stage[y][x] == '@')
				line += "@";
			else if (stage[y][x] == '+')
				line += "+";
			else if (stage[y][x] == '$')
				line += "$";
			else if (stage[y][x] == '*')
				line += "*";
			else if (stage[y][x] == '.')
				line += ".";
			else if (stage[y][x] == ' ')
				line += " ";
		}
		line += "\n";
	}

	return line;
}
//ステージをstring型で入力
void Level::inputString(std::string input) {
	std::stringstream ss(input);
	std::string line;
	int counter = 0;
	std::vector< std::vector<char> > copy_stage;
	while (getline(ss, line, '\n'))
	{
		std::vector<char> temp;
		copy_stage.push_back(temp);
		for (unsigned int i = 0; i < line.length(); i++)
		{
			copy_stage[counter].push_back(line[i]);
		}
		counter++;
	}
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			stage[y][x] = copy_stage[y][x];
		}
	}
}