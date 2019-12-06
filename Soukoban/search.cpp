#include "search.h"
#include <iostream>
#include <time.h>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <queue>
#include <deque>
#include <string>
#include <fstream>
#include "define.h"


//クリア状態から遠ざける
std::queue<State> gen_valid_states_reverse(const State &cur_state)
{
	std::queue<State> valid_moves;
	std::stringstream ss(cur_state.state_str);
	std::vector<std::vector<char>> new_stage;
	State new_state;
	std::string line;
	bool found = false;
	char box_on_goal;
	int x = 0, y = 0, counter = 0, MOVE_COST = 1, PUSH_COST = 1;

	//インデックスが移動に使用されるため、ベクトル配列を生成します
	//扱いやすくなります。たとえば、pos [1,2]で下に移動すると[2,2]になります
	// stageは（y、x）座標として保存されます
	std::vector<std::vector<char>> stage;
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
				if (line[i] == '$' || line[i] == '*')
				{
					box_on_goal = line[i];
					x = i;
					y = counter;
					found = true;
				}
			}
			stage[counter].push_back(line[i]);
		}
		counter++;
	}
	if (!found)
	{
		std::cout << "No box found on level" << std::endl;
		return valid_moves;
	}
	//全方位の移動可能個所をキューへ
	int dx[4] = { 0, 1, 0, -1 };
	int dy[4] = { -1, 0, 1, 0 };
	for (unsigned int i=0; i < 4; i++)
	{
		//移動先
		int next_x = x + dx[i];
		int next_y = y + dy[i];
		//範囲内
		if (next_x < 0 || next_x > WIDTH - 1 || next_y < 0 || next_y > HEIGHT - 1)
			continue;

		char direction = stage[next_y][next_x];
		if (direction == ' ')
		{
			//移動先
			int pos_man_x = next_x + dx[i];
			int pos_man_y = next_y + dy[i];
			//範囲内
			if (pos_man_x < 0 || pos_man_x > WIDTH - 1 || pos_man_y < 0 || pos_man_y > HEIGHT - 1)
				continue;

			char pos_man = stage[pos_man_y][pos_man_x];
			//荷物を押すためのスペース
			if (pos_man == ' ')
			{
				new_stage = stage;
				//荷物のタイルと荷物の北のタイルの調整
				new_stage[next_y][next_x] = '$';
				//荷物= $の場合、空の床に立っていた
				//そうでなければ、荷物はゴールに立っていた
				(box_on_goal == '$') ? new_stage[y][x] = ' ' : new_stage[y][x] = '.';

				//新しい状態を作成および更新します
				new_state = cur_state;
				new_state.state_str = "";
				// vector <vector <char >>を文字列に戻す
				for (unsigned int i = 0; i < new_stage.size(); i++)
				{
					std::vector<char> temp = new_stage[i];
					std::vector<char>::iterator itr;
					for (itr = temp.begin(); itr != temp.end(); itr++)
						new_state.state_str.push_back(*itr);
					new_state.state_str.append("\n");
				}

				//状態の統計を更新します
				switch (i)
				{
				case NORTH:
					new_state.move_list.append("u, ");
					break;
				case EAST:
					new_state.move_list.append("r, ");
					break;
				case SOUTH:
					new_state.move_list.append("d, ");
					break;
				case WEST:
					new_state.move_list.append("l, ");
					break;
				default:
					break;
				}
				new_state.depth++;
				new_state.moves++;
				valid_moves.push(new_state);
			}
		}
	}

	return valid_moves;
} //std::queue<State> gen_valid_states_reverse (const State &cur_state)

SearchStat bfs_reverse(State &initial_state)
{
	std::deque<State> open;
	std::vector<State> closed;
	SearchStat report;
	report.rep_node_count = 0;
	report.fringe_node = 0;
	report.explored_count = 1; //will be replaced, just to stop cout spam
	report.node_count = 1;
	report.node.state_str = "NULL\n";
	State current_state;

	//最初の状態をキューにプッシュします
	open.push_back(initial_state);
	//探索部分
	while (!open.empty())
	{
		// OPENからNを取得します
		current_state = open.front();
		open.pop_front();
		// NをCLOSEDにプッシュします
		closed.push_back(current_state); //探索済み
		std::cout<<current_state.state_str;
		//時間がかかった場合に出力し、フリーズしたかどうか確認する
		if ((closed.size() % 5000) == 0)
			std::cout << "...explored " << closed.size() << " nodes..." << std::endl;

		//有効な状態を生成します
		std::queue<State> valid_states = gen_valid_states_reverse(current_state);

		if (valid_states.empty()) {
			report.node = current_state;
			report.explored_count = (int)closed.size();
			break;
		}

		std::deque<State>::iterator it;
		std::vector<State>::iterator itr;

		//キューの状態が空でない間
		//移動可能な状態をキューへ
		while (!valid_states.empty())
		{
			bool already_seen = false;
			State temp_state = valid_states.front();
			//状態が既にオープンリストに表示されているかどうかを確認します
			for (it = open.begin(); it != open.end(); it++)
			{
				if (it->state_str == temp_state.state_str)
				{
					already_seen = true;
					break;
				}
			}
			//状態が探索済みリストにあるかどうかを確認します
			for (itr = closed.begin(); itr != closed.end(); itr++)
			{
				if (itr->state_str == temp_state.state_str)
				{
					already_seen = true;
					break;
				}
			}
			//重複していない場合、状態をオープンキューに追加します
			if (!already_seen)
			{
				report.node_count++;
				//オープンの背面に追加
				open.push_back(temp_state);
			}
			else
				report.rep_node_count++;
			valid_states.pop();
		}
	}
	report.fringe_node = (int)open.size();
	return report;
} //SearchStat bfs(State &initial_state)

/* クリア状態であるかどうかを確認するために使用される関数。
 *クリア状態とは,空のゴールがないとき、ゴールを埋めるプレーヤーが存在せず、
 *ゴール上にない荷物が存在しないとき
 *
 *前提条件：エージェントprogの現在の状態を表す状態オブジェクト
 *事後条件：クリアの状態が見つかった場合はtrueを返し、それ以外の場合はfalseを返します
 */
bool is_goal(State &cur_state)
{
	//std::cout << cur_state.state_str<<std::endl;
	bool goal = false;
	//空のゴールがない場合
	if ((cur_state.state_str.find_first_of('.')) == std::string::npos)
	{
		//空のゴールに立っているプレーヤーがいない場合、
		//その後、クリア状態になります
		if ((cur_state.state_str.find_first_of('+')) == std::string::npos)
		{
			//そして、ゴールにない箱はありません
			if ((cur_state.state_str.find_first_of('$')) == std::string::npos)
				goal = true;
		}
	}
	return goal;
} //bool is_goal(State &cur_state)

/*その状態が保存されている場合、レベル状態を出力するために使用される関数
* vector <char>のベクター。ベクターを反復処理し、印刷します
*各ベクトルの各文字を出力します。
*
*前提条件：vector <vector <char >>オブジェクト
* Postcoditions：ベクトルのベクトルの各要素を出力します。
*各ベクトルを改行で区切る
*/
void print_level(std::vector< std::vector<char> > &map)
{
	for (unsigned int i = 0; i < map.size(); i++)
	{
		std::vector<char> vec = map[i];
		std::vector<char>::iterator itr;
		for (itr = vec.begin(); itr != vec.end(); itr++)
			std::cout << *itr;
		std::cout << std::endl;
	}
} //void print_level(std::vector< std::vector<char> > &map)

/*この関数は、現在の状態からすべての有効な状態を生成するために使用されます
*有効なレベルが想定されます。プレイヤーエージェントが1人だけで、ゴールが1つだけという意味
 *各ボックスに対して、少なくとも1つの目標と1つのボックス。
 *検索アルゴリズム関数で呼び出されます。
 *
 * smodeの場合：
 * NONE-bfs / dfsでは移動の総コストは無視されます
 * UCS-移動の総コストが計算されます
 * GBFSH1-移動の総コストは無視され、ヒューリスティック関数1が使用されます
 * GBFSH2-移動の総コストは無視され、ヒューリスティック関数2が使用されます
 * ASH1-計算された移動の合計コスト、合計コストで使用されるヒューリスティック関数1
 * ASH2-計算された移動の合計コスト、合計コストで使用されるヒューリスティック関数2
 *
 *前提条件：エージェントの現在の状態を表す状態オブジェクト、
 *どの検索アルゴリズムが使用されたかを表すint
 *事後条件：可能なすべてを表す状態オブジェクトのキューを返します
 *現在の状態からの状態
 */
std::queue<State> gen_valid_states(const State &cur_state)
{
	std::queue<State> valid_moves;
	std::stringstream ss(cur_state.state_str);
	std::vector< std::vector<char> > new_stage;
	State new_state;
	std::string line;
	bool found = false;
	char player, box_move;
	int x, y, counter = 0, MOVE_COST = 1, PUSH_COST = 1;

	//検索アルゴリズムが統一コスト検索の場合、push_costを2に設定します
	if (smode == UCS)
		PUSH_COST = 2;

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
	if (!found)
	{
		std::cout << "No player found on level" << std::endl;
		return valid_moves;
	}

	//北タイルpos = x、y-1
	//北の動きを生成
	char north = stage[y - 1][x];
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

		//新しい状態を作成および更新します
		new_state = cur_state;
		new_state.state_str = "";
		// vector <vector <char >>を文字列に戻す
		for (unsigned int i = 0; i < new_stage.size(); i++)
		{
			std::vector<char> temp = new_stage[i];
			std::vector<char>::iterator itr;
			for (itr = temp.begin(); itr != temp.end(); itr++)
				new_state.state_str.push_back(*itr);
			new_state.state_str.append("\n");
		}

		//状態の統計を更新します
		new_state.move_list.append("u, ");
		new_state.depth++;
		new_state.moves++;
		if (smode == UCS || smode == ASH1 || smode == ASH2)
			new_state.total_cost += MOVE_COST;
		if (smode == GBFSH1 || smode == ASH1)
			new_state.hscore = h1(new_state);
		if (smode == GBFSH2 || smode == ASH2)
			new_state.hscore = h2(new_state);
		if (smode == ASH1 || smode == ASH2)
			new_state.hscore += new_state.total_cost;
		valid_moves.push(new_state);
		break;
		//空の目標に移動します
	case '.':
		new_stage = stage;
		//プレーヤーのタイルとプレーヤーの北のタイルの調整
		new_stage[y - 1][x] = '+';
		(player == '@') ? new_stage[y][x] = ' ' : new_stage[y][x] = '.';

		//新しい状態を作成および更新します
		new_state = cur_state;
		new_state.state_str = "";
		// vector <vector <char >>を文字列に戻す
		for (unsigned int i = 0; i < new_stage.size(); i++)
		{
			std::vector<char> temp = new_stage[i];
			std::vector<char>::iterator itr;
			for (itr = temp.begin(); itr != temp.end(); itr++)
				new_state.state_str.push_back(*itr);
			new_state.state_str.append("\n");
		}

		//状態の統計を更新します
		new_state.move_list.append("u, ");
		new_state.depth++;
		new_state.moves++;
		if (smode == UCS || smode == ASH1 || smode == ASH2)
			new_state.total_cost += MOVE_COST;
		if (smode == GBFSH1 || smode == ASH1)
			new_state.hscore = h1(new_state);
		if (smode == GBFSH2 || smode == ASH2)
			new_state.hscore = h2(new_state);
		if (smode == ASH1 || smode == ASH2)
			new_state.hscore += new_state.total_cost;
		valid_moves.push(new_state);
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

		//新しい状態を作成および更新します
		new_state = cur_state;
		new_state.state_str = "";
		// vector <vector <char >>を文字列に戻す
		for (unsigned int i = 0; i < new_stage.size(); i++)
		{
			std::vector<char> temp = new_stage[i];
			std::vector<char>::iterator itr;
			for (itr = temp.begin(); itr != temp.end(); itr++)
				new_state.state_str.push_back(*itr);
			new_state.state_str.append("\n");
		}

		//状態の統計を更新します
		new_state.move_list.append("u, ");
		new_state.depth++;
		new_state.pushes++;
		if (smode == UCS || smode == ASH1 || smode == ASH2)
			new_state.total_cost += PUSH_COST;
		if (smode == GBFSH1 || smode == ASH1)
			new_state.hscore = h1(new_state);
		if (smode == GBFSH2 || smode == ASH2)
			new_state.hscore = h2(new_state);
		if (smode == ASH1 || smode == ASH2)
			new_state.hscore += new_state.total_cost;
		valid_moves.push(new_state);
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

		//新しい状態を作成および更新します
		new_state = cur_state;
		new_state.state_str = "";
		// vector <vector <char >>を文字列に戻す
		for (unsigned int i = 0; i < new_stage.size(); i++)
		{
			std::vector<char> temp = new_stage[i];
			std::vector<char>::iterator itr;
			for (itr = temp.begin(); itr != temp.end(); itr++)
				new_state.state_str.push_back(*itr);
			new_state.state_str.append("\n");
		}

		//状態の統計を更新します
		new_state.move_list.append("u, ");
		new_state.depth++;
		new_state.pushes++;
		if (smode == UCS || smode == ASH1 || smode == ASH2)
			new_state.total_cost += PUSH_COST;
		if (smode == GBFSH1 || smode == ASH1)
			new_state.hscore = h1(new_state);
		if (smode == GBFSH2 || smode == ASH2)
			new_state.hscore = h2(new_state);
		if (smode == ASH1 || smode == ASH2)
			new_state.hscore += new_state.total_cost;
		valid_moves.push(new_state);
		break;
		//壁に移動します
	case '#':
		break;
	default:
		break;
	}

	//東タイルpos = x、y-1
	//東の動きを生成します
	char east = stage[y][x + 1];
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

		//新しい状態を作成および更新します
		new_state = cur_state;
		new_state.state_str = "";
		// vector <vector <char >>を文字列に戻す
		for (unsigned int i = 0; i < new_stage.size(); i++)
		{
			std::vector<char> temp = new_stage[i];
			std::vector<char>::iterator itr;
			for (itr = temp.begin(); itr != temp.end(); itr++)
				new_state.state_str.push_back(*itr);
			new_state.state_str.append("\n");
		}

		//状態の統計を更新します
		new_state.move_list.append("r, ");
		new_state.depth++;
		new_state.moves++;
		if (smode == UCS || smode == ASH1 || smode == ASH2)
			new_state.total_cost += MOVE_COST;
		if (smode == GBFSH1 || smode == ASH1)
			new_state.hscore = h1(new_state);
		if (smode == GBFSH2 || smode == ASH2)
			new_state.hscore = h2(new_state);
		if (smode == ASH1 || smode == ASH2)
			new_state.hscore += new_state.total_cost;
		//std::cout<<new_state.state_str<<std::endl;
		valid_moves.push(new_state);
		break;
		//空の目標に移動します
	case '.':
		new_stage = stage;
		//プレーヤーのタイルとプレーヤーの東のタイルを調整します
		new_stage[y][x + 1] = '+';
		(player == '@') ? new_stage[y][x] = ' ' : new_stage[y][x] = '.';

		//新しい状態を作成および更新します
		new_state = cur_state;
		new_state.state_str = "";
		// vector <vector <char >>を文字列に戻す
		for (unsigned int i = 0; i < new_stage.size(); i++)
		{
			std::vector<char> temp = new_stage[i];
			std::vector<char>::iterator itr;
			for (itr = temp.begin(); itr != temp.end(); itr++)
				new_state.state_str.push_back(*itr);
			new_state.state_str.append("\n");
		}

		//状態の統計を更新します
		new_state.move_list.append("r, ");
		new_state.depth++;
		new_state.moves++;
		if (smode == UCS || smode == ASH1 || smode == ASH2)
			new_state.total_cost += MOVE_COST;
		if (smode == GBFSH1 || smode == ASH1)
			new_state.hscore = h1(new_state);
		if (smode == GBFSH2 || smode == ASH2)
			new_state.hscore = h2(new_state);
		if (smode == ASH1 || smode == ASH2)
			new_state.hscore += new_state.total_cost;
		valid_moves.push(new_state);
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

		//新しい状態を作成および更新します
		new_state = cur_state;
		new_state.state_str = "";
		// vector <vector <char >>を文字列に戻す
		for (unsigned int i = 0; i < new_stage.size(); i++)
		{
			std::vector<char> temp = new_stage[i];
			std::vector<char>::iterator itr;
			for (itr = temp.begin(); itr != temp.end(); itr++)
				new_state.state_str.push_back(*itr);
			new_state.state_str.append("\n");
		}

		//状態の統計を更新します
		new_state.move_list.append("r, ");
		new_state.depth++;
		new_state.pushes++;
		if (smode == UCS || smode == ASH1 || smode == ASH2)
			new_state.total_cost += PUSH_COST;
		if (smode == GBFSH1 || smode == ASH1)
			new_state.hscore = h1(new_state);
		if (smode == GBFSH2 || smode == ASH2)
			new_state.hscore = h2(new_state);
		if (smode == ASH1 || smode == ASH2)
			new_state.hscore += new_state.total_cost;
		valid_moves.push(new_state);
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

		//新しい状態を作成および更新します
		new_state = cur_state;
		new_state.state_str = "";
		// vector <vector <char >>を文字列に戻す
		for (unsigned int i = 0; i < new_stage.size(); i++)
		{
			std::vector<char> temp = new_stage[i];
			std::vector<char>::iterator itr;
			for (itr = temp.begin(); itr != temp.end(); itr++)
				new_state.state_str.push_back(*itr);
			new_state.state_str.append("\n");
		}

		//状態の統計を更新します
		new_state.move_list.append("r, ");
		new_state.depth++;
		new_state.pushes++;
		if (smode == UCS || smode == ASH1 || smode == ASH2)
			new_state.total_cost += PUSH_COST;
		if (smode == GBFSH1 || smode == ASH1)
			new_state.hscore = h1(new_state);
		if (smode == GBFSH2 || smode == ASH2)
			new_state.hscore = h2(new_state);
		if (smode == ASH1 || smode == ASH2)
			new_state.hscore += new_state.total_cost;
		valid_moves.push(new_state);
		break;
		//壁に移動します
	case '#':
		break;
	default:
		break;
	}

	//南タイルpos = x、y-1
	//南に移動します
	char south = stage[y + 1][x];
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

		//新しい状態を作成および更新します
		new_state = cur_state;
		new_state.state_str = "";
		// vector <vector <char >>を文字列に戻す
		for (unsigned int i = 0; i < new_stage.size(); i++)
		{
			std::vector<char> temp = new_stage[i];
			std::vector<char>::iterator itr;
			for (itr = temp.begin(); itr != temp.end(); itr++)
				new_state.state_str.push_back(*itr);
			new_state.state_str.append("\n");
		}

		//状態の統計を更新します
		new_state.move_list.append("d, ");
		new_state.depth++;
		new_state.moves++;
		if (smode == UCS || smode == ASH1 || smode == ASH2)
			new_state.total_cost += MOVE_COST;
		if (smode == GBFSH1 || smode == ASH1)
			new_state.hscore = h1(new_state);
		if (smode == GBFSH2 || smode == ASH2)
			new_state.hscore = h2(new_state);
		if (smode == ASH1 || smode == ASH2)
			new_state.hscore += new_state.total_cost;
		valid_moves.push(new_state);
		break;
		//空の目標に移動します
	case '.':
		new_stage = stage;
		//プレーヤーのタイルとプレーヤーの南のタイルの調整
		new_stage[y + 1][x] = '+';
		(player == '@') ? new_stage[y][x] = ' ' : new_stage[y][x] = '.';

		//新しい状態を作成および更新します
		new_state = cur_state;
		new_state.state_str = "";
		// vector <vector <char >>を文字列に戻す
		for (unsigned int i = 0; i < new_stage.size(); i++)
		{
			std::vector<char> temp = new_stage[i];
			std::vector<char>::iterator itr;
			for (itr = temp.begin(); itr != temp.end(); itr++)
				new_state.state_str.push_back(*itr);
			new_state.state_str.append("\n");
		}

		//状態の統計を更新します
		new_state.move_list.append("d, ");
		new_state.depth++;
		new_state.moves++;
		if (smode == UCS || smode == ASH1 || smode == ASH2)
			new_state.total_cost += MOVE_COST;
		if (smode == GBFSH1 || smode == ASH1)
			new_state.hscore = h1(new_state);
		if (smode == GBFSH2 || smode == ASH2)
			new_state.hscore = h2(new_state);
		if (smode == ASH1 || smode == ASH2)
			new_state.hscore += new_state.total_cost;
		valid_moves.push(new_state);
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

		//新しい状態を作成および更新します
		new_state = cur_state;
		new_state.state_str = "";
		// vector <vector <char >>を文字列に戻す
		for (unsigned int i = 0; i < new_stage.size(); i++)
		{
			std::vector<char> temp = new_stage[i];
			std::vector<char>::iterator itr;
			for (itr = temp.begin(); itr != temp.end(); itr++)
				new_state.state_str.push_back(*itr);
			new_state.state_str.append("\n");
		}

		//状態の統計を更新します
		new_state.move_list.append("d, ");
		new_state.depth++;
		new_state.pushes++;
		if (smode == UCS || smode == ASH1 || smode == ASH2)
			new_state.total_cost += PUSH_COST;
		if (smode == GBFSH1 || smode == ASH1)
			new_state.hscore = h1(new_state);
		if (smode == GBFSH2 || smode == ASH2)
			new_state.hscore = h2(new_state);
		if (smode == ASH1 || smode == ASH2)
			new_state.hscore += new_state.total_cost;
		valid_moves.push(new_state);
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

		//新しい状態を作成および更新します
		new_state = cur_state;
		new_state.state_str = "";
		// vector <vector <char >>を文字列に戻す
		for (unsigned int i = 0; i < new_stage.size(); i++)
		{
			std::vector<char> temp = new_stage[i];
			std::vector<char>::iterator itr;
			for (itr = temp.begin(); itr != temp.end(); itr++)
				new_state.state_str.push_back(*itr);
			new_state.state_str.append("\n");
		}

		//状態の統計を更新します
		new_state.move_list.append("d, ");
		new_state.depth++;
		new_state.pushes++;
		if (smode == UCS || smode == ASH1 || smode == ASH2)
			new_state.total_cost += PUSH_COST;
		if (smode == GBFSH1 || smode == ASH1)
			new_state.hscore = h1(new_state);
		if (smode == GBFSH2 || smode == ASH2)
			new_state.hscore = h2(new_state);
		if (smode == ASH1 || smode == ASH2)
			new_state.hscore += new_state.total_cost;
		valid_moves.push(new_state);
		break;
		//壁に移動します
	case '#':
		break;
	default:
		break;
	}

	//西タイルpos = x、y-1
	//西の動きを生成します
	char west = stage[y][x - 1];
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

		//新しい状態を作成および更新します
		new_state = cur_state;
		new_state.state_str = "";
		// vector <vector <char >>を文字列に戻す
		for (unsigned int i = 0; i < new_stage.size(); i++)
		{
			std::vector<char> temp = new_stage[i];
			std::vector<char>::iterator itr;
			for (itr = temp.begin(); itr != temp.end(); itr++)
				new_state.state_str.push_back(*itr);
			new_state.state_str.append("\n");
		}

		//状態の統計を更新します
		new_state.move_list.append("l, ");
		new_state.depth++;
		new_state.moves++;
		if (smode == UCS || smode == ASH1 || smode == ASH2)
			new_state.total_cost += MOVE_COST;
		if (smode == GBFSH1 || smode == ASH1)
			new_state.hscore = h1(new_state);
		if (smode == GBFSH2 || smode == ASH2)
			new_state.hscore = h2(new_state);
		if (smode == ASH1 || smode == ASH2)
			new_state.hscore += new_state.total_cost;
		valid_moves.push(new_state);
		break;
		//空の目標に移動します
	case '.':
		new_stage = stage;
		//プレーヤーのタイルとプレーヤーの西のタイルの調整
		new_stage[y][x - 1] = '+';
		(player == '@') ? new_stage[y][x] = ' ' : new_stage[y][x] = '.';

		//新しい状態を作成および更新します
		new_state = cur_state;
		new_state.state_str = "";
		// vector <vector <char >>を文字列に戻す
		for (unsigned int i = 0; i < new_stage.size(); i++)
		{
			std::vector<char> temp = new_stage[i];
			std::vector<char>::iterator itr;
			for (itr = temp.begin(); itr != temp.end(); itr++)
				new_state.state_str.push_back(*itr);
			new_state.state_str.append("\n");
		}

		//状態の統計を更新します
		new_state.move_list.append("l, ");
		new_state.depth++;
		new_state.moves++;
		if (smode == UCS || smode == ASH1 || smode == ASH2)
			new_state.total_cost += MOVE_COST;
		if (smode == GBFSH1 || smode == ASH1)
			new_state.hscore = h1(new_state);
		if (smode == GBFSH2 || smode == ASH2)
			new_state.hscore = h2(new_state);
		if (smode == ASH1 || smode == ASH2)
			new_state.hscore += new_state.total_cost;
		valid_moves.push(new_state);
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

		//新しい状態を作成および更新します
		new_state = cur_state;
		new_state.state_str = "";
		// vector <vector <char >>を文字列に戻す
		for (unsigned int i = 0; i < new_stage.size(); i++)
		{
			std::vector<char> temp = new_stage[i];
			std::vector<char>::iterator itr;
			for (itr = temp.begin(); itr != temp.end(); itr++)
				new_state.state_str.push_back(*itr);
			new_state.state_str.append("\n");
		}

		//状態の統計を更新します
		new_state.move_list.append("l, ");
		new_state.depth++;
		new_state.pushes++;
		if (smode == UCS || smode == ASH1 || smode == ASH2)
			new_state.total_cost += PUSH_COST;
		if (smode == GBFSH1 || smode == ASH1)
			new_state.hscore = h1(new_state);
		if (smode == GBFSH2 || smode == ASH2)
			new_state.hscore = h2(new_state);
		if (smode == ASH1 || smode == ASH2)
			new_state.hscore += new_state.total_cost;
		valid_moves.push(new_state);
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

		//新しい状態を作成および更新します
		new_state = cur_state;
		new_state.state_str = "";
		// vector <vector <char >>を文字列に戻す
		for (unsigned int i = 0; i < new_stage.size(); i++)
		{
			std::vector<char> temp = new_stage[i];
			std::vector<char>::iterator itr;
			for (itr = temp.begin(); itr != temp.end(); itr++)
				new_state.state_str.push_back(*itr);
			new_state.state_str.append("\n");
		}

		//状態の統計を更新します
		new_state.move_list.append("l, ");
		new_state.depth++;
		new_state.pushes++;
		if (smode == UCS || smode == ASH1 || smode == ASH2)
			new_state.total_cost += PUSH_COST;
		if (smode == GBFSH1 || smode == ASH1)
			new_state.hscore = h1(new_state);
		if (smode == GBFSH2 || smode == ASH2)
			new_state.hscore = h2(new_state);
		if (smode == ASH1 || smode == ASH2)
			new_state.hscore += new_state.total_cost;
		valid_moves.push(new_state);
		break;
		//壁に移動します
	case '#':
		break;
	default:
		break;
	}

	return valid_moves;
} //std::queue<State> gen_valid_states (const State &cur_state, const int smode = NONE)

/* この関数は、初期状態で幅優先探索アルゴリズムを実行します。
 *無限の深さを持つ倉庫番パズルのため、探索されたリストは
 *無限ループを防ぐために使用されます。
 *
 *前提条件：レベルの初期状態のStateオブジェクトを取り込みます
 *事後条件：検索結果の統計情報のSearchStatオブジェクトを返します
 */
SearchStat bfs(State &initial_state)
{
	std::deque<State> open;
	std::vector<State> closed;
	SearchStat report;
	report.rep_node_count = 0;
	report.fringe_node = 0;
	report.explored_count = 1;//will be replaced, just to stop cout spam
	report.node_count = 1;
	report.node.state_str = "NULL";
	State current_state;

	//最初の状態をキューにプッシュします
	open.push_back(initial_state);
	//探索部分
	while (!open.empty())
	{
		// OPENからNを取得します
		current_state = open.front();
		open.pop_front();
		// NをCLOSEDにプッシュします
		closed.push_back(current_state);//探索済み

		//時間がかかった場合に出力し、フリーズしたかどうか確認する
		if ((closed.size() % 5000) == 0)
			std::cout << "...explored " << closed.size() << " nodes..." << std::endl;

		//探索が終了した場合、レポートノードを現在のノードに設定し、探索カウントをクローズドリストサイズに設定します
		if (is_goal(current_state))
		{
			report.node = current_state;
			report.explored_count = (int)closed.size();
			open.pop_front();
			break;
		}

		//有効な状態を生成します
		std::queue<State> valid_states = gen_valid_states(current_state);
		std::deque<State>::iterator it;
		std::vector<State>::iterator itr;

		//キューの状態が空でない間
		//移動可能な状態をキューへ
		while (!valid_states.empty())
		{
			bool already_seen = false;
			State temp_state = valid_states.front();
			//状態が既にオープンリストに表示されているかどうかを確認します
			for (it = open.begin(); it != open.end(); it++)
			{
				if (it->state_str == temp_state.state_str)
				{
					already_seen = true;
					break;
				}
			}
			//状態が探索済みリストにあるかどうかを確認します
			for (itr = closed.begin(); itr != closed.end(); itr++)
			{
				if (itr->state_str == temp_state.state_str)
				{
					already_seen = true;
					break;
				}
			}
			//重複していない場合、状態をオープンキューに追加します
			if (!already_seen)
			{
				report.node_count++;
				//オープンの背面に追加
				open.push_back(temp_state);
			}
			else
				report.rep_node_count++;
			valid_states.pop();
		}
	}
	report.fringe_node = (int)open.size();
	return report;
} //SearchStat bfs(State &initial_state)

/* 指定された初期状態で検索アルゴリズムを実行するために使用される関数。
 *バック検索結果を報告します。
 *
 *前提条件：状態オブジェクトと、検索アルゴリズムを表すintを取り込みます
 *事後条件：検索アルゴリズムを実行し、検索統計を出力します。
 */
SearchStat choose_search(State &init_state, int search_choice)
{
	timespec start, end;
	long long sec, nanosec;
	SearchStat final_stat;
	std::string user_choice;

	switch (search_choice)
	{
	case BFS://幅優先探索
		timespec_get(&start, TIME_UTC);
		final_stat = bfs(init_state);
		timespec_get(&end, TIME_UTC);
		break;

	case BFSR://逆幅優先探索
		timespec_get(&start, TIME_UTC);
		final_stat = bfs_reverse(init_state);
		timespec_get(&end, TIME_UTC);
		break;

	default:
		std::cout << "Unrecognized choice" << std::endl;
	}
	if (!final_stat.node.move_list.empty()) {
		//文字列の末尾の「,」を削除するために使用されるサブストリング
		std::cout << "  Solution: " << std::endl;
		std::cout << "    "
			<< final_stat.node.move_list.substr(0, (final_stat.node.move_list.size() - 2))
			<< std::endl;
		std::cout << "    # of nodes generated: ";
		std::cout << final_stat.node_count << std::endl;
		std::cout << "    # of duplicate states generated: ";
		std::cout << final_stat.rep_node_count << std::endl;
		std::cout << "    # of fringe nodes when termination occured: ";
		std::cout << final_stat.fringe_node << std::endl;
		std::cout << "    # of explored nodes: ";
		std::cout << final_stat.explored_count << std::endl;
		//検索アルゴリズムのランタイムを報告する
		std::cout << "  Actual run time: ";
		sec = end.tv_sec - start.tv_sec;
		nanosec = end.tv_nsec - start.tv_nsec;
		std::cout << (sec + (nanosec / 1000000000.0)) << " seconds" << std::endl;
	}

	return final_stat;
} //void choose_search(State &init_state, int search_choice)
