// ReSharper disable All
#pragma once
#include "ActionBase.h"
#include <DirectXMath.h>

// �g�����킩���
// �p�j�s��
class WanderAction : public ActionBase
{
public:
	WanderAction(Enemy* enemy) :ActionBase(enemy) {}
	ActionBase::State Run(float elapsedTime)override;

	float wander_speed_facter = 1.0f;

	DirectX::XMFLOAT3 target_pos = {};
};

// �ҋ@
class IdleAction : public ActionBase
{
public:
	IdleAction(Enemy* enemy) :ActionBase(enemy) {}
	ActionBase::State Run(float elapsedTime)override;

	float idleing_time;
};

// �����킹
class TuraAction : public ActionBase
{
public:
	TuraAction(Enemy* enemy) :ActionBase(enemy) {}
	ActionBase::State Run(float elapsedTime)override;

	float target_vec_x;
	float target_vec_z;
};

//�ǐ�
class PursueAction:public ActionBase
{
public:
	PursueAction(Enemy* enemy):ActionBase(enemy){}
	ActionBase::State Run(float elapsedTime) override;

};

//�ߍU��
class NearAttackAction:public ActionBase
{
public:
	NearAttackAction(Enemy* enemy):ActionBase(enemy)
	{
		start_collision__frame = 60;
		end_collision__frame = 100;
	}
	ActionBase::State Run(float elapsedTime) override;


};

// ���S
class DeathAction:public ActionBase
{
public:
	DeathAction(Enemy* enemy) :ActionBase(enemy) {}
	ActionBase::State Run(float elapsedTime) override;

};

// ��т��U��
class JumpAttackAction : public ActionBase
{
public:
	JumpAttackAction(Enemy* enemy) :ActionBase(enemy)
	{
		start_collision__frame = 60;
		end_collision__frame = 100;

	}
	ActionBase::State Run(float elapsedTime)override;

};

// ��i�U��
class DoubleAttackAction : public ActionBase
{
public:
	DoubleAttackAction(Enemy* enemy) :ActionBase(enemy)
	{
		start_collision__frame = 60;
		end_collision__frame = 100;

	}
	ActionBase::State Run(float elapsedTime)override;

};

// ���r�U��
class TwinArmsAttackAction : public ActionBase
{
public:
	TwinArmsAttackAction(Enemy* enemy) :ActionBase(enemy)
	{
		start_collision__frame = 60;
		end_collision__frame = 100;
	}
	ActionBase::State Run(float elapsedTime)override;

};

// �Ńu���X�U��
class PoisonAttackAction : public ActionBase
{
public:
	PoisonAttackAction(Enemy* enemy) :ActionBase(enemy)
	{
		start_collision__frame = 60;
		end_collision__frame = 100;
	}
	ActionBase::State Run(float elapsedTime)override;

};

// �Ńu���X�U��
class RushAttackAction : public ActionBase
{
public:
	RushAttackAction(Enemy* enemy) :ActionBase(enemy)
	{
		start_collision__frame = 60;
		end_collision__frame = 100;
	}
	ActionBase::State Run(float elapsedTime)override;

	void CalcTargetPos();

	// ���苎�鋗��
	float run_over_length = 3.0f;
};

// �o�b�N�X�e�b�v
class BackStepAction : public ActionBase
{
public:
	BackStepAction(Enemy* enemy) :ActionBase(enemy){}
	ActionBase::State Run(float elapsedTime)override;

	// �����鋗��
	float length = 10.0f;

	float easing_time = 1.0;
	float easing_timer;

	// �o�b�N�X�e�b�v�J�n���W
	DirectX::XMFLOAT3 start_pos;

	// �o�b�N�X�e�b�v�I���n�_
	DirectX::XMFLOAT3 end_pos;

};

// ����
class FearAction : public ActionBase
{
public:
	FearAction(Enemy* enemy) :ActionBase(enemy){}
	ActionBase::State Run(float elapsedTime)override;


};
