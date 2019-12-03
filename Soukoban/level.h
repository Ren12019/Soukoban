#pragma once
#include <string>
#include "define.h"

class Level
{
public:
	Level();
	~Level();
	int stage[HEIGHT][WIDTH] = {};
	void createLevel();//�X�e�[�W�������s��
	void setStage();//��̕����ɔz�u�������ׂăZ�b�g����
	void resetStage();//��̕����͂��̂܂܂Ŕz�u�������Z�b�g����
	void printStage();//�X�e�[�W��\������
	void outputStage();//�X�e�[�W���e�L�X�g�t�@�C���Ƃ��ďo��
	std::string outputString();//�X�e�[�W��string�^�ŏo��
private:
	void createEmptyRoom();//��̕��������
	void fillBlindAlley();//�X�e�[�W�̑܏��H���Ԃ�
	bool setGoal();//�X�e�[�W�ɃS�[����ݒu����
	bool setBox();//�X�e�[�W�ɉו���ݒu����
	bool setPlayer();//�X�e�[�W�Ƀv���C���[��z�u����
	bool checkSection();//������2�ȏ�ɂȂ��Ă��Ȃ����m�F
	int countSpace();//�X�e�[�W�̒ʉ߉\�������J�E���g����
};