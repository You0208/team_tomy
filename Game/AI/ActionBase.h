// ReSharper disable All
#pragma once

class Enemy;

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
protected:
	Enemy* owner;

	// �����i�s�x
	int step = 0;
};
