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
	void setEmptyRoom();//�����𖞂�������̕��������
	void resetStage();//��̕����͂��̂܂܂Ŕz�u�������Z�b�g����
	void printStage();//�X�e�[�W��\������
	void outputStage();//�X�e�[�W���e�L�X�g�t�@�C���Ƃ��ďo��
	std::string outputString();//�X�e�[�W��string�^�ŏo��
	void inputString(std::string);//�X�e�[�W��string�^�œ���
	bool setBoxOnGoal(SQUARE);
	bool setPlayer();//�X�e�[�W�Ƀv���C���[��z�u����
	std::vector<SQUARE> storeCandidate();//�S�[�����u����ꏊ���L���[����
	int countSpace();//�X�e�[�W�̒ʉ߉\�������J�E���g����
private:
	std::vector< std::vector<char> > stage;//�X�e�[�W�}�b�v�̖{��

	void createEmptyRoom();//��̕��������
	void fillBlindAlley();//�X�e�[�W�̑܏��H���Ԃ�
	bool isTwoRoom();//������2�ȏ�ɂȂ��Ă��Ȃ����m�F
	bool canCarryInSquare(const SQUARE);
	std::vector<SQUARE> storeCarryInArea();
	int countAroundWall(const SQUARE);//�Ώۃ}�X�̎��͂̕ǂ��J�E���g����
	bool hasLargeSpace();//���ʂɍL����Ԃ����݂��邩�m�F����
};