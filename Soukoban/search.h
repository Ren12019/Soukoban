#pragma once
#include <vector>
#include <queue>
#include <string>

enum SearchMode { NONE, BFS, BFSR };

/*Stateには、レベル、取得する移動 / プッシュおよび移動リストの数が含まれます
* その状態まで、およびその状態がノード単位でどの程度深いか
*/
struct State
{
	std::string state_str;//ステージの状態
	std::string move_list;//解答経路
	int depth;//現在の状態までの深さ
	int moves;//アバターの移動回数
	int pushes;//荷物を押した回数
	int push_lines;//荷物移動のライン数
	int push_direction;//最後に荷物を押した方向
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

/*その状態が保存されている場合、レベル状態を出力するために使用される関数
* vector <char>のベクター。ベクターを反復処理し、印刷します
*各ベクトルの各文字を出力します。
*
*前提条件：vector <vector <char >>オブジェクト
* Postcoditions：ベクトルのベクトルの各要素を出力します。
*各ベクトルを改行で区切る
*/
void print_level(std::vector< std::vector<char> > &); //void print_level(std::vector< std::vector<char> > &map)

/* 指定された初期状態で検索アルゴリズムを実行するために使用される関数。
 *バック検索結果を報告します。
 *
 *前提条件：状態オブジェクトと、検索アルゴリズムを表すintを取り込みます
 *事後条件：検索アルゴリズムを実行し、検索統計を出力します。
 */
SearchStat choose_search(State &, int); //bool choose_search(State &init_state, int search_choice)
