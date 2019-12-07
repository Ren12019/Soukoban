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

enum direction { NORTH = 0, EAST, SOUTH, WEST };

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
		std::cout << "荷物が存在しません。" << std::endl;
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
	std::queue<State> list;//比較用キュー

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
		/*テスト*/list.push(current_state);

		//時間がかかった場合に出力し、フリーズしたかどうか確認する
		if ((closed.size() % 5000) == 0)
			std::cout << "...explored " << closed.size() << " nodes..." << std::endl;

		//有効な状態を生成します
		std::queue<State> valid_states = gen_valid_states_reverse(current_state);

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
	//最良を収納
	int best = 0;
	while (!list.empty()) {
		if (best < list.front().moves) {
			best = list.front().moves;
			report.node = list.front();
		}
		list.pop();
	}
	report.explored_count = (int)closed.size();
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
		print_level(stage);
		return valid_moves;
	}

	//全方位の移動可能個所をキューへ
	//上、右、下、左
	int dx[4] = { 0, 1, 0, -1 };
	int dy[4] = { -1, 0, 1, 0 };
	for (unsigned int i = 0; i < 4; i++)
	{
		//移動先
		int next_x = x + dx[i];
		int next_y = y + dy[i];
		//荷物の移動先
		int next_to_next_x = next_x + dx[i];
		int next_to_next_y = next_y + dy[i];

		//範囲確認
		if (next_x < 0 || next_x > WIDTH - 1 || next_y < 0 || next_y > HEIGHT - 1)
			continue;

		char direction = stage[next_y][next_x];
		switch (direction)
		{
		//空のマスに移動します
		case ' ':
			new_stage = stage;
			//プレーヤーのタイルとプレーヤーの移動先タイルの調整
			new_stage[next_y][next_x] = '@';
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
			break;
		//空のゴールに移動します
		case '.':
			new_stage = stage;
			//プレーヤーのタイルとプレーヤーの移動先タイルの調整
			new_stage[next_y][next_x] = '+';
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
			break;
		//フロアーのボックスに移動します
		case '$':
			//範囲確認
			if (next_to_next_x < 0 || next_to_next_x > WIDTH - 1 || next_to_next_y < 0 || next_to_next_y > HEIGHT - 1)
				break;
			new_stage = stage;
			//プレーヤーのタイルとプレーヤーの移動先タイルの調整
			new_stage[next_y][next_x] = '@';
			(player == '@') ? new_stage[y][x] = ' ' : new_stage[y][x] = '.';

			//荷物のマスと荷物の移動先のタイルを調整します
			box_move = new_stage[next_to_next_y][next_to_next_x];
			//荷物の移動先が壁または別のボックスの場合
			if (box_move == '#')
				break;
			else if (box_move == '$')
				break;
			else if (box_move == '*')
				break;
			//荷物の移動先が空の床である場合
			else if (box_move == ' ')
			{
				new_stage[next_to_next_y][next_to_next_x] = '$';
			}
			//荷物の移動先が空のゴールの場合
			else if (box_move == '.')
			{
				new_stage[next_to_next_y][next_to_next_x] = '*';
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
			new_state.pushes++;
			valid_moves.push(new_state);
			break;
		//ゴールでボックスに移動
		case '*':
			//範囲確認
			if (next_to_next_x < 0 || next_to_next_x > WIDTH - 1 || next_to_next_y < 0 || next_to_next_y > HEIGHT - 1)
				break;
			new_stage = stage;
			//プレーヤーのタイルとプレーヤーの移動先タイルの調整
			new_stage[next_y][next_x] = '+';
			(player == '@') ? new_stage[y][x] = ' ' : new_stage[y][x] = '.';

			//ボックスタイルとボックスの北のタイルを調整します
			box_move = new_stage[next_to_next_y][next_to_next_x];
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
				new_stage[next_to_next_y][next_to_next_x] = '$';
			}
			//ボックスの北が空のゴールの場合
			else if (box_move == '.')
			{
				new_stage[next_to_next_y][next_to_next_x] = '*';
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
			new_state.pushes++;
			valid_moves.push(new_state);
			break;
		//壁に移動します
		case '#':
			break;
		default:
			break;
		}

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
		closed.push_back(current_state);//探索済み

		//時間がかかった場合に出力し、フリーズしたかどうか確認する
		if ((closed.size() % 5000) == 0)
			std::cout << "...explored " << closed.size() << " nodes..." << std::endl;

		//探索が終了した場合、レポートノードを現在のノードに設定し、探索カウントをクローズドリストサイズに設定します
		if (is_goal(current_state))
		{
			report.node = current_state;
			report.explored_count = (int)closed.size();
			//open.pop_front();
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
	if (!final_stat.node.move_list.empty() && search_choice == BFS) {
		//文字列の末尾の「,」を削除するために使用されるサブストリング
		std::cout << "  Solution: " << std::endl;
		std::cout << "    "
			<< final_stat.node.move_list.substr(0, (final_stat.node.move_list.size() - 2))
			<< std::endl;
		std::cout << "    # 生成した有効ノード数: ";
		std::cout << final_stat.node_count << std::endl;
		std::cout << "    # 生成した総ノード数: ";
		std::cout << final_stat.rep_node_count << std::endl;
		std::cout << "    # 解答発見時に未探索の有効ノード数: ";
		std::cout << final_stat.fringe_node << std::endl;
		std::cout << "    # 探索したノード数: ";
		std::cout << final_stat.explored_count << std::endl;
		//検索アルゴリズムのランタイムを報告する
		std::cout << "  解答時間: ";
		sec = end.tv_sec - start.tv_sec;
		nanosec = end.tv_nsec - start.tv_nsec;
		std::cout << (sec + (nanosec / 1000000000.0)) << " seconds" << std::endl;
	}
	return final_stat;
} //void choose_search(State &init_state, int search_choice)
