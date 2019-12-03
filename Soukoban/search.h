#pragma once
#include <vector>
#include <queue>
#include <string>

enum search_mode { NONE, UCS, GBFSH1, GBFSH2, ASH1, ASH2, BFS, DFS };

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

/* �N���A��Ԃł��邩�ǂ������m�F���邽�߂Ɏg�p�����֐��B
 *�N���A��ԂƂ�,��̃S�[�����Ȃ��Ƃ��A�S�[���𖄂߂�v���[���[�����݂����A
 *�S�[����ɂȂ��ו������݂��Ȃ��Ƃ�
 *
 *�O������F�G�[�W�F���gprog�̌��݂̏�Ԃ�\����ԃI�u�W�F�N�g
 *��������F�N���A�̏�Ԃ����������ꍇ��true��Ԃ��A����ȊO�̏ꍇ��false��Ԃ��܂�
 */
bool is_goal(State &); //bool is_goal(State &cur_state)

/*���̏�Ԃ��ۑ�����Ă���ꍇ�A���x����Ԃ��o�͂��邽�߂Ɏg�p�����֐�
* vector <char>�̃x�N�^�[�B�x�N�^�[�𔽕��������A������܂�
*�e�x�N�g���̊e�������o�͂��܂��B
*
*�O������Fvector <vector <char >>�I�u�W�F�N�g
* Postcoditions�F�x�N�g���̃x�N�g���̊e�v�f���o�͂��܂��B
*�e�x�N�g�������s�ŋ�؂�
*/
void print_level(std::vector< std::vector<char> > &); //void print_level(std::vector< std::vector<char> > &map)

/*�q���[���X�e�B�b�N�֐��́A�v���C���[��
 *�ł��߂��{�b�N�X�A����у{�b�N�X�ƖڕW�̊ԁB
 *���x����1�l�̃v���C���[�݂̂�z�肵�Ă��܂��B
 *
 *�O������F��ԃI�u�W�F�N�g
 *�㏈���F�q���[���X�e�B�b�N�X�R�A��\��int�߂�l
 */
int h1(const State &); //int h1(const State &cur_state)

/* �q���[���X�e�B�b�N�֐��́A�S�[���ɂȂ��{�b�N�X�������ꂩ�ɂ��邩�ǂ������m�F���܂�
*�u���S�łȂ��v�ʒu�B���S�łȂ��|�W�V�����́A�S�[���̂Ȃ��R�[�i�[�ł���A
 * 2�̊댯�ȃR�[�i�[�̐؂�ڂ̂Ȃ��ǂɉ����ă^�C������ׁA�S�[���Ȃ�
*�ǁB�q���[���X�e�B�b�N�֐�1�Ƃ͈قȂ�A���̊֐��͎����v�Z���܂�
 *�I�u�W�F�N�g�Ԃ̋����͂Ȃ��A���łɃS�[����̃{�b�N�X�ɗD�揇�ʂ�t���܂�
 *
 *�O������F��ԃI�u�W�F�N�g
 *�㏈���F�q���[���X�e�B�b�N�X�R�A��\��int�߂�l
 */
int h2(const State &); //int h2(const State &cur_state)


/*���̊֐��́A���݂̏�Ԃ��炷�ׂĂ̗L���ȏ�Ԃ𐶐����邽�߂Ɏg�p����܂�
*�L���ȃ��x�����z�肳��܂��B�v���C���[�G�[�W�F���g��1�l�����ŁA�S�[����1�����Ƃ����Ӗ�
 *�e�{�b�N�X�ɑ΂��āA���Ȃ��Ƃ�1�̖ڕW��1�̃{�b�N�X�B
 *�����A���S���Y���֐��ŌĂяo����܂��B
 *
 * smode�̏ꍇ�F
 * NONE-bfs / dfs�ł͈ړ��̑��R�X�g�͖�������܂�
 * UCS-�ړ��̑��R�X�g���v�Z����܂�
 * GBFSH1-�ړ��̑��R�X�g�͖�������A�q���[���X�e�B�b�N�֐�1���g�p����܂�
 * GBFSH2-�ړ��̑��R�X�g�͖�������A�q���[���X�e�B�b�N�֐�2���g�p����܂�
 * ASH1-�v�Z���ꂽ�ړ��̍��v�R�X�g�A���v�R�X�g�Ŏg�p�����q���[���X�e�B�b�N�֐�1
 * ASH2-�v�Z���ꂽ�ړ��̍��v�R�X�g�A���v�R�X�g�Ŏg�p�����q���[���X�e�B�b�N�֐�2
 *
 *�O������F�G�[�W�F���g�̌��݂̏�Ԃ�\����ԃI�u�W�F�N�g�A
 *�ǂ̌����A���S���Y�����g�p���ꂽ����\��int
 *��������F�\�Ȃ��ׂĂ�\����ԃI�u�W�F�N�g�̃L���[��Ԃ��܂�
 *���݂̏�Ԃ���̏��
 */
std::queue<State> gen_valid_states(const State &, const int); //std::queue<State> gen_valid_states (const State &cur_state, const int smode = NONE)

/* ���̊֐��́A������Ԃŕ��D��T���A���S���Y�������s���܂��B
 *�����̐[�������q�ɔԃp�Y���̂��߁A�T�����ꂽ���X�g��
 *�������[�v��h�����߂Ɏg�p����܂��B
 *
 *�O������F���x���̏�����Ԃ�State�I�u�W�F�N�g����荞�݂܂�
 *��������F�������ʂ̓��v����SearchStat�I�u�W�F�N�g��Ԃ��܂�
 */
SearchStat bfs(State &); //SearchStat bfs(State &initial_state)

/* ���̊֐��́A������ԂŐ[���D��T���A���S���Y�������s���܂��B
 *�����̐[�������q�ɔԃp�Y���̂��߁A�T�����ꂽ���X�g��
 *�������[�v��h�����߂Ɏg�p����܂��B
 *
 *�O������F���x���̏�����Ԃ�State�I�u�W�F�N�g����荞�݂܂�
 *��������F�������ʂ̓��v����SearchStat�I�u�W�F�N�g��Ԃ��܂�
 */
SearchStat dfs(State &); //SearchStat dfs(State &initial_state)

/*���̊֐��́A������Ԃœ���R�X�g�����A���S���Y�������s���܂��B
 *�����̐[�������q�ɔԃp�Y���̂��߁A�T�����ꂽ���X�g��
 *�������[�v��h�����߂Ɏg�p����܂��B
 *
 *�O������F���x���̏�����Ԃ�State�I�u�W�F�N�g����荞�݂܂�
 *��������F�������ʂ̓��v����SearchStat�I�u�W�F�N�g��Ԃ��܂�
 */
SearchStat ucs(State &); //SearchStat ucs(State &initial_state)

/* ���̊֐��́A�C�j�V�������×~�ȍŗǂ̍ŏ��̌����A���S���Y�������s���܂�
 *����̃q���[���X�e�B�b�N�֐����g�p������ԁB
 *�����̐[�������q�ɔԃp�Y���̂��߁A�T�����ꂽ���X�g��
 *�������[�v��h�����߂Ɏg�p����܂��B
 *
 *�O������F���x���̏�����Ԃ�State�I�u�W�F�N�g���擾���A
 *����юg�p����q���[���X�e�B�b�N�֐���\������
 *��������F�������ʂ̓��v����SearchStat�I�u�W�F�N�g��Ԃ��܂�
 */
SearchStat gbfs(State &, int); //SearchStat gbfs(State &initial_state, int hfchoice)

/*���̊֐��́A������Ԃ�A *���������A���S���Y�������s���܂��B
 *����̃q���[���X�e�B�b�N�֐��B
 *�����̐[�������q�ɔԃp�Y���̂��߁A�T�����ꂽ���X�g��
 *�������[�v��h�����߂Ɏg�p����܂��B
 *
 *�O������F���x���̏�����Ԃ�State�I�u�W�F�N�g���擾���A
 *����юg�p����q���[���X�e�B�b�N�֐���\������
 *��������F�������ʂ̓��v����SearchStat�I�u�W�F�N�g��Ԃ��܂�
 */
SearchStat as(State &, int); //SearchStat as(State &initial_state, int hfchoice)

/* �w�肳�ꂽ������ԂŌ����A���S���Y�������s���邽�߂Ɏg�p�����֐��B
 *�o�b�N�������ʂ�񍐂��܂��B
 *
 *�O������F��ԃI�u�W�F�N�g�ƁA�����A���S���Y����\��int����荞�݂܂�
 *��������F�����A���S���Y�������s���A�������v���o�͂��܂��B
 */
void choose_search(State &, int); //bool choose_search(State &init_state, int search_choice)
