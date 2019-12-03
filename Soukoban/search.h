#pragma once
#include <vector>
#include <queue>
#include <string>

enum search_mode { NONE, UCS, GBFSH1, GBFSH2, ASH1, ASH2, BFS, DFS };

/*Stateには、レベル、取得する移動 / プッシュおよび移動リストの数が含まれます
* その状態まで、およびその状態がノード単位でどの程度深いか
*/
struct State
{
	std::string state_str;
	std::string move_list;
	int depth;
	int moves;
	int pushes;
	int total_cost;
	int hscore;
}; //struct State

/* SearchStatは検索アルゴリズム情報をカウントするために使用される。
 *上から順に、ノード量、複製ノード、フリンジノード、探索ノード。
 */
struct SearchStat
{
	State node;
	int node_count;
	int rep_node_count;
	int fringe_node;
	int explored_count;
}; //struct SearchStat

/* クリア状態であるかどうかを確認するために使用される関数。
 *クリア状態とは,空のゴールがないとき、ゴールを埋めるプレーヤーが存在せず、
 *ゴール上にない荷物が存在しないとき
 *
 *前提条件：エージェントprogの現在の状態を表す状態オブジェクト
 *事後条件：クリアの状態が見つかった場合はtrueを返し、それ以外の場合はfalseを返します
 */
bool is_goal(State &); //bool is_goal(State &cur_state)

/*その状態が保存されている場合、レベル状態を出力するために使用される関数
* vector <char>のベクター。ベクターを反復処理し、印刷します
*各ベクトルの各文字を出力します。
*
*前提条件：vector <vector <char >>オブジェクト
* Postcoditions：ベクトルのベクトルの各要素を出力します。
*各ベクトルを改行で区切る
*/
void print_level(std::vector< std::vector<char> > &); //void print_level(std::vector< std::vector<char> > &map)

/*ヒューリスティック関数は、プレイヤーと
 *最も近いボックス、およびボックスと目標の間。
 *レベルで1人のプレイヤーのみを想定しています。
 *
 *前提条件：状態オブジェクト
 *後処理：ヒューリスティックスコアを表すint戻り値
 */
int h1(const State &); //int h1(const State &cur_state)

/* ヒューリスティック関数は、ゴールにないボックスがいずれかにあるかどうかを確認します
*「安全でない」位置。安全でないポジションは、ゴールのないコーナーであり、
 * 2つの危険なコーナーの切れ目のない壁に沿ってタイルを並べ、ゴールなし
*壁。ヒューリスティック関数1とは異なり、この関数は次を計算します
 *オブジェクト間の距離はなく、すでにゴール上のボックスに優先順位を付けます
 *
 *前提条件：状態オブジェクト
 *後処理：ヒューリスティックスコアを表すint戻り値
 */
int h2(const State &); //int h2(const State &cur_state)


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
std::queue<State> gen_valid_states(const State &, const int); //std::queue<State> gen_valid_states (const State &cur_state, const int smode = NONE)

/* この関数は、初期状態で幅優先探索アルゴリズムを実行します。
 *無限の深さを持つ倉庫番パズルのため、探索されたリストは
 *無限ループを防ぐために使用されます。
 *
 *前提条件：レベルの初期状態のStateオブジェクトを取り込みます
 *事後条件：検索結果の統計情報のSearchStatオブジェクトを返します
 */
SearchStat bfs(State &); //SearchStat bfs(State &initial_state)

/* この関数は、初期状態で深さ優先探索アルゴリズムを実行します。
 *無限の深さを持つ倉庫番パズルのため、探索されたリストは
 *無限ループを防ぐために使用されます。
 *
 *前提条件：レベルの初期状態のStateオブジェクトを取り込みます
 *事後条件：検索結果の統計情報のSearchStatオブジェクトを返します
 */
SearchStat dfs(State &); //SearchStat dfs(State &initial_state)

/*この関数は、初期状態で統一コスト検索アルゴリズムを実行します。
 *無限の深さを持つ倉庫番パズルのため、探索されたリストは
 *無限ループを防ぐために使用されます。
 *
 *前提条件：レベルの初期状態のStateオブジェクトを取り込みます
 *事後条件：検索結果の統計情報のSearchStatオブジェクトを返します
 */
SearchStat ucs(State &); //SearchStat ucs(State &initial_state)

/* この関数は、イニシャルで貪欲な最良の最初の検索アルゴリズムを実行します
 *特定のヒューリスティック関数を使用した状態。
 *無限の深さを持つ倉庫番パズルのため、探索されたリストは
 *無限ループを防ぐために使用されます。
 *
 *前提条件：レベルの初期状態のStateオブジェクトを取得し、
 *および使用するヒューリスティック関数を表す整数
 *事後条件：検索結果の統計情報のSearchStatオブジェクトを返します
 */
SearchStat gbfs(State &, int); //SearchStat gbfs(State &initial_state, int hfchoice)

/*この関数は、初期状態でA *検索検索アルゴリズムを実行します。
 *所定のヒューリスティック関数。
 *無限の深さを持つ倉庫番パズルのため、探索されたリストは
 *無限ループを防ぐために使用されます。
 *
 *前提条件：レベルの初期状態のStateオブジェクトを取得し、
 *および使用するヒューリスティック関数を表す整数
 *事後条件：検索結果の統計情報のSearchStatオブジェクトを返します
 */
SearchStat as(State &, int); //SearchStat as(State &initial_state, int hfchoice)

/* 指定された初期状態で検索アルゴリズムを実行するために使用される関数。
 *バック検索結果を報告します。
 *
 *前提条件：状態オブジェクトと、検索アルゴリズムを表すintを取り込みます
 *事後条件：検索アルゴリズムを実行し、検索統計を出力します。
 */
void choose_search(State &, int); //bool choose_search(State &init_state, int search_choice)
