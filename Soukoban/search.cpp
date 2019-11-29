#include "search.h"
#include <iostream>
#include <vector>
#include <queue>
#include <iomanip>
#include <string> 
#include "define.h"
#include "count.h"
#include "print.h"
#include "check.h"
#include "stage.h"

/* 4 �����ւ̗אڒ��_�ւ̈ړ���\���x�N�g�� */
const int dx[4] = { 1, 0, -1, 0 };//�E�A���A���A��
const int dy[4] = { 0, 1, 0, -1 };//�E�A���A���A��

SQUARE searchBox(int stage[][WIDTH]) {
	SQUARE point_box;
	point_box.x = 0;
	point_box.y = 0;
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (stage[y][x] == BOX) {
				point_box.x = x;
				point_box.y = y;
				return point_box;
			}
		}
	}

	return point_box;
}

SQUARE searchGoal(int stage[][WIDTH]) {
	SQUARE point_goal;
	point_goal.x = 0;
	point_goal.y = 0;
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (stage[y][x] == GOAL) {
				point_goal.x = x;
				point_goal.y = y;
				return point_goal;
			}
		}
	}

	return point_goal;
}

int searchBreadthFirst(int stage[][WIDTH]) {
	int search_stage[HEIGHT][WIDTH] = {};//�T���p�̃X�e�[�W	
	SQUARE start_pos;//�ו��̏����ʒu
	SQUARE goal_pos;//�ו��̏����ʒu
	SQUARE movable_pos;//���[�p
	SQUARE current_pos;//�ו��̌��ݍ��W
	std::queue<SQUARE> search;//�ړ��\�ł��薢���B�̍��W��ۑ��i�T���p�L���[�j
	std::vector<std::vector<int>> dist(HEIGHT, std::vector<int>(WIDTH,-1));//�ʉ߂������W�̃X�^�[�g����̋���
	// �T�����Ɋe�}�X�͂ǂ̃}�X���痈���̂���\���z�� (�ŒZ�o�H����m�邾���Ȃ�A����͕s�v)
	std::vector<std::vector<int> > prev_x(HEIGHT, std::vector<int>(WIDTH, -1));
	std::vector<std::vector<int> > prev_y(HEIGHT, std::vector<int>(WIDTH, -1));
	                    /*(�c�̃T�C�Y,(���̃T�C�Y,����������l))*/

	//
	/*�����ݒ�*/
	//
	/*�T���p�X�e�[�W�̏�����*/
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			search_stage[y][x] = stage[y][x];
		}
	}
	/*�����ʒu���Z�b�g*/
	start_pos = searchBox(stage);
	goal_pos = searchGoal(stage);
	/*�X�^�[�g�̋������u0�v�ɐݒ�*/
	dist[start_pos.y][start_pos.x] = 0;
	/*�X�^�[�g���v�b�V��*/
	search.push(start_pos);
	current_pos = search.front();


	//
	/*�T��*/
	//
	while (!search.empty())
	{
		/*�ړ��i�Ֆʂ̕ύX�j*/
		search_stage[current_pos.y][current_pos.x] = PATH;
		if (search_stage[search.front().y][search.front().x] == PATH) {
			search_stage[search.front().y][search.front().x] = BOX;
		}
		else if (search_stage[search.front().y][search.front().x] == GOAL) {
			search_stage[search.front().y][search.front().x] = BOX_ON_GOAL;
		}
		current_pos = search.front();
		search.pop();

		/*���͂̈ړ��\�ȍ��W���L���[�Ɏ��[*/
		for (int direction = 0; direction < 4; direction++) {
			int next_x = current_pos.x + dx[direction];
			int next_y = current_pos.y + dy[direction];
			// ��O���m
			if (next_x < 0 || next_x >= WIDTH || next_y < 0 || next_y >= HEIGHT) continue; 
			//�ו����ړ��\��
			if (search_stage[next_y][next_x] != WALL && search_stage[next_y][next_x] != BOX && search_stage[next_y][next_x] != MAN) {
				movable_pos.x = next_x;
				movable_pos.y = next_y;
				//�ʉߍς݂��m�F
				if (dist[movable_pos.y][movable_pos.x] == -1) {
					search.push(movable_pos);
					dist[movable_pos.y][movable_pos.x] = dist[current_pos.y][current_pos.x] + 1;
					prev_x[movable_pos.y][movable_pos.x] = current_pos.x;  // �ǂ̒��_�����񂪓`�d���ė������A�c�����̍��W������
					prev_y[movable_pos.y][movable_pos.x] = current_pos.y;  // �ǂ̒��_�����񂪓`�d���ė������A�������̍��W������
				}
			}
		}

		/*�T���̏I���m�F*/
		if (checkClear(search_stage)) {
			std::cout << "Clear" << std::endl;
			break;
		}
		if (search.empty()) {
			std::cout << "Not found route" << std::endl;
			return 0;
		}
	}
	
	//
	/* ���ʏo�� */
	//
	/* �e�}�X�ւ̃X�^�[�g����̍ŒZ���������Ă݂� */
	for (int y = 0; y < HEIGHT; ++y) {
		for (int x = 0; x < WIDTH; ++x) {
			if (stage[y][x] == WALL) std::cout << "###";
			else std::cout << std::setw(3) << dist[y][x];
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

	/* �S�[���Ɏ���܂ł̍ŒZ�o�H�𕜌����Ă݂� */
	int x = goal_pos.x, y = goal_pos.y;
	while (x != -1 && y != -1) {
		search_stage[y][x] = CHECK; // �ʉ߂������Ƃ�����

		// �O�̒��_�֍s��
		int px = prev_x[y][x];
		int py = prev_y[y][x];
		x = px, y = py;
	}

	search_stage[start_pos.y][start_pos.x] = BOX;
	search_stage[goal_pos.y][goal_pos.x] = GOAL;
	printStage(search_stage);

	return 1;
}

std::vector<SQUARE> searchBoxTwo(int stage[][WIDTH]) {
	std::vector<SQUARE>box_pos(NUMBER_OF_BOX);

	for (int y = 0,cnt_box=0; y < HEIGHT && cnt_box < NUMBER_OF_BOX; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (stage[y][x] == BOX || stage[y][x] == BOX_ON_GOAL) {
				box_pos[cnt_box].x = x;
				box_pos[cnt_box].y = y;
				cnt_box++;
			}
		}
	}

	return box_pos;
}

std::vector<SQUARE> searchGoalTwo(int stage[][WIDTH]) {
	std::vector<SQUARE>goal_pos(NUMBER_OF_BOX);

	for (int y = 0, cnt_goal = 0; y < HEIGHT && cnt_goal < NUMBER_OF_BOX; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (stage[y][x] == GOAL || stage[y][x] == BOX_ON_GOAL) {
				goal_pos[cnt_goal].x = x;
				goal_pos[cnt_goal].y = y;
				cnt_goal++;
			}
		}
	}

	return goal_pos;
}

int searchBreadthFirstTwo(int stage[][WIDTH]) {
	using namespace std;
	int search_stage[HEIGHT][WIDTH] = {};//�T���p�̃X�e�[�W	
	vector<SQUARE> start_pos(NUMBER_OF_BOX);//�ו��̏����ʒu
	vector<SQUARE> goal_pos(NUMBER_OF_BOX);//�ו��̏����ʒu
	vector<SQUARE> movable_pos(NUMBER_OF_BOX);//���[�p
	vector<SQUARE> current_pos(NUMBER_OF_BOX);//�ו��̌��ݍ��W
	queue<vector<SQUARE>> search_que;//�ړ��\�ł��薢���B�̍��W��ۑ��i�T���p�L���[�j
	//vector<vector<vector<int>>> dist(NUMBER_OF_BOX, vector< vector<int>>(HEIGHT, vector<int>(WIDTH, -1)));//�ʉ߂������W�̃X�^�[�g����̋���
	// �T�����Ɋe�}�X�͂ǂ̃}�X���痈���̂���\���z�� (�ŒZ�o�H����m�邾���Ȃ�A����͕s�v)
	//vector<vector<vector<int>>> prev_x(NUMBER_OF_BOX, vector< vector<int>>(HEIGHT, vector<int>(WIDTH, -1)));
	//vector<vector<vector<int>>> prev_y(NUMBER_OF_BOX, vector< vector<int>>(HEIGHT, vector<int>(WIDTH, -1)));
	                                                                  /*(�c�̃T�C�Y,(���̃T�C�Y,����������l))*/

//
	/*�����ݒ�*/
//
	/*�T���p�X�e�[�W�̏�����*/
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			search_stage[y][x] = stage[y][x];
		}
	}
	/*�����ʒu���Z�b�g*/
	start_pos = searchBoxTwo(stage);
	goal_pos = searchGoalTwo(stage);

	///*�X�^�[�g�̋������u0�v�ɐݒ�*/
	//for (int cnt_box = 0; cnt_box < NUMBER_OF_BOX; cnt_box++) {
	//	dist[cnt_box][start_pos[cnt_box].y][start_pos[cnt_box].x] = 0;
	//}
	/*�X�^�[�g���v�b�V��*/
	search_que.push(start_pos);
	current_pos = search_que.front();

	//
	/*�T��*/
	//
	while (!search_que.empty())
	{
		/*�ړ��i�Ֆʂ̕ύX�j*/
		for (int cnt_box = 0; cnt_box < NUMBER_OF_BOX; cnt_box++) {
			//���ݒn���ו�
			if (search_stage[current_pos[cnt_box].y][current_pos[cnt_box].x] == BOX) {
				if (search_stage[search_que.front()[cnt_box].y][search_que.front()[cnt_box].x] == PATH) {
					//�ړ��悪��
					search_stage[search_que.front()[cnt_box].y][search_que.front()[cnt_box].x] = BOX;
					//�ړ��O�ɋ����ꏊ���󂫂�
					search_stage[current_pos[cnt_box].y][current_pos[cnt_box].x] = PATH;
					addStageList(search_stage);
				}
				else if (search_stage[search_que.front()[cnt_box].y][search_que.front()[cnt_box].x] == GOAL) {
					//�ړ��悪�S�[��
					search_stage[search_que.front()[cnt_box].y][search_que.front()[cnt_box].x] = BOX_ON_GOAL;
					//�ړ��O�ɋ����ꏊ���󂫂�
					search_stage[current_pos[cnt_box].y][current_pos[cnt_box].x] = PATH;
					addStageList(search_stage);
				}
				else if (search_stage[search_que.front()[cnt_box].y][search_que.front()[cnt_box].x] == BOX) {
					continue;
				}
				else {
					cout << "error" << endl;
					cout << search_stage[search_que.front()[cnt_box].y][search_que.front()[cnt_box].x] << "," << search_que.front()[cnt_box].x << "," << search_que.front()[cnt_box].y << endl;
				}
			}
			else if (search_stage[current_pos[cnt_box].y][current_pos[cnt_box].x] == BOX_ON_GOAL) {
				if (search_stage[search_que.front()[cnt_box].y][search_que.front()[cnt_box].x] == PATH) {
					//�ړ��悪��
					search_stage[search_que.front()[cnt_box].y][search_que.front()[cnt_box].x] = BOX;
					//�ړ��O�ɋ����ꏊ���S�[����
					search_stage[current_pos[cnt_box].y][current_pos[cnt_box].x] = GOAL;
					addStageList(search_stage);
				}
				else if (search_stage[search_que.front()[cnt_box].y][search_que.front()[cnt_box].x] == GOAL) {
					//�ړ��悪�S�[��
					search_stage[search_que.front()[cnt_box].y][search_que.front()[cnt_box].x] = BOX_ON_GOAL;
					//�ړ��O�ɋ����ꏊ���S�[����
					search_stage[current_pos[cnt_box].y][current_pos[cnt_box].x] = GOAL;
					addStageList(search_stage);
				}
				else if (search_stage[search_que.front()[cnt_box].y][search_que.front()[cnt_box].x] == BOX) {
					continue;
				}
				else {
					cout << "error" << endl;
					cout << search_stage[search_que.front()[cnt_box].y][search_que.front()[cnt_box].x] << "," << search_que.front()[cnt_box].x << "," << search_que.front()[cnt_box].y << endl;
				}
			}
		}
		current_pos = search_que.front();
		search_que.pop();

		/*���͂̈ړ��\�ȍ��W���L���[�Ɏ��[*/
		for (int cnt_box = 0; cnt_box < NUMBER_OF_BOX; cnt_box++) {
			for (int direction = 0; direction < 4; direction++) {
				int next_x = current_pos[cnt_box].x + dx[direction];
				int next_y = current_pos[cnt_box].y + dy[direction];
				// ��O���m
				if (next_x < 0 || next_x >= WIDTH || next_y < 0 || next_y >= HEIGHT) continue;
				//�ו����ړ��\��
				if (search_stage[next_y][next_x] == PATH || search_stage[next_y][next_x] == GOAL) {
					movable_pos[cnt_box].x = next_x;
					movable_pos[cnt_box].y = next_y;
					//�����ɂȂ�
					if (!checkStageList(search_stage,current_pos,movable_pos)) {//�_�����ۂ��H
						search_que.push(movable_pos);
						//dist[cnt_box][movable_pos[cnt_box].y][movable_pos[cnt_box].x] = dist[cnt_box][current_pos[cnt_box].y][current_pos[cnt_box].x] + 1;
						//prev_x[cnt_box][movable_pos[cnt_box].y][movable_pos[cnt_box].x] = current_pos[cnt_box].x;  // �ǂ̒��_�����񂪓`�d���ė������A�c�����̍��W������
						//prev_y[cnt_box][movable_pos[cnt_box].y][movable_pos[cnt_box].x] = current_pos[cnt_box].y;  // �ǂ̒��_�����񂪓`�d���ė������A�������̍��W������
					}
				}
			}
		}

		/*�T���̏I���m�F*/
		if (checkClear(search_stage)) {
			cout << "Clear" << endl;
			break;
		}
		if (search_que.empty()) {
			cout << "Not found route" << endl;
			//return 0;
		}
	}
	printStage(search_stage);
	//
	/* ���ʏo�� */
	//
	/* �e�}�X�ւ̃X�^�[�g����̍ŒZ���������Ă݂� */
	/*for (int cnt_box = 0; cnt_box < NUMBER_OF_BOX; cnt_box++) {
		cout << cnt_box << endl;
		for (int y = 0; y < HEIGHT; ++y) {
			for (int x = 0; x < WIDTH; ++x) {
				if (stage[y][x] == WALL) cout << "###";
				else cout << setw(3) << dist[cnt_box][y][x];
			}
			cout << endl;
		}
		cout << endl;
	}*/

	/* �S�[���Ɏ���܂ł̍ŒZ�o�H�𕜌����Ă݂� */
	//for (int cnt_box = 0; cnt_box < NUMBER_OF_BOX; cnt_box++) {
	//	int x = goal_pos[cnt_box].x, y = goal_pos[cnt_box].y;
	//	while (x != -1 && y != -1) {
	//		search_stage[y][x] = CHECK; // �ʉ߂������Ƃ�����

	//		// �O�̒��_�֍s��
	//		int px = prev_x[cnt_box][y][x];
	//		int py = prev_y[cnt_box][y][x];
	//		x = px, y = py;
	//	}
	//}

	//for (int cnt_box = 0; cnt_box < NUMBER_OF_BOX; cnt_box++) {
	//	search_stage[start_pos[cnt_box].y][start_pos[cnt_box].x] = BOX;
	//	search_stage[goal_pos[cnt_box].y][goal_pos[cnt_box].x] = GOAL;
	//}
	//printStage(search_stage);

	return 1;
}