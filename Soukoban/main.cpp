#include <stdio.h>
#include <iostream>
#include <time.h>
#include <fstream>

#include "define.h"
#include "level.h"
#include "search.h"

int main(int argc, char** argv)
{
	bool repeat = true;

	// whileループを使用して生成と検索アルゴリズムを繰り返します
	while (repeat)
	{
		////////////////////////////////
		/*ステージ生成部分*/
		////////////////////////////////
		//初期設定
		srand((unsigned int)time(NULL));//乱数設定
		Level level;
		//ステージの生成
		level.createLevel();

		////////////////////////////////
		/*ステージ探索部分*/
		////////////////////////////////
		std::string usr_input;
		std::ifstream fs;
		std::string line;
		std::string input_level = level.outputString();//生成したステージをインプット

		//初期設定
		State init_state;
		SearchStat final_stat;
		init_state.state_str = input_level;
		init_state.move_list = "";
		init_state.moves = init_state.pushes =
			init_state.total_cost = init_state.depth =
			init_state.hscore = 0;

		std::cout << "Sokoban level input:" << std::endl;
		std::cout << init_state.state_str;


		bool valid_input = true;

		//生成したレベルに対して幅優先探索を行う
		final_stat = choose_search(init_state, BFS);
		////test
		std::cout << "  Solution: " << std::endl;
		std::cout << "    "
			<< final_stat.node.move_list.substr(0, (final_stat.node.move_list.size() - 2))//末尾の「,」を除外できる
			<< std::endl;
		//ユーザーが繰り返しの有効な選択肢を選択するために使用されるwhileループ
		while (valid_input)
		{
			std::cout << "Again?[y/n]: ";
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
				std::cout << "Invalid choice.  ";
		}

		//生成されたステージを保存するか
		valid_input = true;
		while (valid_input)
		{
			std::cout << "Save this level?[y/n]: ";
			std::cin >> usr_input;
			//有効な入力は、valid_inputをfalseに設定し、ループを中断します
			if (usr_input == "y")
			{
				level.outputStage();
				valid_input = false;
			}
			else if (usr_input == "Y")
			{
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
				std::cout << "Invalid choice.  ";
		}
	}

	return 0;
} //int main(int argc, char** argv)