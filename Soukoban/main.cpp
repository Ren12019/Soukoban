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

	// whileループを使用して検索アルゴリズムを繰り返します
	while (repeat)
	{
		////////////////////////////////
		/*ステージ生成部分*/
		////////////////////////////////
		//初期設定
		srand((unsigned int)time(NULL));//乱数設定
		Level level;
		level.createLevel();
		level.printStage();
		level.outputStage();

		////////////////////////////////
		/*ステージ探索部分*/
		////////////////////////////////
		int level_size;
		std::string usr_input;
		std::ifstream fs;
		std::string line;
		std::string input_level = "";

		//引数が存在するかどうかを確認します。txtファイルがない場合はレベルを入力できません
		if (argc != 2)
		{
			std::cerr << "  usage: " << argv[0] << "<sokoban_level>.txt" << std::endl;
			return 0;
		}
		//倉庫番レベルのtxtファイルを開き、文字列として保存します
		fs.open(argv[1]);
		if (!fs)
		{
			std::cerr << "  error opening file " << argv[1] << std::endl;
			return 0;
		}

		//入力レベルの最初の行から配列のサイズを取得します
		std::getline(fs, line, '\n');
		level_size = atoi(line.c_str());

		//文字列に行を追加します
		while (std::getline(fs, line))
		{
			input_level.append(line) += "\n";
		}
		fs.close();
		//初期設定
		State init_state;
		init_state.state_str = input_level;
		init_state.move_list = "";
		init_state.moves = init_state.pushes =
			init_state.total_cost = init_state.depth =
			init_state.hscore = 0;

		std::cout << "Sokoban level input:" << std::endl;
		std::cout << level_size << std::endl;
		std::cout << init_state.state_str;


		bool valid_input = true;

		//生成したレベルに対して幅優先探索を行う
		choose_search(init_state, BFS);

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
	}

	return 0;
} //int main(int argc, char** argv)