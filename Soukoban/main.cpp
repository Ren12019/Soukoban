#include <stdio.h>
#include <iostream>
#include <time.h>
#include <fstream>

#include "define.h"
#include "level.h"
#include "search.h"

int main(int argc, char** argv)
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
	bool repeat = true;
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

	// whileループを使用して検索アルゴリズムを繰り返します
	while (repeat)
	{
		bool loop = true, valid_input = true;

		//有効な検索アルゴリズムの選択を確認します
		while (loop)
		{
			//検索の選択肢を選択するようユーザーに要求します
			std::cout << "\nMenu:\n"
				<< "  1) Breadth first search\n"
				<< "  2) Depth first search\n"
				<< "  3) Uniform cost search\n"
				<< "  4) Greedy best first search\n"
				<< "  5) A* search"
				<< std::endl;

			std::cin >> usr_input;
			if (usr_input == "1")
			{
				choose_search(init_state, BFS);
				valid_input = true;
				loop = false;
			}
			else if (usr_input == "2")
			{
				choose_search(init_state, DFS);
				valid_input = true;
				loop = false;
			}
			else if (usr_input == "3")
			{
				choose_search(init_state, UCS);
				valid_input = true;
				loop = false;
			}
			else if (usr_input == "4")
			{
				std::cout << "  Choose heuristics function 1 or 2 for Greedy Best First Search: ";
				std::cin >> usr_input;
				if (usr_input == "1")
				{
					choose_search(init_state, GBFSH1);
					valid_input = true;
					loop = false;
				}
				else if (usr_input == "2")
				{
					choose_search(init_state, GBFSH2);
					valid_input = true;
					loop = false;
				}
				else
				{
					std::cout << "Invalid heuristics choice." << std::endl;
					valid_input = false;
				}
			}
			else if (usr_input == "5")
			{
				std::cout << "  Choose heuristics function 1 or 2: ";
				std::cin >> usr_input;
				if (usr_input == "1")
				{
					choose_search(init_state, ASH1);
					valid_input = true;
					loop = false;
				}
				else if (usr_input == "2")
				{
					choose_search(init_state, ASH2);
					valid_input = true;
					loop = false;
				}
				else
				{
					std::cout << "Invalid heuristics choice." << std::endl;
					valid_input = false;
				}
			}
			else
			{
				std::cout << "Invalid choice.  Please choose again." << std::endl;
				valid_input = false;
			}
		}

		//ユーザーが繰り返しの有効な選択肢を選択するために使用されるwhileループ
		while (valid_input)
		{
			std::cout << "Choose another search algorithm?[y/n]: ";
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