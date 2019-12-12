#pragma once
#include <string>
#include <vector>
#include <queue>
#include "define.h"

class Level
{
public:
	Level();
	~Level();
	std::vector< std::vector<char> > stage;
	void setEmptyRoom();//条件を満たした空の部屋を作る
	void createLevel();//ステージ生成を行う
	void setStage();//空の部屋に配置物をすべてセットする
	void resetStage();//空の部屋はそのままで配置物をリセットする
	void printStage();//ステージを表示する
	void outputStage();//ステージをテキストファイルとして出力
	std::string outputString();//ステージをstring型で出力
	void inputString(std::string);//ステージをstring型で入力
	bool setBoxOnGoal();
	bool setBoxOnGoal(SQUARE);
	bool setPlayer();//ステージにプレイヤーを配置する
	std::vector<SQUARE> decisionCanditdate();//ゴールが置ける場所をキューする
private:
	void createEmptyRoom();//空の部屋を作る
	void fillBlindAlley();//ステージの袋小路をつぶす
	bool setGoal();//ステージにゴールを設置する
	bool setBox();//ステージに荷物を設置する
	bool checkSection();//部屋が2つ以上になっていないか確認
	int countSpace();//ステージの通過可能部分をカウントする
	bool checkCarryInSquare(const int, const int);
	std::vector<SQUARE> checkCarryInArea();
	int countAroundWall(const int, const int);
	bool checkSquare();
	bool checkCornerSquare(const int, const int);
	std::vector<SQUARE> checkPutBox();
	bool checkDeadlock(const int, const int);
	std::vector<SQUARE> checkPutPlayer();
	bool checkLargeSpace();//無駄に広い空間が存在するか確認する
};