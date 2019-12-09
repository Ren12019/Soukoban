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

//レベルの評価用
struct Evaluation {
	std::string stage;//ステージの内容
	int pushes;//クリアするのに必要な荷物を押す回数
	int moves;//クリアするのに必要なアバターの移動回数
	int change_lines;//クリアするのに必要な荷物を押す回数
};
//座標の組み合わせをリスト化
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

void printList(std::queue<std::vector<SQUARE>>list) {
	int count = 1;
	while (!list.empty()) {
		std::cout << count << std::endl;
		for (unsigned int i = 0; i < list.front().size(); i++) {
			std::cout << "(" << list.front()[i].y << "," << list.front()[i].x << ")" << std::endl;
		}
		list.pop();
		count++;
	}
}

int main(int argc, char** argv)
{
	//初期設定
	srand(6);//乱数設定test

	//srand((unsigned int)time(NULL));//乱数設定
	bool repeat = true;
	
	// whileループを使用して生成と検索アルゴリズムを繰り返します
	while (repeat){
		//初期設定
		Level level;
		std::queue<Evaluation> compare;//レベルの評価用
		std::vector<SQUARE> candidate;//ゴールの候補地のキュー
		std::queue<std::vector<SQUARE>>list_cand;//ゴールの候補地の組み合わせのリスト
		timespec start, end;
		//時間計測開始
		timespec_get(&start, TIME_UTC);
		////////////////////////////////
		/*ステージ生成部分*/
		////////////////////////////////
		/*空の部屋の生成*/
		level.setEmptyRoom();
		//使用する空の部屋を表示
		level.printStage();
		//ゴールが配置可能な場所を配列に収納
		candidate = level.decisionCanditdate();
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
					//printf("配置できる場所が存在しません。\n");
					break;//終了
				}
				//配置できる場所が足りない
				else if (list_cand.front().size()< NUMBER_OF_BOX) {
					//printf("配置できる場所が足りません。\n");
					break;//終了
				}
				//座標の決定
				//int num = rand() % candidate.size();
				SQUARE set_square = list_cand.front()[cnt_box];
				//ゴール上の荷物を配置
				if (!level.setBoxOnGoal(set_square)) {
					//配置に失敗
					list_cand.pop();
					level.resetStage();
					cnt_box = 0;
					break;
				}
				//candidate.erase(candidate.begin() + num);
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
				if (create_finish_stat.node.state_str != create_start_stat.state_str && create_finish_stat.node.state_str != "NULL\n") {
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
				//std::cout << "新しいレベルを作り直します。" << std::endl;
				//リセット
				level.resetStage();
				continue;//終了
			}
			//人の移動回数を表示
			std::cout << "このレベルの荷物を動かす最小回数は:" << final_stat.node.pushes
				<< std::endl;

			/*比較用に保存*/
			Evaluation cur_state;
			cur_state.stage = init_state.state_str;//レベル
			cur_state.pushes =final_stat.node.pushes;//プッシュ
			cur_state.moves = final_stat.node.moves;//ムーブ
			cur_state.change_lines = final_stat.node.push_lines;//移動の方向転換
			//配列にレベルと評価を保存
			compare.push(cur_state);
			//リセット
			level.resetStage();
		}
		//全て表示
#if 0		
		while (!compare.empty()) {
			std::cout << compare.front().stage;
			compare.pop();
		}

		//一番いいものを表示
#else 
		////////////////////////////////
		/*比較部分*/
		////////////////////////////////
		int best_value = 0;
		int best_pushes = 0;
		int best_moves = 0;
		int best_chages = 0;
		std::string best_stage = "この空の部屋では生成できません\n";
		while (!compare.empty()) {
			int value_level = compare.front().pushes + compare.front().moves + compare.front().change_lines;
			//pushが最大となれば更新
			if (best_value < value_level) {
				best_value = value_level;
				best_stage = compare.front().stage;
				best_pushes = compare.front().pushes;
				best_moves = compare.front().moves;
				best_chages = compare.front().change_lines;
			}
			//次へ
			compare.pop();
		}
		//最良ステージを表示
		if (best_value != 0) {
			std::cout << "ベストな配置です。" << std::endl;
			std::cout << best_stage;
			std::cout << "評価値は " << best_value << std::endl;
			std::cout << "最短解答手数は " << best_pushes << std::endl;
			std::cout << "最短移動回数は " << best_moves + best_pushes << std::endl;
			std::cout << "最短方向転換数は " << best_chages << std::endl;
		}
		else {
			std::cout << best_stage;
		}
#endif
		//生成時間
		long long sec, nanosec;
		timespec_get(&end, TIME_UTC);
		std::cout << "  所要時間: ";
		sec = end.tv_sec - start.tv_sec;
		nanosec = end.tv_nsec - start.tv_nsec;
		std::cout << (sec + (nanosec / 1000000000.0)) << " seconds" << std::endl;
		//ユーザーが繰り返しの有効な選択肢を選択するために使用されるwhileループ
		bool valid_input = true;
		//生成されたステージを保存するか
#if 1
		while (valid_input)
		{
			std::string usr_input;
			std::cout << "このレベルを保存しますか?[y/n]: ";
			std::cin >> usr_input;
			//有効な入力は、valid_inputをfalseに設定し、ループを中断します
			if (usr_input == "y")
			{
				level.inputString(best_stage);
				level.outputStage();
				valid_input = false;
			}
			else if (usr_input == "Y")
			{
				level.inputString(best_stage);
				level.outputStage();
				valid_input = false;
			}
			else if (usr_input == "n")
			{
				valid_input = false;
			}
			else if (usr_input == "N")
			{
				valid_input = false;
			}
			else
				std::cout << "有効な値を入力してください。  ";
		}
		valid_input = true;

#endif
		//繰り返し機能を利用するか
		while (valid_input)
		{
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
	}

	return 0;
} //int main(int argc, char** argv)