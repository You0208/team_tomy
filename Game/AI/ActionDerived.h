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
	NearAttackAction(Enemy* enemy):ActionBase(enemy){}
	ActionBase::State Run(float elapsedTime) override;

	// �����蔻��J�n�t���[��
	int start_collision__frame = 30;
	// �����蔻��I���t���[��
	int end_collision__frame = 60;

};

// ���S
class DeathAction:public ActionBase
{
public:
	DeathAction(Enemy* enemy) :ActionBase(enemy) {}
	ActionBase::State Run(float elapsedTime) override;

};

// �܍U��
class ClawAttackAction : public ActionBase
{
public:
	ClawAttackAction(Enemy* enemy) :ActionBase(enemy) {}
	ActionBase::State Run(float elapsedTime)override;

	float wander_speed_facter = 1.0f;
};
