#pragma once
#include <string>
#include <vector>
#include <queue>
#include "define.h"

class Level
{
public:
	Level();
	~Level();
	std::vector< std::vector<char> > stage;
	void setEmptyRoom();//�����𖞂�������̕��������
	void createLevel();//�X�e�[�W�������s��
	void setStage();//��̕����ɔz�u�������ׂăZ�b�g����
	void resetStage();//��̕����͂��̂܂܂Ŕz�u�������Z�b�g����
	void printStage();//�X�e�[�W��\������
	void outputStage();//�X�e�[�W���e�L�X�g�t�@�C���Ƃ��ďo��
	std::string outputString();//�X�e�[�W��string�^�ŏo��
	void inputString(std::string);//�X�e�[�W��string�^�œ���
	bool setBoxOnGoal();
	bool setBoxOnGoal(SQUARE);
	bool setPlayer();//�X�e�[�W�Ƀv���C���[��z�u����
	std::vector<SQUARE> decisionCanditdate();//�S�[�����u����ꏊ���L���[����
private:
	void createEmptyRoom();//��̕��������
	void fillBlindAlley();//�X�e�[�W�̑܏��H���Ԃ�
	bool setGoal();//�X�e�[�W�ɃS�[����ݒu����
	bool setBox();//�X�e�[�W�ɉו���ݒu����
	bool checkSection();//������2�ȏ�ɂȂ��Ă��Ȃ����m�F
	int countSpace();//�X�e�[�W�̒ʉ߉\�������J�E���g����
	bool checkCarryInSquare(const int, const int);
	std::vector<SQUARE> checkCarryInArea();
	int countAroundWall(const int, const int);
	bool checkSquare();
	bool checkCornerSquare(const int, const int);
	std::vector<SQUARE> checkPutBox();
	bool checkDeadlock(const int, const int);
	std::vector<SQUARE> checkPutPlayer();
	bool checkLargeSpace();//���ʂɍL����Ԃ����݂��邩�m�F����
};