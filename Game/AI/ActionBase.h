// ReSharper disable All
#pragma once

#include "Game/Enemy/Enemy.h"
// �s���������N���X

class ActionBase
{
public:
	ActionBase(Enemy* enemy) :owner(enemy) {}
	virtual ~ActionBase() = default;
	// ���s���
	enum class State
	{
		Run,		// ���s��
		Failed,		// ���s���s
		Complete,	// ���s����
	};

	// ���s����(�������z�֐�)
	// ������I�[�o���C�h���Ď��ۂ̓��������
	virtual ActionBase::State Run(float elapsedTime) = 0;

	// ����t���[���̊Ԃ����U������\(�߂�ǂ�����֐���)
	void CollisionFragJudge()
	{
		if (owner->GetFrameIndex() >= start_collision__frame &&
			owner->GetFrameIndex() <= end_collision__frame)
			owner->attack_collision_flag = true;

		else owner->attack_collision_flag = false;

	}


protected:
	Enemy* owner;

	// �����i�s�x
	int step = 0;

	// �����蔻��J�n�t���[��
	int start_collision__frame;
	// �����蔻��I���t���[��
	int end_collision__frame;

};
