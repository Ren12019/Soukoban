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
bool Level::canCarryInSquare(const SQUARE square) {
	const int x = square.x;
	const int y = square.y;

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
std::vector<SQUARE> Level::storeCarryInArea() {
	std::vector<SQUARE>checklist;
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (stage[y][x] == ' ') {
				SQUARE square = { x,y };
				if (!canCarryInSquare(square)) {
					checklist.push_back(square);
				}
			}
		}
	}

	return checklist;
}
//ゴールが置ける場所をキューする
std::vector<SQUARE> Level::storeCandidate() {
	std::vector<SQUARE>canditdate;
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (stage[y][x] == ' ') {
				SQUARE square = { x,y };
				if (!canCarryInSquare(square)) {
					canditdate.push_back(square);
				}
			}
		}
	}

	return canditdate;
}
//周囲の壁の数を数える
int Level::countAroundWall(const SQUARE square) {
	const int x = square.x;
	const int y = square.y;
	int cnt_wall = 0;

	if (stage[y - 1][x] == '#')
		cnt_wall++;
	if (stage[y + 1][x] == '#')
		cnt_wall++;
	if (stage[y][x - 1] == '#')
		cnt_wall++;
	if (stage[y][x + 1] == '#')
		cnt_wall++;

	return cnt_wall;
}
//テンプレートを使用してグリッドから空部屋を作成する
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
//袋小路を埋める
void Level::fillBlindAlley() {
	bool exist = true;
	while (exist) {
		exist = false;
		for (int y = 0; y < HEIGHT; y++) {
			for (int x = 0; x < WIDTH; x++) {
				if (stage[y][x] == ' ') {
					SQUARE square = { x,y };
					if (countAroundWall(square) >= 3) {
						stage[y][x] = '#';
						exist = true;
					}
				}
			}
		}
	}
}
//ゴールを設置する
bool Level::setBoxOnGoal(SQUARE set_pos) {
	int x = set_pos.x;
	int y = set_pos.y;
	if (stage[y][x] == ' ') {
		stage[y][x] = '*';
		return true;
	}

	return false;
}
//アバターを配置する
bool Level::setPlayer() {
	//配置可能な座標をvectorに
	std::vector<SQUARE> candidate = storeCandidate();

	//配置できる場所が存在しない
	if (candidate.empty()) {
		std::cout << "アバターの配置に失敗しました。" << std::endl;
		return false;
	}
	//リストの中からランダムに
	SQUARE set_square = candidate[rand() % candidate.size()];
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
bool Level::isTwoRoom() {
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
//無駄な空間の確認
bool Level::hasLargeSpace() {
	//bool check_space = false;
	//幅と高さを計測
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			//スペースを探索
			if (stage[y][x] == ' ') {
				//3*4
				//スペースのみの長方形であるか
				for (int cnt_y = 0; cnt_y < 4; cnt_y++) {
					int h = y + cnt_y;
					if (h >= HEIGHT) {
						break;
					}
					int w;
					for (int cnt_x = 0; cnt_x < 3; cnt_x++) {
						w = x + cnt_x;
						if (w >= WIDTH) {
							break;
						}
						if (stage[h][w] == '#') {
							break;
						}
						else if (w == x + 2 && h == y + 3) {
							return true;
						}
					}
					if (stage[h][w] == '#') {
						break;
					}
				}
				//4*3
				//計測した幅と高さの空間がスペースのみの長方形であるか
				for (int cnt_y = 0; cnt_y < 3; cnt_y++) {
					int h = y + cnt_y;
					if (h >= HEIGHT) {
						break;
					}
					int w;
					for (int cnt_x = 0; cnt_x < 4; cnt_x++) {
						w = x + cnt_x;
						if (w >= WIDTH) {
							break;
						}
						if (stage[h][w] == '#') {
							break;
						}
						else if (w == x + 3 && h == y + 2) {
							return true;
						}
					}
					if (stage[h][w] == '#') {
						break;
					}
				}
			}
		}
	}
	
	return false;
}
//条件を満たした空部屋を作る
void Level::setEmptyRoom() {
	//2部屋でなく、8マス以上かつ3*4,4*3の長方形の空間がないものを生成
	do {
		//テンプレートで空部屋を生成
		createEmptyRoom();
		//意味のないマスを埋め整地
		fillBlindAlley();
	} while (isTwoRoom() || countSpace() < 13 || hasLargeSpace());
}