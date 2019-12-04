#include <stdio.h>
#include <iostream>
#include <time.h>
#include <fstream>
#include <sstream>

#include "define.h"
#include "level.h"
#include "search.h"

//レベルの評価用
struct Evaluation {
	std::string stage;//ステージの内容
	int cnt_pushes;//クリアするのに必要な荷物を押す回数
	int cnt_switching_box;//クリアするのに必要な荷物を押す回数
};

//荷物の移動回数をカウント
int countMovingSolution(State &init_state, std::string solution) {
	unsigned int count = 0;
	int cnt_move = 0;
	State cur_state = init_state;
	std::stringstream ss(init_state.state_str);
	std::vector< std::vector<char> > new_stage;
	State new_state;
	std::string line;
	bool found = false;
	char player, box_move;
	int x, y, counter = 0, MOVE_COST = 1, PUSH_COST = 1;
	//インデックスが移動に使用されるため、ベクトル配列を生成します
	//扱いやすくなります。たとえば、pos [1,2]で下に移動すると[2,2]になります
	// stageは（y、x）座標として保存されます
	std::vector< std::vector<char> > stage;
	while (getline(ss, line, '\n'))
	{
		std::vector<char> temp;
		stage.push_back(temp);
		for (unsigned int i = 0; i < line.length(); i++)
		{
			if (!found)
			{
				//任意の状態でプレイヤーが1人だけ存在すると仮定します
				//ユーザーが見つかった場合、foundをtrueに設定し、x、yの位置
				if (line[i] == '@' || line[i] == '+')
				{
					player = line[i];
					x = i;
					y = counter;
					found = true;
				}
			}
			stage[counter].push_back(line[i]);
		}
		counter++;
	}
	//経路の再現
	while (count < solution.size()) {
		//初期設定
		char move_direction = solution[count];
		char north = stage[y - 1][x];
		char east = stage[y][x + 1];
		char south = stage[y + 1][x];
		char west = stage[y][x - 1];
		switch (move_direction) {
		case'u':
			//北の動きを生成
			switch (north)
			{
				//空の場所に移動します
			case ' ':
				new_stage = stage;
				//プレーヤーのタイルとプレーヤーの北のタイルの調整
				new_stage[y - 1][x] = '@';
				//プレイヤー= @の場合、空の床に立っていた
				//そうでなければ、プレーヤーはゴールに立っていた
				(player == '@') ? new_stage[y][x] = ' ' : new_stage[y][x] = '.';

				//変更した状態を現在の状態へ
				y--;
				stage = new_stage;
				player = stage[y][x];
				break;
				//空の目標に移動します
			case '.':
				new_stage = stage;
				//プレーヤーのタイルとプレーヤーの北のタイルの調整
				new_stage[y - 1][x] = '+';
				(player == '@') ? new_stage[y][x] = ' ' : new_stage[y][x] = '.';

				//変更した状態を現在の状態へ
				y--;
				stage = new_stage;
				player = stage[y][x];
				break;
				//フロアーのボックスに移動します
			case '$':
				new_stage = stage;
				//プレーヤーのタイルとプレーヤーの北のタイルの調整
				new_stage[y - 1][x] = '@';
				(player == '@') ? new_stage[y][x] = ' ' : new_stage[y][x] = '.';

				//ボックスタイルとボックスの北のタイルを調整します
				box_move = new_stage[y - 2][x];
				//ボックスの北が壁または別のボックスの場合
				if (box_move == '#')
					break;
				else if (box_move == '$')
					break;
				else if (box_move == '*')
					break;
				//ボックスの北が空の床である場合
				else if (box_move == ' ')
				{
					new_stage[y - 2][x] = '$';
				}
				//ボックスの北が空のゴールの場合
				else if (box_move == '.')
				{
					new_stage[y - 2][x] = '*';
				}
				else
					break;

				//箱を移動させたらカウント
				cnt_move++;
				//変更した状態を現在の状態へ
				y--;
				stage = new_stage;
				player = stage[y][x];
				break;
				//ゴールでボックスに移動
			case '*':
				new_stage = stage;
				//プレーヤーのタイルとプレーヤーの北のタイルの調整
				new_stage[y - 1][x] = '+';
				(player == '@') ? new_stage[y][x] = ' ' : new_stage[y][x] = '.';

				//ボックスタイルとボックスの北のタイルを調整します
				box_move = new_stage[y - 2][x];
				//ボックスの北が壁または別のボックスの場合
				if (box_move == '#')
					break;
				else if (box_move == '$')
					break;
				else if (box_move == '*')
					break;
				//ボックスの北が空の床である場合
				else if (box_move == ' ')
				{
					new_stage[y - 2][x] = '$';
				}
				//ボックスの北が空のゴールの場合
				else if (box_move == '.')
				{
					new_stage[y - 2][x] = '*';
				}
				else
					break;

				//箱を移動させたらカウント
				cnt_move++;
				//変更した状態を現在の状態へ
				y--;
				stage = new_stage;
				player = stage[y][x];
				break;
				//壁に移動します
			case '#':
				break;
			default:
				break;
			}
			break;
		case'r':
			//東の動きを生成します
			switch (east)
			{
				//空の場所に移動します
			case ' ':
				new_stage = stage;
				//プレーヤーのタイルとプレーヤーの東のタイルを調整します
				new_stage[y][x + 1] = '@';
				//プレイヤー= @の場合、空の床に立っていた
				//そうでなければ、プレーヤーはゴールに立っていた
				(player == '@') ? new_stage[y][x] = ' ' : new_stage[y][x] = '.';

				//変更した状態を現在の状態へ
				x++;
				stage = new_stage;
				player = stage[y][x];
				break;
				//空の目標に移動します
			case '.':
				new_stage = stage;
				//プレーヤーのタイルとプレーヤーの東のタイルを調整します
				new_stage[y][x + 1] = '+';
				(player == '@') ? new_stage[y][x] = ' ' : new_stage[y][x] = '.';

				//変更した状態を現在の状態へ
				x++;
				stage = new_stage;
				player = stage[y][x];
				break;
				//フロアーのボックスに移動します
			case '$':
				new_stage = stage;
				//プレーヤーのタイルとプレーヤーの東のタイルを調整します
				new_stage[y][x + 1] = '@';
				(player == '@') ? new_stage[y][x] = ' ' : new_stage[y][x] = '.';

				//ボックスタイルとボックスの東のタイルの調整
				box_move = new_stage[y][x + 2];
				//ボックスの東が壁または別のボックスである場合
				if (box_move == '#')
					break;
				else if (box_move == '$')
					break;
				else if (box_move == '*')
					break;
				//ボックスの東が空の床の場合
				else if (box_move == ' ')
				{
					new_stage[y][x + 2] = '$';
				}
				//ボックスの東が空のゴールの場合
				else if (box_move == '.')
				{
					new_stage[y][x + 2] = '*';
				}
				else
					break;

				//箱を移動させたらカウント
				cnt_move++;
				//変更した状態を現在の状態へ
				x++;
				stage = new_stage;
				player = stage[y][x];
				break;
				//ゴールでボックスに移動
			case '*':
				new_stage = stage;
				//プレーヤーのタイルとプレーヤーの東のタイルを調整します
				new_stage[y][x + 1] = '+';
				(player == '@') ? new_stage[y][x] = ' ' : new_stage[y][x] = '.';

				//ボックスタイルとボックスの東のタイルの調整
				box_move = new_stage[y][x + 2];
				//ボックスの東が壁または別のボックスである場合
				if (box_move == '#')
					break;
				else if (box_move == '$')
					break;
				else if (box_move == '*')
					break;
				//ボックスの東が空の床の場合
				else if (box_move == ' ')
				{
					new_stage[y][x + 2] = '$';
				}
				//ボックスの東が空のゴールの場合
				else if (box_move == '.')
				{
					new_stage[y][x + 2] = '*';
				}
				else
					break;

				//箱を移動させたらカウント
				cnt_move++;
				//変更した状態を現在の状態へ
				x++;
				stage = new_stage;
				player = stage[y][x];
				break;
				//壁に移動します
			case '#':
				break;
			default:
				break;
			}
			break;
		case'd':
			//南に移動します
			switch (south)
			{
				//空の場所に移動します
			case ' ':
				new_stage = stage;
				//プレーヤーのタイルとプレーヤーの南のタイルの調整
				new_stage[y + 1][x] = '@';
				//プレイヤー= @の場合、空の床に立っていた
				//そうでなければ、プレーヤーはゴールに立っていた
				(player == '@') ? new_stage[y][x] = ' ' : new_stage[y][x] = '.';

				//変更した状態を現在の状態へ
				y++;
				stage = new_stage;
				player = stage[y][x];
				break;
				//空の目標に移動します
			case '.':
				new_stage = stage;
				//プレーヤーのタイルとプレーヤーの南のタイルの調整
				new_stage[y + 1][x] = '+';
				(player == '@') ? new_stage[y][x] = ' ' : new_stage[y][x] = '.';

				//変更した状態を現在の状態へ
				y++;
				stage = new_stage;
				player = stage[y][x];
				break;
				//フロアーのボックスに移動します
			case '$':
				new_stage = stage;
				//プレーヤーのタイルとプレーヤーの南のタイルの調整
				new_stage[y + 1][x] = '@';
				(player == '@') ? new_stage[y][x] = ' ' : new_stage[y][x] = '.';

				//ボックスタイルとボックスの南側のタイルの調整
				box_move = new_stage[y + 2][x];
				//ボックスの南が壁または別のボックスである場合
				if (box_move == '#')
					break;
				else if (box_move == '$')
					break;
				else if (box_move == '*')
					break;
				//ボックスの南が空の床の場合
				else if (box_move == ' ')
				{
					new_stage[y + 2][x] = '$';
				}
				//ボックスの南が空のゴールの場合
				else if (box_move == '.')
				{
					new_stage[y + 2][x] = '*';
				}
				else
					break;

				//箱を移動させたらカウント
				cnt_move++;
				//変更した状態を現在の状態へ
				y++;
				stage = new_stage;
				player = stage[y][x];
				break;
				//ゴールでボックスに移動
			case '*':
				new_stage = stage;
				//プレーヤーのタイルとプレーヤーの南のタイルの調整
				new_stage[y + 1][x] = '+';
				(player == '@') ? new_stage[y][x] = ' ' : new_stage[y][x] = '.';

				//ボックスタイルとボックスの南側のタイルの調整
				box_move = new_stage[y + 2][x];
				//ボックスの南が壁または別のボックスである場合
				if (box_move == '#')
					break;
				else if (box_move == '$')
					break;
				else if (box_move == '*')
					break;
				//ボックスの南が空の床の場合
				else if (box_move == ' ')
				{
					new_stage[y + 2][x] = '$';
				}
				//ボックスの南が空のゴールの場合
				else if (box_move == '.')
				{
					new_stage[y + 2][x] = '*';
				}
				else
					break;

				//箱を移動させたらカウント
				cnt_move++;
				//変更した状態を現在の状態へ
				y++;
				stage = new_stage;
				player = stage[y][x];
				break;
				//壁に移動します
			case '#':
				break;
			default:
				break;
			}
			break;
		case'l':
			//西の動きを生成します
			switch (west)
			{
				//空の場所に移動します
			case ' ':
				new_stage = stage;
				//プレーヤーのタイルとプレーヤーの西のタイルの調整
				new_stage[y][x - 1] = '@';
				//プレイヤー= @の場合、空の床に立っていた
				//そうでなければ、プレーヤーはゴールに立っていた
				(player == '@') ? new_stage[y][x] = ' ' : new_stage[y][x] = '.';

				//変更した状態を現在の状態へ
				x--;
				stage = new_stage;
				player = stage[y][x];
				break;
				//空の目標に移動します
			case '.':
				new_stage = stage;
				//プレーヤーのタイルとプレーヤーの西のタイルの調整
				new_stage[y][x - 1] = '+';
				(player == '@') ? new_stage[y][x] = ' ' : new_stage[y][x] = '.';

				//変更した状態を現在の状態へ
				x--;
				stage = new_stage;
				player = stage[y][x];
				break;
				//階のボックスに移動します
			case '$':
				new_stage = stage;
				//プレーヤーのタイルとプレーヤーの西のタイルの調整
				new_stage[y][x - 1] = '@';
				(player == '@') ? new_stage[y][x] = ' ' : new_stage[y][x] = '.';

				//ボックスタイルとボックスの西側のタイルの調整
				box_move = new_stage[y][x - 2];
				//ボックスの西が壁または別のボックスの場合
				if (box_move == '#')
					break;
				else if (box_move == '$')
					break;
				else if (box_move == '*')
					break;
				//ボックスの西が空の床の場合
				else if (box_move == ' ')
				{
					new_stage[y][x - 2] = '$';
				}
				//ボックスの西が空のゴールの場合
				else if (box_move == '.')
				{
					new_stage[y][x - 2] = '*';
				}
				else
					break;

				//箱を移動させたらカウント
				cnt_move++;
				//変更した状態を現在の状態へ
				x--;
				stage = new_stage;
				player = stage[y][x];
				break;
				//ゴールでボックスに移動
			case '*':
				new_stage = stage;
				//プレーヤーのタイルとプレーヤーの西のタイルの調整
				new_stage[y][x - 1] = '+';
				(player == '@') ? new_stage[y][x] = ' ' : new_stage[y][x] = '.';

				//ボックスタイルとボックスの西側のタイルの調整
				box_move = new_stage[y][x - 2];
				//ボックスの西が壁または別のボックスの場合
				if (box_move == '#')
					break;
				else if (box_move == '$')
					break;
				else if (box_move == '*')
					break;
				//ボックスの西が空の床の場合
				else if (box_move == ' ')
				{
					new_stage[y][x - 2] = '$';
				}
				//ボックスの西が空のゴールの場合
				else if (box_move == '.')
				{
					new_stage[y][x - 2] = '*';
				}
				else
					break;

				//箱を移動させたらカウント
				cnt_move++;
				//変更した状態を現在の状態へ
				x--;
				stage = new_stage;
				player = stage[y][x];
				break;
				//壁に移動します
			case '#':
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
		count += 3;
	}

	return cnt_move;
}

int main(int argc, char** argv)
{
	//初期設定
	srand((unsigned int)time(NULL));//乱数設定
	bool repeat = true;

	// whileループを使用して生成と検索アルゴリズムを繰り返します
	while (repeat)
	{
		//初期設定
		bool compare_end = false;
		Level level;
		std::queue<Evaluation> compare;//レベルの評価用
		
		//ステージの生成
		level.setEmptyRoom();

		//ゴールと荷物の配置を比較
		while (!compare_end) {
			////////////////////////////////
			/*ステージ生成部分*/
			////////////////////////////////
			level.setStage();
			////////////////////////////////
			/*ステージ探索部分*/
			////////////////////////////////
			std::ifstream fs;
			std::string line;
			std::string input_level = level.outputString();//生成したステージをインプット

			//初期設定
			State init_state;//初期状態
			SearchStat final_stat;//探索終了状態
			//初期化
			init_state.state_str = input_level;
			init_state.move_list = "";
			init_state.moves = init_state.pushes =
				init_state.total_cost = init_state.depth =
				init_state.hscore = 0;

			std::cout << "生成しました" << std::endl;
			std::cout << init_state.state_str;

			//生成したレベルに対して幅優先探索を行う
			final_stat = choose_search(init_state, BFS);
			//クリアチェック
			if (final_stat.node.move_list.empty()) {
				std::cout << "生成されたレベルは解答不可能です。" << std::endl;
				std::cout << "新しいレベルを作り直します。" << std::endl;
				continue;
			}
			//人の移動回数を表示
			std::cout << "このレベルの荷物を動かす最小回数は:" << countMovingSolution(init_state, final_stat.node.move_list.substr(0, (final_stat.node.move_list.size() - 2)))
				<< std::endl;
			/////////////
			/*比較部分*/
			////////////
			Evaluation cur_state;
			cur_state.stage = init_state.state_str;//レベル
			cur_state.cnt_pushes = countMovingSolution(init_state, final_stat.node.move_list.substr(0, (final_stat.node.move_list.size() - 2)));//プッシュ
			//配列にレベルと評価を保存
			compare.push(cur_state);

			//リセット
			level.resetStage();

			//3つ保存したら終了
			if (compare.size() == 1) {
				compare_end = true;
			}
		}
		//全て表示
		while (!compare.empty()) {
			std::cout << compare.front().stage;
			compare.pop();
		}

		//ユーザーが繰り返しの有効な選択肢を選択するために使用されるwhileループ
		bool valid_input = true;
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
		//生成されたステージを保存するか
		valid_input = true;
		while (valid_input)
		{
			std::string usr_input;
			std::cout << "このレベルを保存しますか?[y/n]: ";
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
				std::cout << "有効な値を入力してください。  ";
		}
	}

	return 0;
} //int main(int argc, char** argv)