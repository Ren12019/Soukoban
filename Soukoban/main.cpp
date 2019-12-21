#include <stdio.h>
#include <iostream>
#include <time.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>

#include "define.h"
#include "level.h"
#include "search.h"

//実行モード
enum ExecutionMode { NORMAL = 1, RESEARCH };
//生成モード
enum SetMode { BRUTE_FORCE, MY_MODE};
//問題評価の基準点数
enum Border { MOVES = 18, PUSHES = 8, LINES = 4, TOTAL_VAL = 30,MAX=100 };
int moves, pushes, lines, value;
//レベルの評価用
struct EVALUATION {
	std::string stage = "この空の部屋では生成できません\n";//ステージの内容
	int pushes = 0;//荷物の移動スコア
	int moves = 0;//アバターの移動スコア
	int change_lines = 0;//荷物の方向転換スコア
};
//座標の評価用
class EVA_LOCATION :public EVALUATION {
private:
	SQUARE box_loc = {};
	SQUARE goal_loc = {};
	int value = 0;//評価値
public:
	void setLocationBox(SQUARE);
	void setLocationGoal(SQUARE);
	void setValue();
	bool isEmpty();
	SQUARE outputLocationBox();
	SQUARE outputLocationGoal();
	int outputValue();
	int outputPushes();
	int outputMoves();
	int outputLines();
};
void EVA_LOCATION::setLocationBox(SQUARE target_loca) {
	box_loc = target_loca;
}
void EVA_LOCATION::setLocationGoal(SQUARE target_loca) {
	goal_loc = target_loca;
}
void EVA_LOCATION::setValue() {
	value = moves + change_lines;
}
bool EVA_LOCATION::isEmpty() {
	if (value == 0) {
		return true;
	}
	return false;
}
SQUARE EVA_LOCATION::outputLocationBox() {
	return box_loc;
}
SQUARE EVA_LOCATION::outputLocationGoal() {
	return goal_loc;
}
int EVA_LOCATION::outputValue() {
	return value;
}
int EVA_LOCATION::outputPushes() {
	return pushes;
}
int EVA_LOCATION::outputMoves() {
	return moves;
}
int EVA_LOCATION::outputLines() {
	return lines;
}

//表示用
struct PROPERTY :public EVALUATION{
	std::string mode_name = {};//モード名
	timespec start, end;//タイマー
	int value = 0;//評価値
	int area = 0;//面積
};
//基準点をセット
void setReferencePoint(Level level) {
	std::ifstream ifs("ReferencePoint/2box/2_2_easy.txt");
	std::string name;
	std::string str;
	//一行目を廃棄
	getline(ifs, name);
	//一行づつ読み込み
	int space = level.countSpace();
	if (space < 14 || space>29) {
		return;
	}
	for (int i = (space - 14); i >= 0; i--) {
		getline(ifs, str);
	}
	std::string tmp;
	std::istringstream stream(str);
	//面積
	getline(stream, tmp, ',');
	//moves
	getline(stream, tmp, ',');
	moves = std::stoi(tmp);
	//pushes
	getline(stream, tmp, ',');
	pushes = std::stoi(tmp);
	//lines
	getline(stream, tmp, ',');
	lines = std::stoi(tmp);
	//value
	getline(stream, tmp, '\n');
	value = std::stoi(tmp);
}
//座標の組み合わせをリスト化（総当たり用）
std::queue<std::vector<SQUARE>>createListCandidate(const std::vector<SQUARE> candidate) {
	std::queue<SQUARE> que_cand;//読み取り用
	std::queue < std::vector<SQUARE>> list_cand;//座標の組み合わせリスト
	//選択用にキューへ
	for (unsigned int i = 0; i < candidate.size(); i++) {
		que_cand.push(candidate[i]);
	}
	//選出
	for (unsigned int i = 0; i < candidate.size(); i++) {
		SQUARE first = que_cand.front();
		que_cand.pop();
		std::queue <SQUARE> copy_cand = que_cand;
		switch(NUMBER_OF_BOX) {
		case 1: {
			std::vector<SQUARE>set;
			//1つ目の座標
			set.push_back(first);
			list_cand.push(set);
			break;
		}
		case 2: {
			while (!copy_cand.empty()) {
				std::vector<SQUARE>set;
				//1つ目の座標
				set.push_back(first);
				//2つ目の座標
				set.push_back(copy_cand.front());
				copy_cand.pop();
				list_cand.push(set);
			}
			break;
		}
		case 3: {
			while (!copy_cand.empty()) {
				std::vector<SQUARE>set;
				set.push_back(first);
				SQUARE second = copy_cand.front();
				copy_cand.pop();
				std::queue <SQUARE> copy_copy_cand = copy_cand;
				while (!copy_copy_cand.empty()) {
					std::vector<SQUARE>set;
					//1つ目の座標
					set.push_back(first);
					//2つ目の座標
					set.push_back(second);
					//3つ目の座標
					set.push_back(copy_copy_cand.front());
					copy_copy_cand.pop();
					list_cand.push(set);
				}
			}
			break;
		}
		default:
			break;
		}
	}

	return list_cand;
}
//座標の組み合わせをリスト化（効率化用）
std::vector<std::vector<SQUARE>>createListCandidateVector(const std::vector<SQUARE> candidate) {
	std::queue<SQUARE> que_cand;//読み取り用
	std::vector < std::vector<SQUARE>> list_cand;//座標の組み合わせリスト
	//選択用にキューへ
	for (unsigned int i = 0; i < candidate.size(); i++) {
		que_cand.push(candidate[i]);
	}
	//選出
	for (unsigned int i = 0; i < candidate.size(); i++) {
		SQUARE first = que_cand.front();
		que_cand.pop();
		std::queue <SQUARE> copy_cand = que_cand;
		switch (NUMBER_OF_BOX) {
		case 1: {
			std::vector<SQUARE>set;
			//1つ目の座標
			set.push_back(first);
			list_cand.push_back(set);
			break;
		}
		case 2: {
			while (!copy_cand.empty()) {
				std::vector<SQUARE>set;
				//1つ目の座標
				set.push_back(first);
				//2つ目の座標
				set.push_back(copy_cand.front());
				copy_cand.pop();
				list_cand.push_back(set);
			}
			break;
		}
		case 3: {
			while (!copy_cand.empty()) {
				std::vector<SQUARE>set;
				set.push_back(first);
				SQUARE second = copy_cand.front();
				copy_cand.pop();
				std::queue <SQUARE> copy_copy_cand = copy_cand;
				while (!copy_copy_cand.empty()) {
					std::vector<SQUARE>set;
					//1つ目の座標
					set.push_back(first);
					//2つ目の座標
					set.push_back(second);
					//3つ目の座標
					set.push_back(copy_copy_cand.front());
					copy_copy_cand.pop();
					list_cand.push_back(set);
				}
			}
			break;
		}
		default:
			break;
		}
	}

	return list_cand;
}
//座標を高評価順にソート
std::queue<EVA_LOCATION>createQueueCandidateCarefullySelected(const std::queue<EVA_LOCATION> candidate) {
	std::queue<EVA_LOCATION> que_cand;//読み取り用
	std::vector<EVA_LOCATION> sort_cand;//ソート用
	std::queue<EVA_LOCATION> result;//返り値
	//選択用にキューへ
	que_cand = candidate;
	//選出
	while(!que_cand.empty()) {
		EVA_LOCATION current = que_cand.front();
		que_cand.pop();
		//空なら
		if (sort_cand.empty()) {
			sort_cand.push_back(current);
			continue;
		}
		//比較し挿入
		for (unsigned int i = 0; i < sort_cand.size(); i++) {
			//value
			int arr_value = sort_cand[i].outputValue();
			if (current.outputValue() > arr_value) {
				sort_cand.insert(sort_cand.begin() + i, current);
				break;
			}
			else if (current.outputValue() == arr_value) {
				//lines
				int arr_lines = sort_cand[i].outputLines();
				if (current.outputValue() > arr_lines) {
					sort_cand.insert(sort_cand.begin() + i, current);
					break;
				}
				else if (current.outputValue() == arr_lines) {
					//pushes
					int arr_pushes = sort_cand[i].outputPushes();
					if (current.outputValue() > arr_pushes) {
						sort_cand.insert(sort_cand.begin() + i, current);
						break;
					}
					else if (current.outputValue() == arr_pushes) {
						//moves
						int arr_moves = sort_cand[i].outputMoves();
						if (current.outputValue() >= arr_moves) {
							sort_cand.insert(sort_cand.begin() + i, current);
							break;
						}
					}

				}
			}
			//挿入ができないなら
			if (i == sort_cand.size() - 1) {
				sort_cand.push_back(current);
				break;
			}
		}
	}
	//ソート内容をキューへ
	for (unsigned int i = 0; i < sort_cand.size(); i++) {
		result.push(sort_cand[i]);
	}

	return result;
}
//総当たりで最良の配置を求める
std::queue<EVALUATION> runBruteForceMode(Level level) {
	std::queue<EVALUATION> compare;//レベルの評価用
	std::vector<SQUARE> candidate;//ゴールの候補地のキュー
	//ゴールが配置可能な場所を配列に収納
	candidate = level.storeCandidateAll();
	std::queue<std::vector<SQUARE>>list_cand;//ゴールの候補地の組み合わせのリスト
	//配置可能な場所の組み合わせをリスト化
	list_cand = createListCandidate(candidate);
	/*ゴールの配置を総当たり*/
	//全てのゴールを試すまで
	while (!list_cand.empty() && list_cand.front().size() >= NUMBER_OF_BOX) {
		//初期設定
		State create_start_stat;//初期状態
		SearchStat create_finish_stat;//探索終了状態
		std::string input_level;
		bool create_box = false;
		int cnt_box = 0;
		//規定数配置し終わるまで
		while (!create_box) {
			//配置できる場所が存在しない
			if (list_cand.empty()) {
				break;//終了
			}
			//配置できる場所が足りない
			else if (list_cand.front().size() < NUMBER_OF_BOX) {
				break;//終了
			}
			//座標の決定
			SQUARE set_square = list_cand.front()[cnt_box];
			//ゴール上の荷物を配置
			if (!level.setBoxOnGoal(set_square)) {
				//配置に失敗
				list_cand.pop();
				level.resetStage();
				cnt_box = 0;
				break;
			}
			//生成したステージをインプット
			input_level = level.outputString();

			//探索用に初期化
			create_start_stat.state_str = input_level;
			create_start_stat.move_list = "";
			create_start_stat.moves = create_start_stat.pushes =
				create_start_stat.push_lines = create_start_stat.depth =
				create_start_stat.push_direction = 0;

			//一次的に荷物を壁に
			if (cnt_box != 0) {
				//荷物を見つけ壁に置き換え
				std::queue<SQUARE>pos_box;
				SQUARE pos_instant;//キューへプッシュ用
				for (unsigned int i = 0; i < create_start_stat.state_str.size(); i++) {
					if (create_start_stat.state_str[i] == '$') {
						create_start_stat.state_str[i] = '#';
						pos_instant.x = i % (WIDTH + 1);
						pos_instant.y = i / (WIDTH + 1);
						pos_box.push(pos_instant);
					}
				}
				//生成したレベルに対して逆に幅優先探索を行う
				create_finish_stat = choose_search(create_start_stat, BFSR);
				if (create_finish_stat.node.state_str != "NULL\n") {
					//壁を荷物に戻す
					while (!pos_box.empty()) {
						unsigned int i = pos_box.front().y*(WIDTH + 1) + pos_box.front().x;
						create_finish_stat.node.state_str[i] = '$';
						pos_box.pop();
					}
				}
			}
			else {
				//生成したレベルに対して逆に幅優先探索を行う
				create_finish_stat = choose_search(create_start_stat, BFSR);
			}

			//盤面に変更がありNULLでないとき配置成功
			if (create_finish_stat.node.state_str != input_level && create_finish_stat.node.state_str != "NULL\n") {
				cnt_box++;//配置数をカウント
				level.inputString(create_finish_stat.node.state_str);//ステージを更新
			}
			//失敗なら次の組み合わせへ
			else {
				list_cand.pop();
				level.resetStage();
				cnt_box = 0;
			}
			//配置数が規定に達したか判定
			if (cnt_box == NUMBER_OF_BOX) {
				create_box = true;
				list_cand.pop();
			}
		}
		if (create_finish_stat.node.state_str == "NULL\n") {
			continue;
		}
		else if (!create_box) {
			continue;
		}

		////////////////////////////////
		/*ステージ探索部分*/
		////////////////////////////////
		//levelにインプット
		level.inputString(create_finish_stat.node.state_str);
		//アバターを配置
		level.setPlayer();
		//探索用にセット
		input_level = level.outputString();


		//初期設定
		State init_state;//初期状態
		SearchStat final_stat;//探索終了状態
		//test
		std::cout << "生成されたレベルです。" << std::endl;
		std::cout << input_level;

		//初期化
		init_state.state_str = input_level;
		init_state.move_list = "";
		init_state.moves = init_state.pushes =
			init_state.push_lines = init_state.depth =
			init_state.push_direction = 0;

		//生成したレベルに対して幅優先探索を行う
		final_stat = choose_search(init_state, BFS);
		//クリアチェック
		if (final_stat.node.move_list.empty()) {
			std::cout << "生成されたレベルは解答不可能です。" << std::endl;
			//リセット
			level.resetStage();
			continue;//終了
		}
		//人の移動回数を表示
		std::cout << "このレベルの荷物を動かす最小回数は:" << final_stat.node.pushes
			<< std::endl;

		/*比較用に保存*/
		EVALUATION cur_state;
		cur_state.stage = init_state.state_str;//レベル
		cur_state.pushes = final_stat.node.pushes;//プッシュ
		cur_state.moves = final_stat.node.moves;//ムーブ
		cur_state.change_lines = final_stat.node.push_lines;//移動の方向転換
		//配列にレベルと評価を保存
		compare.push(cur_state);
		//リセット
		level.resetStage();
	}

	return compare;
}
//（効率化）総当たりで最良の配置を求める
std::queue<EVALUATION> runBruteForceModeSpeedUp(Level level) {
	std::queue<EVALUATION> compare;//レベルの評価用
	std::vector<SQUARE> candidate;//ゴールの候補地のキュー
	//ゴールが配置可能な場所を配列に収納
	candidate = level.storeCandidateTouchingWall();
	std::queue<std::vector<SQUARE>>list_cand;//ゴールの候補地の組み合わせのリスト
	//配置可能な場所の組み合わせをリスト化
	list_cand = createListCandidate(candidate);
	/*ゴールの配置を総当たり*/
	//全てのゴールを試すまで
	while (!list_cand.empty() && list_cand.front().size() >= NUMBER_OF_BOX) {
		//初期設定
		State create_start_stat;//初期状態
		SearchStat create_finish_stat;//探索終了状態
		std::string input_level;
		bool create_box = false;
		int cnt_box = 0;
		//規定数配置し終わるまで
		while (!create_box) {
			//配置できる場所が存在しない
			if (list_cand.empty()) {
				break;//終了
			}
			//配置できる場所が足りない
			else if (list_cand.front().size() < NUMBER_OF_BOX) {
				break;//終了
			}
			//座標の決定
			SQUARE set_square = list_cand.front()[cnt_box];
			//ゴール上の荷物を配置
			if (!level.setBoxOnGoal(set_square)) {
				//配置に失敗
				list_cand.pop();
				level.resetStage();
				cnt_box = 0;
				break;
			}
			//生成したステージをインプット
			input_level = level.outputString();

			//探索用に初期化
			create_start_stat.state_str = input_level;
			create_start_stat.move_list = "";
			create_start_stat.moves = create_start_stat.pushes =
				create_start_stat.push_lines = create_start_stat.depth =
				create_start_stat.push_direction = 0;

			//一次的に荷物を壁に
			if (cnt_box != 0) {
				//荷物を見つけ壁に置き換え
				std::queue<SQUARE>pos_box;
				SQUARE pos_instant;//キューへプッシュ用
				for (unsigned int i = 0; i < create_start_stat.state_str.size(); i++) {
					if (create_start_stat.state_str[i] == '$') {
						create_start_stat.state_str[i] = '#';
						pos_instant.x = i % (WIDTH + 1);
						pos_instant.y = i / (WIDTH + 1);
						pos_box.push(pos_instant);
					}
				}
				//生成したレベルに対して逆に幅優先探索を行う
				create_finish_stat = choose_search(create_start_stat, BFSR);
				if (create_finish_stat.node.state_str != "NULL\n") {
					//壁を荷物に戻す
					while (!pos_box.empty()) {
						unsigned int i = pos_box.front().y*(WIDTH + 1) + pos_box.front().x;
						create_finish_stat.node.state_str[i] = '$';
						pos_box.pop();
					}
				}
			}
			else {
				//生成したレベルに対して逆に幅優先探索を行う
				create_finish_stat = choose_search(create_start_stat, BFSR);
			}

			//盤面に変更がありNULLでないとき配置成功
			if (create_finish_stat.node.state_str != input_level && create_finish_stat.node.state_str != "NULL\n") {
				cnt_box++;//配置数をカウント
				level.inputString(create_finish_stat.node.state_str);//ステージを更新
			}
			//失敗なら次の組み合わせへ
			else {
				list_cand.pop();
				level.resetStage();
				cnt_box = 0;
			}
			//配置数が規定に達したか判定
			if (cnt_box == NUMBER_OF_BOX) {
				create_box = true;
				list_cand.pop();
			}
		}
		if (create_finish_stat.node.state_str == "NULL\n") {
			continue;
		}
		else if (!create_box) {
			continue;
		}

		////////////////////////////////
		/*ステージ探索部分*/
		////////////////////////////////
		//levelにインプット
		level.inputString(create_finish_stat.node.state_str);
		//アバターを配置
		level.setPlayer();
		//探索用にセット
		input_level = level.outputString();


		//初期設定
		State init_state;//初期状態
		SearchStat final_stat;//探索終了状態
		//test
		std::cout << "生成されたレベルです。" << std::endl;
		std::cout << input_level;

		//初期化
		init_state.state_str = input_level;
		init_state.move_list = "";
		init_state.moves = init_state.pushes =
			init_state.push_lines = init_state.depth =
			init_state.push_direction = 0;

		//生成したレベルに対して幅優先探索を行う
		final_stat = choose_search(init_state, BFS);
		//クリアチェック
		if (final_stat.node.move_list.empty()) {
			std::cout << "生成されたレベルは解答不可能です。" << std::endl;
			//リセット
			level.resetStage();
			continue;//終了
		}
		//人の移動回数を表示
		std::cout << "このレベルの荷物を動かす最小回数は:" << final_stat.node.pushes
			<< std::endl;

		/*比較用に保存*/
		EVALUATION cur_state;
		cur_state.stage = init_state.state_str;//レベル
		cur_state.pushes = final_stat.node.pushes;//プッシュ
		cur_state.moves = final_stat.node.moves;//ムーブ
		cur_state.change_lines = final_stat.node.push_lines;//移動の方向転換
		//配列にレベルと評価を保存
		compare.push(cur_state);
		//リセット
		level.resetStage();
	}

	return compare;
}
//座標ごとに評価を行い評価が高い順に選択
std::queue<EVALUATION> runCarefullySelectedMode(Level level) {
	std::queue<EVALUATION> result;//レベルの評価用
	std::queue<EVA_LOCATION> comp_candidate;//ゴールの候補地評価用
	std::vector<SQUARE> candidate;//ゴールの候補地のキュー
	State init_state;//初期状態
	SearchStat final_stat;//探索終了状態

	//ゴールが配置可能な場所を配列に収納
	candidate = level.storeCandidateAll();
	//ゴールの候補地を採点
	//全てのゴールを試すまで
	while (!candidate.empty()) {
		level.resetStage();
		//初期設定
		State create_start_stat;//初期状態
		SearchStat create_finish_stat;//探索終了状態
		std::string input_level;
		//座標の決定
		SQUARE set_square = candidate.front();
		candidate.erase(candidate.begin());
		//ゴール上の荷物を配置
		if (!level.setBoxOnGoal(set_square)) {
			//配置に失敗
			continue;
		}
		//生成したステージをインプット
		input_level = level.outputString();

		//探索用に初期化
		create_start_stat.state_str = input_level;
		create_start_stat.move_list = "";
		create_start_stat.moves = create_start_stat.pushes =
			create_start_stat.push_lines = create_start_stat.depth =
			create_start_stat.push_direction = 0;

		//生成したレベルに対して逆に幅優先探索を行う
		create_finish_stat = choose_search(create_start_stat, BFSR);

		//盤面に変更がありNULLでないとき配置成功
		if (create_finish_stat.node.state_str != input_level && create_finish_stat.node.state_str != "NULL\n") {
			level.inputString(create_finish_stat.node.state_str);//ステージを更新
		}
		//失敗なら次の組み合わせへ
		else {
			continue;
		}
		//採点
		////////////////////////////////
		/*ステージ探索部分*/
		////////////////////////////////
		//levelにインプット
		level.inputString(create_finish_stat.node.state_str);
		//アバターを配置
		level.setPlayer();
		SQUARE pos_box = level.searchBox();//一次的に荷物の位置を保存
		//探索用にセット
		input_level = level.outputString();

		//初期設定
		State init_state;//初期状態
		SearchStat final_stat;//探索終了状態

		//初期化
		init_state.state_str = input_level;
		init_state.move_list = "";
		init_state.moves = init_state.pushes =
			init_state.push_lines = init_state.depth =
			init_state.push_direction = 0;

		//生成したレベルに対して幅優先探索を行う
		final_stat = choose_search(init_state, BFS);
		//クリアチェック
		if (final_stat.node.move_list.empty()) {
			std::cout << "生成されたレベルは解答不可能です。" << std::endl;
			//リセット
			level.resetStage();
			continue;//終了
		}
		//人の移動回数を表示
		std::cout << "このレベルの荷物を動かす最小回数は:" << final_stat.node.pushes << std::endl;

		/*比較用に保存*/
		EVA_LOCATION cur_state;
		cur_state.setLocationGoal(set_square);
		level.inputString(final_stat.node.state_str);
		cur_state.setLocationBox(pos_box);
		cur_state.stage = init_state.state_str;//レベル
		cur_state.pushes = final_stat.node.pushes;//プッシュ
		cur_state.moves = final_stat.node.moves;//ムーブ
		cur_state.change_lines = final_stat.node.push_lines;//移動の方向転換
		cur_state.setValue();
		//配列にレベルと評価を保存
		comp_candidate.push(cur_state);
	}
	std::cout << "ゴール採点終了" << std::endl;
	if (comp_candidate.empty()) {
		return result;
	}
	std::queue<EVA_LOCATION>list_cand;//ゴールの候補地の組み合わせのリスト
	//配置可能な場所を高評価順にリスト化
	list_cand = createQueueCandidateCarefullySelected(comp_candidate);
	//候補地を組み合わせる
	std::queue<EVA_LOCATION> cpy_list_cand = list_cand;
	/*ゴールの配置*/
	SQUARE best_box = cpy_list_cand.front().outputLocationBox();
	SQUARE best_goal = cpy_list_cand.front().outputLocationGoal();
	cpy_list_cand.pop();
	if (NUMBER_OF_BOX == 2) {
		while (!cpy_list_cand.empty()) {
			level.resetStage();
			level.setBox(best_box);
			level.setGoal(best_goal);
			SQUARE second_box = cpy_list_cand.front().outputLocationBox();
			SQUARE second_goal = cpy_list_cand.front().outputLocationGoal();
			cpy_list_cand.pop();
			if (level.setGoal(second_goal)) {
				if (!level.setBox(second_box)) {
					continue;
				}
			}
			else {
				continue;
			}
			
			////////////////////////////////
			/*ステージ探索部分*/
			////////////////////////////////
			//アバターを配置
			level.setPlayer();
			//探索用にセット
			std::string input_level = level.outputString();
			//初期化
			init_state.state_str = input_level;
			init_state.move_list = "";
			init_state.moves = init_state.pushes =
				init_state.push_lines = init_state.depth =
				init_state.push_direction = 0;
			level.printStage();
			//生成したレベルに対して幅優先探索を行う
			final_stat = choose_search(init_state, BFS);
			//クリアチェック
			if (final_stat.node.move_list.empty()) {
				std::cout << "生成されたレベルは解答不可能です。" << std::endl;
				//リセット
				level.resetStage();
				continue;
			}
			else {
				//人の移動回数を表示
				std::cout << "このレベルの荷物を動かす最小回数は:" << final_stat.node.pushes << std::endl;
				/*返り値用に保存*/
				EVALUATION cur_state;
				cur_state.stage = init_state.state_str;//レベル
				cur_state.pushes = final_stat.node.pushes;//プッシュ
				cur_state.moves = final_stat.node.moves;//ムーブ
				cur_state.change_lines = final_stat.node.push_lines;//移動の方向転換
				result.push(cur_state);
				break;//終了
			}
		}
	}
	else if (NUMBER_OF_BOX == 3) {
		while (!cpy_list_cand.empty()) {
			SQUARE second_box = cpy_list_cand.front().outputLocationBox();
			SQUARE second_goal = cpy_list_cand.front().outputLocationGoal();
			cpy_list_cand.pop();
			std::queue<EVA_LOCATION> cpy_cpy_list_cand = cpy_list_cand;
			while (!cpy_cpy_list_cand.empty()) {
				level.resetStage();
				level.setBox(best_box);
				level.setGoal(best_goal);
				if (level.setGoal(second_goal)) {
					if (!level.setBox(second_box)) {
						break;
					}
				}
				else {
					break;
				}
				SQUARE third_box = cpy_list_cand.front().outputLocationBox();
				SQUARE third_goal = cpy_list_cand.front().outputLocationGoal();
				cpy_cpy_list_cand.pop();

				if (level.setGoal(third_goal)) {
					if (!level.setBox(third_box)) {
						continue;
					}
				}
				else {
					continue;
				}
				////////////////////////////////
				/*ステージ探索部分*/
				////////////////////////////////
				//アバターを配置
				level.setPlayer();
				//探索用にセット
				std::string input_level = level.outputString();
				//初期化
				init_state.state_str = input_level;
				init_state.move_list = "";
				init_state.moves = init_state.pushes =
					init_state.push_lines = init_state.depth =
					init_state.push_direction = 0;

				//生成したレベルに対して幅優先探索を行う
				final_stat = choose_search(init_state, BFS);
				//クリアチェック
				if (final_stat.node.move_list.empty()) {
					std::cout << "生成されたレベルは解答不可能です。" << std::endl;
					//リセット
					level.resetStage();
					continue;
				}
				else {
					//人の移動回数を表示
					std::cout << "このレベルの荷物を動かす最小回数は:" << final_stat.node.pushes << std::endl;
					/*返り値用に保存*/
					EVALUATION cur_state;
					cur_state.stage = init_state.state_str;//レベル
					cur_state.pushes = final_stat.node.pushes;//プッシュ
					cur_state.moves = final_stat.node.moves;//ムーブ
					cur_state.change_lines = final_stat.node.push_lines;//移動の方向転換
					result.push(cur_state);
					break;//終了
				}
			}
		}
	}

	return result;
}
//基準点を満たす配置を求める。存在しない場合は総当たりとなる。
std::queue<EVALUATION> runMyMode(Level level) {
	std::queue<EVALUATION> better_level;//レベルの評価用
	std::queue<EVALUATION> compare;//レベルの評価用
	std::vector<SQUARE> candidate;//ゴールの候補地のキュー
	std::vector<std::vector<SQUARE>>list_cand;//ゴールの候補地の組み合わせのリスト
	candidate = level.storeCandidateAll();
	//配置可能な場所の組み合わせをリスト化
	list_cand = createListCandidateVector(candidate);
	/*ゴールの配置をランダムに*/
	//規定を満たしたものができるまで
	while (!list_cand.empty() && list_cand.front().size() >= NUMBER_OF_BOX) {
		//初期設定
		State create_start_stat;//初期状態
		SearchStat create_finish_stat;//探索終了状態
		std::string input_level;
		bool create_box = false;
		int cnt_box = 0;
		//規定数配置し終わるまで
		while (!create_box) {
			//配置できる場所が存在しない
			if (list_cand.empty()) {
				break;//終了
			}
			//配置できる場所が足りない
			else if (list_cand.front().size() < NUMBER_OF_BOX) {
				break;//終了
			}
			//座標の決定
			int set = rand() % list_cand.size();
			SQUARE set_square = list_cand[set][cnt_box];
			//ゴール上の荷物を配置
			if (!level.setBoxOnGoal(set_square)) {
				//配置に失敗
				list_cand.erase(list_cand.begin() + set);
				level.resetStage();
				cnt_box = 0;
				break;
			}
			//生成したステージをインプット
			input_level = level.outputString();

			//探索用に初期化
			create_start_stat.state_str = input_level;
			create_start_stat.move_list = "";
			create_start_stat.moves = create_start_stat.pushes =
				create_start_stat.push_lines = create_start_stat.depth =
				create_start_stat.push_direction = 0;

			//一次的に荷物を壁に
			if (cnt_box != 0) {
				//荷物を見つけ壁に置き換え
				std::queue<SQUARE>pos_box;
				SQUARE pos_instant;//キューへプッシュ用
				for (unsigned int i = 0; i < create_start_stat.state_str.size(); i++) {
					if (create_start_stat.state_str[i] == '$') {
						create_start_stat.state_str[i] = '#';
						pos_instant.x = i % (WIDTH + 1);
						pos_instant.y = i / (WIDTH + 1);
						pos_box.push(pos_instant);
					}
				}
				//生成したレベルに対して逆に幅優先探索を行う
				create_finish_stat = choose_search(create_start_stat, BFSR);
				if (create_finish_stat.node.state_str != "NULL\n") {
					//壁を荷物に戻す
					while (!pos_box.empty()) {
						unsigned int i = pos_box.front().y*(WIDTH + 1) + pos_box.front().x;
						create_finish_stat.node.state_str[i] = '$';
						pos_box.pop();
					}
				}
			}
			else {
				//生成したレベルに対して逆に幅優先探索を行う
				create_finish_stat = choose_search(create_start_stat, BFSR);
			}

			//盤面に変更がありNULLでないとき配置成功
			if (create_finish_stat.node.state_str != input_level && create_finish_stat.node.state_str != "NULL\n") {
				cnt_box++;//配置数をカウント
				level.inputString(create_finish_stat.node.state_str);//ステージを更新
			}
			//失敗なら次の組み合わせへ
			else {
				list_cand.erase(list_cand.begin() + set);
				level.resetStage();
				cnt_box = 0;
			}
			//配置数が規定に達したか判定
			if (cnt_box == NUMBER_OF_BOX) {
				create_box = true;
				list_cand.erase(list_cand.begin() + set);
			}
		}
		if (create_finish_stat.node.state_str == "NULL\n") {
			continue;
		}
		else if (!create_box) {
			continue;
		}

		////////////////////////////////
		/*ステージ探索部分*/
		////////////////////////////////
		//levelにインプット
		level.inputString(create_finish_stat.node.state_str);
		//アバターを配置
		level.setPlayer();
		//探索用にセット
		input_level = level.outputString();


		//初期設定
		State init_state;//初期状態
		SearchStat final_stat;//探索終了状態
		//test
		std::cout << "生成されたレベルです。" << std::endl;
		std::cout << input_level;

		//初期化
		init_state.state_str = input_level;
		init_state.move_list = "";
		init_state.moves = init_state.pushes =
			init_state.push_lines = init_state.depth =
			init_state.push_direction = 0;

		//生成したレベルに対して幅優先探索を行う
		final_stat = choose_search(init_state, BFS);
		//クリアチェック
		if (final_stat.node.move_list.empty()) {
			std::cout << "生成されたレベルは解答不可能です。" << std::endl;
			//リセット
			level.resetStage();
			continue;//終了
		}
		//人の移動回数を表示
		std::cout << "このレベルの荷物を動かす最小回数は:" << final_stat.node.pushes
			<< std::endl;
		setReferencePoint(level);
		/*基準点と比較*/
		//プッシュ
		if (final_stat.node.pushes >= moves) {
			//ムーブ
			if (final_stat.node.moves >= pushes) {
				//移動の方向転換
				if (final_stat.node.push_lines >= lines) {
					EVALUATION cur_state;
					cur_state.stage = init_state.state_str;//レベル
					cur_state.pushes = final_stat.node.pushes;//プッシュ
					cur_state.moves = final_stat.node.moves;//ムーブ
					cur_state.change_lines = final_stat.node.push_lines;//移動の方向転換
					//配列にレベルと評価を保存
					better_level.push(cur_state);
					break;//終了
				}
			}
		}
		else {
			EVALUATION cur_state;
			cur_state.stage = init_state.state_str;//レベル
			cur_state.pushes = final_stat.node.pushes;//プッシュ
			cur_state.moves = final_stat.node.moves;//ムーブ
			cur_state.change_lines = final_stat.node.push_lines;//移動の方向転換
			//配列にレベルと評価を保存
			compare.push(cur_state);
		}

		//リセット
		level.resetStage();
	}
	//未発見であればリストを返す
	if (better_level.empty()) {
		return compare;
	}
	else {
		return better_level;
	}
}
//表示用に格納
PROPERTY setProperty(timespec start,std::queue<EVALUATION>compare, int mode) {
	PROPERTY property;
	//スタートを記録
	property.start = start;
	if (mode == BRUTE_FORCE) {
		property.mode_name = "Brute Force Mode";
	}
	else if (mode == MY_MODE) {
		property.mode_name = "My Mode";
	}
	while (!compare.empty()) {
		int value_level = compare.front().pushes + compare.front().moves + compare.front().change_lines;
		//pushが最大となれば更新
		if (property.value < value_level) {
			property.value = value_level;
			property.stage = compare.front().stage;
			property.pushes = compare.front().pushes;
			property.moves = compare.front().moves;
			property.change_lines = compare.front().change_lines;
		}
		//次へ
		compare.pop();
	}
	//タイマーを停止
	timespec_get(&(property.end), TIME_UTC);

	Level level;
	level.inputString(property.stage);
	property.area = level.countSpace();
	return property;
}
//結果を表示
void printResult(PROPERTY property){
	//モード名を表示
	std::cout << property.mode_name << std::endl;
	//評価値を表示
	if (property.value != 0) {
		std::cout << property.stage;
		std::cout << "面積は " << property.area << std::endl;
		std::cout << "評価値は " << property.value << std::endl;
		std::cout << "最短解答手数は " << property.pushes << std::endl;
		std::cout << "最短移動回数は " << property.moves + property.pushes << std::endl;
		std::cout << "最短方向転換数は " << property.change_lines << std::endl;
	}
	else {
		std::cout << property.stage;
	}
	
	//処理時間を表示
	long long sec, nanosec;
	sec = property.end.tv_sec - property.start.tv_sec;
	nanosec = property.end.tv_nsec - property.start.tv_nsec;
	std::cout << (sec + (nanosec / 1000000000.0)) << " seconds" << std::endl;
}

int main(int argc, char** argv) {
	/*初期設定*/
	srand((unsigned int)time(NULL));//乱数設定
	int mode;
	bool valid_choice = true;
	while (valid_choice) {
		std::cout << "モードを選択してください。" << std::endl << "(1:normal,2:research)" << std::endl;
		std::cin >> mode;
		if (mode == NORMAL) {
			valid_choice = false;
		}
		else if (mode == RESEARCH) {
			valid_choice = false;
		}
		else {
			std::cout << "正しい値を入力してください。" << std::endl;
		}
	}//valid_choice
	switch (mode) {
	case NORMAL: {
		bool repeat = true;
		// 生成を繰り返す
		while (repeat) {
			/*空部屋を作成*/
			Level level;
			level.setEmptyRoom();
			//使用する空の部屋を表示
			level.printStage();

			/*２モードで生成*/
			timespec start;
			std::queue<EVALUATION> compare;//レベルの評価用
			PROPERTY brute_force, my_mode;//評価表示用
			/*Brute Force Mode*/
			//時間計測開始
			timespec_get(&start, TIME_UTC);
			std::cout << "Brute Force Mode" << std::endl;
			//総当たりで候補を生成
			compare = runBruteForceMode(level);
			//最良なレベルを選択し、評価値等をセット
			brute_force = setProperty(start, compare, BRUTE_FORCE);
			/*My Mode*/
			//時間計測開始
			timespec_get(&start, TIME_UTC);
			std::cout << "My Mode" << std::endl;
			//基準を満たしたものを生成
			compare = runBruteForceModeSpeedUp(level);
			// 生成不可能な空部屋であればやり直し
			if (compare.empty()) {
				continue;
			}
			//評価値等をセット
			my_mode = setProperty(start, compare, MY_MODE);

			/*最良ステージを表示*/
			printResult(brute_force);
			printResult(my_mode);

			//繰り返し機能を利用するか
			bool valid_input = true;
			while (valid_input) {
				std::string usr_input;
				std::cout << "もう一度実行しますか?[y/n]: ";
				std::cin >> usr_input;
				//有効な入力は、valid_inputをfalseに設定し、ループを中断します
				if (usr_input == "y")
				{
					valid_input = false;
					repeat = true;
				}
				else if (usr_input == "Y")
				{
					valid_input = false;
					repeat = true;
				}
				else if (usr_input == "n")
				{
					valid_input = false;
					repeat = false;
				}
				else if (usr_input == "N")
				{
					valid_input = false;
					repeat = false;
				}
				else
					std::cout << "有効な値を入力してください。  ";
			}
		}//repeat
		break;
	}//case normal
	case RESEARCH: {
		//繰り返し機能を利用するか
		bool valid_input = true;
		int times;
		while (valid_input) {
			std::cout << "何回実行しますか?[1-" << MAX << "] " << std::endl;
			std::cin >> times;
			//有効な入力は、valid_inputをfalseに設定し、ループを中断します
			if (times >0 && times<=MAX){
				valid_input = false;
			}
			else
				std::cout << "有効な値を入力してください。  ";
		}
		std::queue<PROPERTY> result;
		// 生成を繰り返す
		while (result.size() != times) {
			/*空部屋を作成*/
			Level level;
			level.setEmptyRoom();
			//使用する空の部屋を表示
			level.printStage();

			/*Brute Force Mode*/
			timespec start;
			std::queue<EVALUATION> compare;//レベルの評価用
			PROPERTY mode_pro;//評価表示用
			//時間計測開始
			timespec_get(&start, TIME_UTC);
			//総当たりで候補を生成
			compare = runCarefullySelectedMode(level);
			//生成不可能な空部屋であればやり直し
			if (compare.empty()) {
				continue;
			}
			//最良なレベルを選択し、評価値等をセット
			mode_pro = setProperty(start, compare, BRUTE_FORCE);

			result.push(mode_pro);
		}//result.size()!=times
		//実行によって生成されたレベルをすべて表示
		while (!result.empty()) {
			long long now_times = (times + 1) - result.size();
			std::cout << now_times << std::endl;
			/*最良ステージを表示*/
			printResult(result.front());
			result.pop();
		}

		break;
	}//case research
	default: {
		break;
	}
	}//switch(mode)

	return 0;
} //int main(int argc, char** argv)