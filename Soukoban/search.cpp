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
			if (stage[y][x] == BOX) {
				point_goal.x = x;
				point_goal.y = y;
				return point_goal;
			}
		}
	}

	return point_goal;
}

//EDGE searchAround(int stage[][WIDTH]) {
//	EDGE a;
//
//	return ;
//}

//int searchBreadthFirstSample(int stage[][WIDTH]) {
//	////////////////////////////////////////
///* ���͎󂯎�� */
//////////////////////////////////////////
//
///* �c�Ɖ��̒��� */
//	int height, width;
//	cout << "���ƍ�������͂��Ă�������" << endl;
//	cin >> height >> width;
//
//	/* �Ֆ� */
//	vector<string> field(height);
//	cout << "�Ֆʂ���s�Â��͂��Ă�������" << endl;
//	for (int h = 0; h < height; ++h) {
//		cout << h << endl;
//		cin >> field[h];
//	}
//
//	/* �X�^�[�g�n�_�ƃS�[���n�_ */
//	int sx, sy, gx, gy;
//	for (int h = 0; h < height; ++h) {
//		for (int w = 0; w < width; ++w) {
//			if (field[h][w] == 'S') {
//				sx = h;
//				sy = w;
//			}
//			if (field[h][w] == 'G') {
//				gx = h;
//				gy = w;
//			}
//		}
//	}
//
//
//	////////////////////////////////////////
//	/* ���D��T���̏����ݒ� */
//	////////////////////////////////////////
//
//	// �e�Z���̍ŒZ���� (�K��Ă��Ȃ��Ƃ���� -1 �Ƃ��Ă���)
//	vector<vector<int> > dist(height, vector<int>(width, -1));
//	dist[sx][sy] = 0; // �X�^�[�g�� 0 ��
//
//	// �u��x�������_�v�̂����u�܂��K��Ă��Ȃ����_�v��\���L���[
//	queue<pair<int, int> > que;
//	que.push(make_pair(sx, sy)); // �X�^�[�g�� push
//
//
//	////////////////////////////////////////
//	/* ���D��T�������{ */
//	////////////////////////////////////////    
//
//	/* �L���[����ɂȂ�܂� */
//	while (!que.empty()) {
//		pair<int, int> current_pos = que.front(); // �L���[�̐擪������ (C++ �ł͂�������Ă� pop ���Ȃ�)
//		int x = current_pos.first;
//		int y = current_pos.second;
//		que.pop(); // �L���[���� pop ��Y�ꂸ��
//
//		// �אڒ��_��T��
//		for (int direction = 0; direction < 4; ++direction) {
//			int next_x = x + dx[direction];
//			int next_y = y + dy[direction];
//			if (next_x < 0 || next_x >= height || next_y < 0 || next_y >= width) continue; // ��O�A�E�g�Ȃ�_��
//			if (field[next_x][next_y] == '#') continue; // �ǂ̓_��
//
//			// �܂����Ă��Ȃ����_�Ȃ� push
//			if (dist[next_x][next_y] == -1) {
//				que.push(make_pair(next_x, next_y));
//				dist[next_x][next_y] = dist[x][y] + 1;
//			}
//		}
//	}
//
//
//	////////////////////////////////////////
//	/* ���ʏo�� */
//	////////////////////////////////////////    
//
//	/* �e�}�X�ւ̃X�^�[�g����̍ŒZ���������Ă݂� */
//	for (int h = 0; h < height; ++h) {
//		for (int w = 0; w < width; ++w) {
//			if (field[h][w] != '.' && field[h][w] != 'G') cout << std::setw(3) << field[h][w];
//			else cout << std::setw(3) << dist[h][w];
//		}
//		cout << endl;
//	}
//	cout << endl;
//
//	//////////////////////////////////////////
//	///* ���� */
//	//////////////////////////////////////////
//	//SQUARE position_box;
//	//SQUARE position_goal;
//	////int N = countSpace(stage);//���_�̐�
//	////int M = countConnectionAll(stage);//�ӂ̐�
//	////TargetStage Target(N);
//
//	// /* �Ֆ� */
//	//vector<vector<string>> field(WIDTH);
//	///*for (int y = 0; y < HEIGHT; y++) {
//	//	for (int x = 0; x < WIDTH; x++) {
//	//		if(stage[y][x]==WALL)
//	//			field[x][y] = '#';
//	//		else if (stage[y][x] == MAN)
//	//			field[x][y] = '@';
//	//		else if (stage[y][x] == MAN_ON_GOAL)
//	//			field[x][y] = '+';
//	//		else if (stage[y][x] == BOX)
//	//			field[x][y] = '$';
//	//		else if (stage[y][x] == BOX_ON_GOAL)
//	//			field[x][y] = '*';
//	//		else if (stage[y][x] == GOAL)
//	//			field[x][y] = '.';
//	//		else if (stage[y][x] == PATH)
//	//			field[x][y] = ' ';
//	//		else if (stage[y][x] == CHECK)
//	//			field[x][y] = 'x';
//	//	}
//	//}*/
//	//for (int h = 0; h < HEIGHT; ++h) cin >> field[h];
//
//
//	///* �X�^�[�g�n�_�ƃS�[���n�_ */
//	//position_box = searchBox(stage);
//	//position_goal = searchGoal(stage);
//	//int sx, sy, gx, gy;
//	//sx = position_box.y;
//	//sy = position_box.x;
//	//gx = position_goal.y;
//	//gy = position_goal.x;
//	//////////////////////////////////////////
//	///* ���D��T���̏����ݒ� */
//	//////////////////////////////////////////
//	//// �e�Z���̍ŒZ���� (�K��Ă��Ȃ��Ƃ���� -1 �Ƃ��Ă���)
//	//vector<vector<int> > dist(HEIGHT, vector<int>(WIDTH, -1));
//	//dist[sx][sy] = 0; // �X�^�[�g�� 0 ��
//
//	//// �T�����Ɋe�}�X�͂ǂ̃}�X���痈���̂���\���z�� (�ŒZ�o�H����m�邾���Ȃ�A����͕s�v)
//	//vector<vector<int> > prev_x(WIDTH, vector<int>(HEIGHT, -1));
//	//vector<vector<int> > prev_y(WIDTH, vector<int>(HEIGHT, -1));
//
//	//// �u��x�������_�v�̂����u�܂��K��Ă��Ȃ����_�v��\���L���[
//	//queue<pair<int, int> > que;
//	//que.push(make_pair(sx, sy)); // �X�^�[�g�� push
//
//	//////////////////////////////////////////
//	///* ���D��T�������{ */
//	//////////////////////////////////////////    
//
//	// /* �L���[����ɂȂ�܂� */
//	//while (!que.empty()) {
//	//	pair<int, int> current_pos = que.front(); // �L���[�̐擪������ (C++ �ł͂�������Ă� pop ���Ȃ�)
//	//	int x = current_pos.first;
//	//	int y = current_pos.second;
//	//	que.pop(); // �L���[���� pop ��Y�ꂸ��
//
//	//	// �אڒ��_��T��
//	//	for (int direction = 0; direction < 4; ++direction) {
//	//		int next_x = x + dx[direction];
//	//		int next_y = y + dy[direction];
//	//		if (next_x < 0 || next_x >= HEIGHT || next_y < 0 || next_y >= WIDTH) continue; // ��O�A�E�g�Ȃ�_��
//	//		if (field[next_x][next_y] == "#") continue; // �ǂ̓_��
//
//	//		// �܂����Ă��Ȃ����_�Ȃ� push
//	//		if (dist[next_x][next_y] == -1) {
//	//			que.push(make_pair(next_x, next_y));
//	//			dist[next_x][next_y] = dist[x][y] + 1; // (next_x, next_y) �̋������X�V
//	//			prev_x[next_x][next_y] = x;  // �ǂ̒��_�����񂪓`�d���ė������A�c�����̍��W������
//	//			prev_y[next_x][next_y] = y;  // �ǂ̒��_�����񂪓`�d���ė������A�������̍��W������
//	//		}
//	//	}
//	//}
//
//
//	//////////////////////////////////////////
// //  /* ���ʏo�� */
// //  ////////////////////////////////////////    
//
// //  /* �e�}�X�ւ̃X�^�[�g����̍ŒZ���������Ă݂� */
//	//for (int h = 0; h < HEIGHT; ++h) {
//	//	for (int w = 0; w < WIDTH; ++w) {
//	//		if (field[h][w] != '.') cout << std::setw(3) << field[h][w];
//	//		else cout << std::setw(3) << dist[h][w];
//	//	}
//	//	cout << endl;
//	//}
//	//cout << endl;
//
//	///* �S�[���Ɏ���܂ł̍ŒZ�o�H�𕜌����Ă݂� */
//	//int x = gx, y = gy;
//	//while (x != -1 && y != -1) {
//	//	field[x][y] = 'o'; // �ʉ߂������Ƃ�����
//
//	//	// �O�̒��_�֍s��
//	//	int px = prev_x[x][y];
//	//	int py = prev_y[x][y];
//	//	x = px, y = py;
//	//}
//	//for (int h = 0; h < HEIGHT; ++h) {
//	//	for (int w = 0; w < WIDTH; ++w) {
//	//		cout << std::setw(3) << field[h][w];
//	//	}
//	//	cout << endl;
//	//}
//
//	return 0;
//}

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
			printStage(search_stage);
			break;
		}
		if (search.empty()) {
			std::cout << "Not found route" << std::endl;
			printStage(search_stage);
			break;
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
	std::cout << x << "," << y << std::endl;
	while (x != -1 && y != -1) {
		std::cout << "1:" << x << "," << y << std::endl;

		search_stage[y][x] = CHECK; // �ʉ߂������Ƃ�����

		// �O�̒��_�֍s��
		int px = prev_x[y][x];
		int py = prev_y[y][x];
		x = px, y = py;
		std::cout << "2:" << x << "," << y << std::endl;
	}

	printStage(search_stage);

	return 0;
}