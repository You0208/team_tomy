// ReSharper disable All
#pragma once
#include "ActionBase.h"

// �p�j�s��
class WanderAction : public ActionBase
{
public:
	WanderAction(Enemy* enemy) :ActionBase(enemy) {}
	ActionBase::State Run(float elapsedTime)override;

	float wander_speed_facter = 1.0f;
};

// �܍U��
class ClawAttackAction : public ActionBase
{
public:
	ClawAttackAction(Enemy* enemy) :ActionBase(enemy) {}
	ActionBase::State Run(float elapsedTime)override;

	float wander_speed_facter = 1.0f;
};
