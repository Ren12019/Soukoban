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
	void setEmptyRoom();//条件を満たした空の部屋を作る
	void resetStage();//空の部屋はそのままで配置物をリセットする
	void printStage();//ステージを表示する
	void outputStage();//ステージをテキストファイルとして出力
	std::string outputString();//ステージをstring型で出力
	void inputString(std::string);//ステージをstring型で入力
	bool setBoxOnGoal(SQUARE);
	bool setPlayer();//ステージにプレイヤーを配置する
	std::vector<SQUARE> storeCandidate();//ゴールが置ける場所をキューする
	int countSpace();//ステージの通過可能部分をカウントする
private:
	std::vector< std::vector<char> > stage;//ステージマップの本体

	void createEmptyRoom();//空の部屋を作る
	void fillBlindAlley();//ステージの袋小路をつぶす
	bool isTwoRoom();//部屋が2つ以上になっていないか確認
	bool canCarryInSquare(const SQUARE);
	std::vector<SQUARE> storeCarryInArea();
	int countAroundWall(const SQUARE);//対象マスの周囲の壁をカウントする
	bool hasLargeSpace();//無駄に広い空間が存在するか確認する
};