#pragma once
#include <vector>
#include <queue>
#include <string>

enum search_mode { NONE, UCS, GBFSH1, GBFSH2, ASH1, ASH2, BFS, DFS,BFSR };

/*State�ɂ́A���x���A�擾����ړ� / �v�b�V������шړ����X�g�̐����܂܂�܂�
* ���̏�Ԃ܂ŁA����т��̏�Ԃ��m�[�h�P�ʂłǂ̒��x�[����
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

/* SearchStat�͌����A���S���Y�������J�E���g���邽�߂Ɏg�p�����B
 *�ォ�珇�ɁA�m�[�h�ʁA�����m�[�h�A�t�����W�m�[�h�A�T���m�[�h�B
 */
struct SearchStat
{
	State node;
	int node_count;
	int rep_node_count;
	int fringe_node;
	int explored_count;
}; //struct SearchStat

/*���̏�Ԃ��ۑ�����Ă���ꍇ�A���x����Ԃ��o�͂��邽�߂Ɏg�p�����֐�
* vector <char>�̃x�N�^�[�B�x�N�^�[�𔽕��������A������܂�
*�e�x�N�g���̊e�������o�͂��܂��B
*
*�O������Fvector <vector <char >>�I�u�W�F�N�g
* Postcoditions�F�x�N�g���̃x�N�g���̊e�v�f���o�͂��܂��B
*�e�x�N�g�������s�ŋ�؂�
*/
void print_level(std::vector< std::vector<char> > &); //void print_level(std::vector< std::vector<char> > &map)

/* �w�肳�ꂽ������ԂŌ����A���S���Y�������s���邽�߂Ɏg�p�����֐��B
 *�o�b�N�������ʂ�񍐂��܂��B
 *
 *�O������F��ԃI�u�W�F�N�g�ƁA�����A���S���Y����\��int����荞�݂܂�
 *��������F�����A���S���Y�������s���A�������v���o�͂��܂��B
 */
SearchStat choose_search(State &, int); //bool choose_search(State &init_state, int search_choice)
