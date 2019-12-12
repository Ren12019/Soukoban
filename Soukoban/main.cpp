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

//生成モード
enum Mode { BRUTE_FORCE, MY_MODE, NUM_MODE };
//問題評価の基準点数
enum Border { MOVES = 18, PUSHES = 8, LINES = 4, TOTAL_VAL = 30 };
//レベルの評価用
struct EVALUATION {
	std::string stage;//ステージの内容
	int pushes;//クリアするのに必要な荷物を押す回数
	int moves;//クリアするのに必要なアバターの移動回数
	int change_lines;//クリアするのに必要な荷物を押す回数
};
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

int main(int argc, char** argv){
	//初期設定
	srand((unsigned int)time(NULL));//乱数設定
	bool repeat = true;

	// 生成と検索アルゴリズムを繰り返します
	while (repeat) {
		//初期設定
		Level level;
		std::queue<EVALUATION> compare;//レベルの評価用
		std::queue<EVALUATION> better_level;//レベルの評価用
		std::vector<SQUARE> candidate;//ゴールの候補地のキュー
		timespec start1, end1;
		timespec start2, end2;
		int best_value1 = 0;
		int best_pushes1 = 0;
		int best_moves1 = 0;
		int best_chages1 = 0;
		std::string best_stage1 = "この空の部屋では生成できません\n";
		int best_value2 = 0;
		int best_pushes2 = 0;
		int best_moves2 = 0;
		int best_chages2 = 0;
		std::string best_stage2 = "この空の部屋では生成できません\n";
		//時間計測開始
		//timespec_get(&start, TIME_UTC);
		////////////////////////////////
		/*ステージ生成部分*/
		////////////////////////////////
		/*空の部屋の生成*/
		level.setEmptyRoom();
		//使用する空の部屋を表示
		level.printStage();
		//ゴールが配置可能な場所を配列に収納
		candidate = level.decisionCanditdate();
		//２モードで生成
		int cnt_mode = BRUTE_FORCE;
		while (cnt_mode < NUM_MODE) {
			switch (cnt_mode) {
			case BRUTE_FORCE: {
				timespec_get(&start1, TIME_UTC);
				std::cout << "Brute Force Mode" << std::endl;
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
				break;
			}
			case MY_MODE: {
				level.resetStage();
				timespec_get(&start2, TIME_UTC);
				std::cout << "My Mode" << std::endl;
				std::vector<std::vector<SQUARE>>list_cand;//ゴールの候補地の組み合わせのリスト
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

					/*基準点と比較*/
					//プッシュ
					if (final_stat.node.pushes >= PUSHES) {
						//ムーブ
						if (final_stat.node.moves >= MOVES) {
							//移動の方向転換
							if (final_stat.node.push_lines >= LINES) {
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
				break;
			}
			default:
				break;
			}
			
			////////////////////////////////
			/*比較部分*/
			////////////////////////////////
			//Brute Force
			if (cnt_mode == BRUTE_FORCE) {
				while (!compare.empty()) {
					int value_level = compare.front().pushes + compare.front().moves + compare.front().change_lines;
					//pushが最大となれば更新
					if (best_value1 < value_level) {
						best_value1 = value_level;
						best_stage1 = compare.front().stage;
						best_pushes1 = compare.front().pushes;
						best_moves1 = compare.front().moves;
						best_chages1 = compare.front().change_lines;
					}
					//次へ
					compare.pop();
				}
				//タイマーを停止
				timespec_get(&end1, TIME_UTC);
			}
			//My Mode
			else if (cnt_mode == MY_MODE) {
				if (!better_level.empty()) {
					int value_level = better_level.front().pushes + better_level.front().moves + better_level.front().change_lines;
					best_value2 = value_level;
					best_stage2 = better_level.front().stage;
					best_pushes2 = better_level.front().pushes;
					best_moves2 = better_level.front().moves;
					best_chages2 = better_level.front().change_lines;
					std::cout << "基準を超えるレベルが見つかりました。" << std::endl;
				}
				else
				{
					while (!compare.empty()) {
						int value_level = compare.front().pushes + compare.front().moves + compare.front().change_lines;
						//pushが最大となれば更新
						if (best_value2 < value_level) {
							best_value2 = value_level;
							best_stage2 = compare.front().stage;
							best_pushes2 = compare.front().pushes;
							best_moves2 = compare.front().moves;
							best_chages2 = compare.front().change_lines;
						}
						//次へ
						compare.pop();
					}
					std::cout << "基準を超えるレベルは見つかりませんでした。" << std::endl;
				}
				//タイマーを停止
				timespec_get(&end2, TIME_UTC);
			}
			cnt_mode++;
		}//2mode

		/*最良ステージを表示*/
		std::cout << "Brute Force Mode" << std::endl;
		if (best_value1 != 0) {
			std::cout << best_stage1;
			std::cout << "評価値は " << best_value1 << std::endl;
			std::cout << "最短解答手数は " << best_pushes1 << std::endl;
			std::cout << "最短移動回数は " << best_moves1 + best_pushes1 << std::endl;
			std::cout << "最短方向転換数は " << best_chages1 << std::endl;
		}
		else {
			std::cout << best_stage1;
		}
		std::cout << "My Mode" << std::endl;
		if (best_value2 != 0) {
			std::cout << best_stage2;
			std::cout << "評価値は " << best_value2 << std::endl;
			std::cout << "最短解答手数は " << best_pushes2 << std::endl;
			std::cout << "最短移動回数は " << best_moves2 + best_pushes2 << std::endl;
			std::cout << "最短方向転換数は " << best_chages2 << std::endl;
		}
		else {
			std::cout << best_stage2;
		}
		/*処理時間を表示*/
		long long sec, nanosec;
		std::cout << "Brute Force Mode" << std::endl;
		std::cout << "所要時間: ";
		sec = end1.tv_sec - start1.tv_sec;
		nanosec = end1.tv_nsec - start1.tv_nsec;
		std::cout << (sec + (nanosec / 1000000000.0)) << " seconds" << std::endl;
		std::cout << "My Mode" << std::endl;
		std::cout << "所要時間: ";
		sec = end2.tv_sec - start2.tv_sec;
		nanosec = end2.tv_nsec - start2.tv_nsec;
		std::cout << (sec + (nanosec / 1000000000.0)) << " seconds" << std::endl;
		//ユーザーが繰り返しの有効な選択肢を選択するために使用されるwhileループ
		bool valid_input = true;
		/*生成されたステージを保存するか*/
		//while (valid_input)
		//{
		//	std::string usr_input;
		//	std::cout << "このレベルを保存しますか?[y/n]: ";
		//	std::cin >> usr_input;
		//	//有効な入力は、valid_inputをfalseに設定し、ループを中断します
		//	if (usr_input == "y")
		//	{
		//		level.inputString(best_stage);
		//		level.outputStage();
		//		valid_input = false;
		//	}
		//	else if (usr_input == "Y")
		//	{
		//		level.inputString(best_stage);
		//		level.outputStage();
		//		valid_input = false;
		//	}
		//	else if (usr_input == "n")
		//	{
		//		valid_input = false;
		//	}
		//	else if (usr_input == "N")
		//	{
		//		valid_input = false;
		//	}
		//	else
		//		std::cout << "有効な値を入力してください。  ";
		//}

		//繰り返し機能を利用するか
		valid_input = true;
		while (valid_input){
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

	return 0;
} //int main(int argc, char** argv)