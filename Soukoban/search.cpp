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

/*ヒューリスティック関数は、プレイヤーと
 *最も近いボックス、およびボックスと目標の間。
 *レベルで1人のプレイヤーのみを想定しています。
 *
 *前提条件：状態オブジェクト
 *後処理：ヒューリスティックスコアを表すint戻り値
 */
int h1(const State &cur_state)
{
	std::stringstream ss(cur_state.state_str);
	std::string line;
	int x, y, counter = 0;
	std::vector< std::vector<int> > box_list;
	std::vector< std::vector<int> > goal_list;
	int score = 0;
	int playerdist = 1000;

	while (getline(ss, line, '\n'))
	{
		for (unsigned int i = 0; i < line.length(); i++)
		{
			//任意の状態でプレイヤーが1人だけ存在すると仮定します
			//ユーザーが見つかった場合、x、yの位置を設定します
			if (line[i] == '@')
			{
				x = i;
				y = counter;
			}
			//プレーヤーが空のゴールにいる場合、プレーヤーの位置
			//空の目標と同じです
			else if (line[i] == '+')
			{
				x = i;
				y = counter;
				std::vector<int> goal_loc;
				goal_loc.push_back(i);
				goal_loc.push_back(counter);
				goal_list.push_back(goal_loc);
			}
			//ボックスが床にある場合、ボックスリストに追加
			else if (line[i] == '$')
			{
				std::vector<int> box_loc;
				box_loc.push_back(i);
				box_loc.push_back(counter);
				box_list.push_back(box_loc);
			}
			//空の目標の場合、目標リストに追加します
			else if (line[i] == '.')
			{
				std::vector<int> goal_loc;
				goal_loc.push_back(i);
				goal_loc.push_back(counter);
				goal_list.push_back(goal_loc);
			}
			//ゴールにボックスがある場合、それは取る必要があります
			//複数の目標がある場合に、フィルされない目標よりも優先順位
			//ボックスと目標。プログラムにボックスを残してほしい
			//ゴールに。このため、-1000がスコアに追加されます
			else if (line[i] == '*')
			{
				score -= 1000;
			}
		}
		counter++;
	}

	//床の各ボックスについて、各空のゴールまでの距離を計算します
	for (unsigned int i = 0; i < box_list.size(); i++)
	{
		for (unsigned int j = 0; j < goal_list.size(); j++)
		{
			// xおよびyコードの距離を計算します
			int xdist = box_list[i][0] - goal_list[j][0];
			int ydist = box_list[i][1] - goal_list[j][1];
			//距離の絶対値を取得します
			if (xdist < 0)
				xdist *= -1;
			if (ydist < 0)
				ydist *= -1;
			//スコアに距離を追加します。スコアが低いほど良いです
			score += xdist;
			score += ydist;
		}
		//プレイヤーからボックスまでの距離を計算します
		int p_to_box_x = box_list[i][0] - x;
		int p_to_box_y = box_list[i][1] - y;
		//距離の絶対値を取得します
		if (p_to_box_x < 0)
			p_to_box_x *= -1;
		if (p_to_box_y < 0)
			p_to_box_y *= -1;
		//任意のボックスまでの最短距離を保存します
				//注：これは、最短距離が1000タイル以内の場合にのみ機能します
		if (playerdist > (p_to_box_y + p_to_box_x))
			playerdist = (p_to_box_y + p_to_box_x);
	}
	score += playerdist;
	return score;
} //int h1(const State &cur_state)

/* ヒューリスティック関数は、ゴールにないボックスがいずれかにあるかどうかを確認します
*「安全でない」位置。安全でないポジションは、ゴールのないコーナーであり、
 * 2つの危険なコーナーの切れ目のない壁に沿ってタイルを並べ、ゴールなし
*壁。ヒューリスティック関数1とは異なり、この関数は次を計算します
 *オブジェクト間の距離はなく、すでにゴール上のボックスに優先順位を付けます
 *
 *前提条件：状態オブジェクト
 *後処理：ヒューリスティックスコアを表すint戻り値
 */
int h2(const State &cur_state)
{
	std::stringstream ss(cur_state.state_str);
	std::vector< std::vector<int> > box_list;
	std::vector< std::vector<char> > stage;
	std::string line;
	int score = 0, counter = 0;

	//レベルマップを生成し、ボックスがレベル​​マップとして保存されている場所をマークします
	//（y、x）座標
	//次にcharごとに（x平面）
	while (getline(ss, line, '\n'))
	{
		std::vector<char> temp;
		stage.push_back(temp);
		for (unsigned int i = 0; i < line.length(); i++)
		{
			//ボックスが床にある場合、ボックスリストに追加
			//ゴール上のボックスは無視できないため、無視します
			//安全でない位置になります
			if (line[i] == '$')
			{
				std::vector<int> box_loc;
				box_loc.push_back(i);
				box_loc.push_back(counter);
				box_list.push_back(box_loc);
			}
			stage[counter].push_back(line[i]);
		}
		counter++;
	}

	//床の各ボックスについて、壁に隣接しているかどうかを確認します
	//隣接する2つの壁に隣接している場合、
	//それはコーナーです
	for (unsigned int i = 0; i < box_list.size(); i++)
	{
		bool N_wall = false;
		bool E_wall = false;
		bool S_wall = false;
		bool W_wall = false;
		bool in_corner = false;

		int cur_box_x = box_list[i][0];
		int cur_box_y = box_list[i][1];

		//ボックスの北に壁があるかどうかを確認します
		if (stage[cur_box_y - 1][cur_box_x] == '#')
			N_wall = true;
		//ボックスの東側に壁があるかどうかを確認します
		if (stage[cur_box_y][cur_box_x + 1] == '#')
			E_wall = true;
		//ボックスの南に壁があるかどうかを確認します
		if (stage[cur_box_y + 1][cur_box_x] == '#')
			S_wall = true;
		//ボックスの西側に壁があるかどうかを確認します
		if (stage[cur_box_y][cur_box_x - 1] == '#')
			W_wall = true;

		//最初にボックスが角にあるかどうかを確認します
		// 北東コーナーのボックスを確認します
		if (N_wall && E_wall)
		{
			in_corner = true;
		}
		// 北西コーナーのボックスを確認します
		else if (N_wall && W_wall)
		{
			in_corner = true;
		}
		// 南東コーナーのボックスを確認します
		else if (S_wall && E_wall)
		{
			in_corner = true;
		}
		// 南西コーナーのボックスを確認します
		else if (S_wall && W_wall)
		{
			in_corner = true;
		}

		//ボックスが角にある場合、ボックスはデッドロック位置にあります
		if (in_corner)
		{
			score += 1000;
		}
		//ボックスが壁の隣にある場合、壁が壊れていないかどうかを確認します
		//安全でないコーナーが2つあり、壁に沿ってゴールがない
		else
		{
			//壁がボックスの北の場合、東端および西端の壁を検索します
			if (N_wall)
			{
				bool safe = false;
				bool corner_E = false;
				bool corner_W = false;

				//切れ目なくアクセス可能なタイルがあるかどうかを東に検索
				//角が見つかるまで北の壁。ボックスとプレイヤーは無視されます
				//移動できるため、アクセス可能なタイルと見なされます
				for (unsigned int i = cur_box_x + 1; i < stage[cur_box_y].size(); i++)
				{
					//途中で目標が見つかった場合、安全でない位置にはなりません
					if ((stage[cur_box_y][i] == '.') ||
						(stage[cur_box_y][i] == '*') || (stage[cur_box_y][i] == '+'))
					{
						safe = true;
						break;
					}

					//北のタイルが壁でない場合、安全です
					if (stage[cur_box_y - 1][i] != '#')
					{
						safe = true;
						break;
					}

					// 北東コーナーの場合
					if ((stage[cur_box_y][i] == '#') && (stage[cur_box_y - 1][i] == '#'))
					{
						corner_E = true;
						break;
					}
				}

				//切れ目なくアクセス可能なタイルがあるかどうかを西に検索します
				//角が見つかるまで北の壁。ボックスとプレイヤーは無視されます
				//移動できるため、アクセス可能なタイルと見なされます
				for (int i = cur_box_x - 1; i >= 0; i--)
				{
					//途中で目標が見つかった場合、安全でない位置にはなりません
					if ((stage[cur_box_y][i] == '.') ||
						(stage[cur_box_y][i] == '*') || (stage[cur_box_y][i] == '+'))
					{
						safe = true;
						break;
					}

					//北のタイルが壁でない場合、安全です
					if (stage[cur_box_y - 1][i] != '#')
					{
						safe = true;
						break;
					}

					// 北西コーナーの場合
					if ((stage[cur_box_y][i] == '#') && (stage[cur_box_y - 1][i] == '#'))
					{
						corner_W = true;
						break;
					}
				}

				//ゴールのあるボックスの東と西のパスに沿って壁が壊れていない場合
				//スコアをインクリメントして安全でない位置を示します
				if (!safe)
				{
					if (corner_E && corner_W)
						score += 1000;
				}
			}

			//壁がボックスの東である場合、北および南の壁を検索します
			if (E_wall)
			{
				bool safe = false;
				bool corner_N = false;
				bool corner_S = false;

				//北を検索して、切れ目のない場所にアクセス可能なタイルがあるかどうかを確認します
				//角が見つかるまで東の壁。ボックスとプレイヤーは無視されます
				//移動できるため、アクセス可能なタイルと見なされます
				for (int i = cur_box_y - 1; i >= 0; i--)
				{
					//途中で目標が見つかった場合、安全でない位置にはなりません
					if ((stage[i][cur_box_x] == '.') ||
						(stage[i][cur_box_x] == '*') || (stage[i][cur_box_x] == '+'))
					{
						safe = true;
						break;
					}

					//東のタイルが壁でない場合、安全です
					if (stage[i][cur_box_x + 1] != '#')
					{
						safe = true;
						break;
					}

					// NEコーナーの場合
					if ((stage[i][cur_box_x] == '#') && (stage[i][cur_box_x + 1] == '#'))
					{
						corner_N = true;
						break;
					}
				}

				//南を検索して、壊れていないタイルに沿ってアクセス可能なタイルがあるかどうかを確認します
				//角が見つかるまで東の壁。ボックスとプレイヤーは無視されます
				//移動できるため、アクセス可能なタイルと見なされます
				for (unsigned int i = cur_box_y + 1; i < stage.size(); i++)
				{
					//途中で目標が見つかった場合、安全でない位置にはなりません
					if ((stage[i][cur_box_x] == '.') ||
						(stage[i][cur_box_x] == '*') || (stage[i][cur_box_x] == '+'))
					{
						safe = true;
						break;
					}

					//東のタイルが壁でない場合、安全です
					if (stage[i][cur_box_x + 1] != '#')
					{
						safe = true;
						break;
					}

					// 南東コーナーの場合
					if ((stage[i][cur_box_x] == '#') && (stage[i][cur_box_x + 1] == '#'))
					{
						corner_S = true;
						break;
					}
				}

				//ゴールのあるボックスの東と西のパスに沿って壁が壊れていない場合
				//スコアをインクリメントして安全でない位置を示します				if (!safe)
				{
					if (corner_N && corner_S)
						score += 1000;
				}
			}

			//壁がボックスの南側の場合、東端および西端の壁を検索します
			if (S_wall)
			{
				bool safe = false;
				bool corner_E = false;
				bool corner_W = false;

				//切れ目なくアクセス可能なタイルがあるかどうかを東に検索
				//角が見つかるまで南壁。ボックスとプレイヤーは無視されます
				//移動できるため、アクセス可能なタイルと見なされます				for (int i = cur_box_x + 1; i < stage[cur_box_y].size(); i++)
				{
					//途中で目標が見つかった場合、安全でない位置にはなりません
					if ((stage[cur_box_y][i] == '.') ||
						(stage[cur_box_y][i] == '*') || (stage[cur_box_y][i] == '+'))
					{
						safe = true;
						break;
					}

					//南のタイルが壁でない場合、安全です
					if (stage[cur_box_y + 1][i] != '#')
					{
						safe = true;
						break;
					}

					// 南東コーナーの場合
					if ((stage[cur_box_y][i] == '#') && (stage[cur_box_y + 1][i] == '#'))
					{
						corner_E = true;
						break;
					}
				}

				//切れ目なくアクセス可能なタイルがあるかどうかを西に検索します
				//角が見つかるまで南壁。ボックスとプレイヤーは無視されます
				//移動できるため、アクセス可能なタイルと見なされます
				for (int i = cur_box_x - 1; i >= 0; i--)
				{
					//途中で目標が見つかった場合、安全でない位置にはなりません
					if ((stage[cur_box_y][i] == '.') ||
						(stage[cur_box_y][i] == '*') || (stage[cur_box_y][i] == '+'))
					{
						safe = true;
						break;
					}

					//南のタイルが壁でない場合、安全です
					if (stage[cur_box_y + 1][i] != '#')
					{
						safe = true;
						break;
					}

					// 南西コーナーの場合
					if ((stage[cur_box_y][i] == '#') && (stage[cur_box_y + 1][i] == '#'))
					{
						corner_W = true;
						break;
					}
				}

				//ゴールのあるボックスの東と西のパスに沿って壁が壊れていない場合
				//スコアをインクリメントして安全でない位置を示します
				if (!safe)
				{
					if (corner_E && corner_W)
						score += 1000;
				}
			}

			//壁がボックスの西である場合、北および南の壁を検索します
			if (W_wall)
			{
				bool safe = false;
				bool corner_N = false;
				bool corner_S = false;

				//北を検索して、切れ目のない場所にアクセス可能なタイルがあるかどうかを確認します
				//角が見つかるまで東の壁。ボックスとプレイヤーは無視されます
				//移動できるため、アクセス可能なタイルと見なされます
				for (int i = cur_box_y - 1; i >= 0; i--)
				{
					//途中で目標が見つかった場合、安全でない位置にはなりません
					if ((stage[i][cur_box_x] == '.') ||
						(stage[i][cur_box_x] == '*') || (stage[i][cur_box_x] == '+'))
					{
						safe = true;
						break;
					}

					//東のタイルが壁でない場合、安全です
					if (stage[i][cur_box_x - 1] != '#')
					{
						safe = true;
						break;
					}

					// 北西コーナーの場合
					if ((stage[i][cur_box_x] == '#') && (stage[i][cur_box_x - 1] == '#'))
					{
						corner_N = true;
						break;
					}
				}

				//南を検索して、壊れていないタイルに沿ってアクセス可能なタイルがあるかどうかを確認します
				//角が見つかるまで東の壁。ボックスとプレイヤーは無視されます
				//移動できるため、アクセス可能なタイルと見なされます
				for (unsigned int i = cur_box_y + 1; i < stage.size(); i++)
				{
					//途中で目標が見つかった場合、安全でない位置にはなりません
					if ((stage[i][cur_box_x] == '.') ||
						(stage[i][cur_box_x] == '*') || (stage[i][cur_box_x] == '+'))
					{
						safe = true;
						break;
					}

					//東のタイルが壁でない場合、安全です
					if (stage[i][cur_box_x - 1] != '#')
					{
						safe = true;
						break;
					}

					// 南西コーナーの場合
					if ((stage[i][cur_box_x] == '#') && (stage[i][cur_box_x - 1] == '#'))
					{
						corner_S = true;
						break;
					}
				}

				//ゴールのあるボックスの東と西のパスに沿って壁が壊れていない場合
				//スコアをインクリメントして安全でない位置を示します
				if (!safe)
				{
					if (corner_N && corner_S)
						score += 1000;
				}
			}
		}
	}
	return score;
} //int h2(const State &cur_state)


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
std::queue<State> gen_valid_states(const State &cur_state, const int smode = NONE)
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

/* この関数は、初期状態で深さ優先探索アルゴリズムを実行します。
 *無限の深さを持つ倉庫番パズルのため、探索されたリストは
 *無限ループを防ぐために使用されます。
 *
 *前提条件：レベルの初期状態のStateオブジェクトを取り込みます
 *事後条件：検索結果の統計情報のSearchStatオブジェクトを返します
 */
SearchStat dfs(State &initial_state)
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
	while (!open.empty())
	{
		// OPENからNを取得します
		current_state = open.front();
		open.pop_front();
		// NをCLOSEDにプッシュします
		closed.push_back(current_state);

		//時間がかかった場合に印刷し、フリーズしたかどうか疑問に思う
		if ((closed.size() % 5000) == 0)
			std::cout << "...explored " << closed.size() << " nodes..." << std::endl;

		//見つかった場合、レポートノードを現在のノードに設定し、探索カウントをクローズドリストサイズに設定します
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
		std::deque<State> temp_open;

		//キューの状態が空でない間
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
			//状態が既に閉じたリストにあるかどうかを確認します
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
				//一時的なキューを使用してノードの順序を逆にします
				temp_open.push_front(temp_state);
			}
			else
				report.rep_node_count++;
			valid_states.pop();
		}
		// dfsで左から右への検索を維持するために、逆キューを逆にします
		while (!temp_open.empty())
		{
			State temp_state = temp_open.front();
			open.push_front(temp_state);
			temp_open.pop_front();
		}
	}
	report.fringe_node = (int)open.size();
	return report;
} //SearchStat dfs(State &initial_state)

/*この関数は、初期状態で統一コスト検索アルゴリズムを実行します。
 *無限の深さを持つ倉庫番パズルのため、探索されたリストは
 *無限ループを防ぐために使用されます。
 *
 *前提条件：レベルの初期状態のStateオブジェクトを取り込みます
 *事後条件：検索結果の統計情報のSearchStatオブジェクトを返します
 */
SearchStat ucs(State &initial_state)
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
	while (!open.empty())
	{
		// OPENからNを取得します
		current_state = open.front();
		open.pop_front();
		// NをCLOSEDにプッシュします
		closed.push_back(current_state);

		//時間がかかった場合に印刷し、フリーズしたかどうか疑問に思う
		if ((closed.size() % 5000) == 0)
			std::cout << "...explored " << closed.size() << " nodes..." << std::endl;

		//見つかった場合、レポートノードを現在のノードに設定し、探索カウントをクローズドリストサイズに設定します
		if (is_goal(current_state))
		{
			report.node = current_state;
			report.explored_count = (int)closed.size();
			open.pop_front();
			break;
		}

		//有効な状態を生成します
		std::queue<State> valid_states = gen_valid_states(current_state, UCS);
		std::deque<State>::iterator it;
		std::vector<State>::iterator itr;

		//キューの状態が空でない間
		while (!valid_states.empty())
		{
			bool already_seen = false;
			bool inserted = false;
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
			//状態が既に閉じたリストにあるかどうかを確認します
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
				//より高い総コストでノードの先頭に追加します
				//生成されたノードの順序を維持するために使用されます
				for (it = open.begin(); it != open.end(); it++)
				{
					//最も低い合計コストの優先度が最初
					if (it->total_cost > temp_state.total_cost)
					{
						open.insert(it, temp_state);
						inserted = true;
						break;
					}
				}
				//優先度の高いノードがない場合は、プッシュバックします
				if (!inserted)
					open.push_back(temp_state);
			}
			else
				report.rep_node_count++;
			valid_states.pop();
		}
	}
	report.fringe_node = (int)open.size();
	return report;
} //SearchStat ucs(State &initial_state)

/* この関数は、イニシャルで貪欲な最良の最初の検索アルゴリズムを実行します
 *特定のヒューリスティック関数を使用した状態。
 *無限の深さを持つ倉庫番パズルのため、探索されたリストは
 *無限ループを防ぐために使用されます。
 *
 *前提条件：レベルの初期状態のStateオブジェクトを取得し、
 *および使用するヒューリスティック関数を表す整数
 *事後条件：検索結果の統計情報のSearchStatオブジェクトを返します
 */
SearchStat gbfs(State &initial_state, int hfchoice)
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
	while (!open.empty())
	{
		// OPENからNを取得します
		current_state = open.front();
		open.pop_front();
		// NをCLOSEDにプッシュします
		closed.push_back(current_state);

		//時間がかかった場合に印刷し、フリーズしたかどうか疑問に思う
		if ((closed.size() % 5000) == 0)
			std::cout << "...explored " << closed.size() << " nodes..." << std::endl;

		//見つかった場合、レポートノードを現在のノードに設定し、探索カウントをクローズドリストサイズに設定します		if (is_goal(current_state))
		{
			report.node = current_state;
			report.explored_count = (int)closed.size();
			open.pop_front();
			break;
		}

		//有効な状態を生成します
		std::queue<State> valid_states;
		valid_states = gen_valid_states(current_state, hfchoice);
		std::deque<State>::iterator it;
		std::vector<State>::iterator itr;

		//キューの状態が空でない間
		while (!valid_states.empty())
		{
			bool already_seen = false;
			bool inserted = false;
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
			//状態が既に閉じたリストにあるかどうかを確認します
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
				//より高い総コストでノードの先頭に追加します
				//生成されたノードの順序を維持するために使用されます
				for (it = open.begin(); it != open.end(); it++)
				{
					//priority of lowest hscore first
					if (it->hscore > temp_state.hscore)
					{
						open.insert(it, temp_state);
						inserted = true;
						break;
					}
				}
				//優先度の高いノードがない場合は、プッシュバックします
				if (!inserted)
					open.push_back(temp_state);
			}
			else
				report.rep_node_count++;
			valid_states.pop();
		}
	}
	report.fringe_node = (int)open.size();
	return report;
} //SearchStat gbfs(State &initial_state, int hfchoice)

/*この関数は、初期状態でA *検索検索アルゴリズムを実行します。
 *所定のヒューリスティック関数。
 *無限の深さを持つ倉庫番パズルのため、探索されたリストは
 *無限ループを防ぐために使用されます。
 *
 *前提条件：レベルの初期状態のStateオブジェクトを取得し、
 *および使用するヒューリスティック関数を表す整数
 *事後条件：検索結果の統計情報のSearchStatオブジェクトを返します
 */
SearchStat as(State &initial_state, int hfchoice)
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
	while (!open.empty())
	{
		// OPENからNを取得します
		current_state = open.front();
		open.pop_front();
		// NをCLOSEDにプッシュします
		closed.push_back(current_state);

		//時間がかかった場合に印刷し、フリーズしたかどうか疑問に思う
		if ((closed.size() % 5000) == 0)
			std::cout << "...explored " << closed.size() << " nodes..." << std::endl;

		//見つかった場合、レポートノードを現在のノードに設定し、探索カウントをクローズドリストサイズに設定します
		if (is_goal(current_state))
		{
			report.node = current_state;
			report.explored_count = (int)closed.size();
			open.pop_front();
			break;
		}

		//有効な状態を生成します
		std::queue<State> valid_states;
		valid_states = gen_valid_states(current_state, hfchoice);
		std::deque<State>::iterator it;
		std::vector<State>::iterator itr;

		//キューの状態が空でない間
		while (!valid_states.empty())
		{
			bool already_seen = false;
			bool inserted = false;
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
			//状態が既に閉じたリストにあるかどうかを確認します
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
				//より高い総コストでノードの先頭に追加します
				//生成されたノードの順序を維持するために使用されます
				for (it = open.begin(); it != open.end(); it++)
				{
					//最も低いhscoreの優先度が最初
					if (it->hscore > temp_state.hscore)
					{
						open.insert(it, temp_state);
						inserted = true;
						break;
					}
				}
				//優先度の高いノードがない場合は、プッシュバックします
				if (!inserted)
					open.push_back(temp_state);
			}
			else
				report.rep_node_count++;
			valid_states.pop();
		}
	}
	report.fringe_node = (int)open.size();
	return report;
} //SearchStat as(State &initial_state, int hfchoice)

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
		std::cout << "BREADTH FIRST SEARCH:" << std::endl;
		timespec_get(&start, TIME_UTC);
		final_stat = bfs(init_state);
		timespec_get(&end, TIME_UTC);
		break;

	case DFS://深さ優先探索
		std::cout << "DEPTH FIRST SEARCH:" << std::endl;
		timespec_get(&start, TIME_UTC);
		final_stat = dfs(init_state);
		timespec_get(&end, TIME_UTC);
		break;

	case UCS://均一コスト探索
		std::cout << "UNIFORM COST SEARCH:" << std::endl;
		timespec_get(&start, TIME_UTC);
		final_stat = ucs(init_state);
		timespec_get(&end, TIME_UTC);
		break;

	case GBFSH1://最良優先探索１
		std::cout << "GREEDY BEST FIRST SEARCH, HEURISTICS FUNCTION 1:" << std::endl;
		timespec_get(&start, TIME_UTC);
		final_stat = gbfs(init_state, GBFSH1);
		timespec_get(&end, TIME_UTC);
		break;

	case GBFSH2://最良優先探索２
		std::cout << "GREEDY BEST FIRST SEARCH, HEURISTICS FUNCTION 2:" << std::endl;
		timespec_get(&start, TIME_UTC);
		final_stat = gbfs(init_state, GBFSH2);
		timespec_get(&end, TIME_UTC);
		break;

	case ASH1://A*探索１
		std::cout << "A* SEARCH, HEURISTICS FUNCTION 1:" << std::endl;
		timespec_get(&start, TIME_UTC);
		final_stat = as(init_state, ASH1);
		timespec_get(&end, TIME_UTC);
		break;

	case ASH2://A*探索２
		std::cout << "A* SEARCH, HEURISTICS FUNCTION 2:" << std::endl;
		timespec_get(&start, TIME_UTC);
		final_stat = as(init_state, ASH2);
		timespec_get(&end, TIME_UTC);
		break;

	default:
		std::cout << "Unrecognized choice" << std::endl;
	}

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

	return final_stat;
} //void choose_search(State &init_state, int search_choice)
