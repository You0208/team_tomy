// ReSharper disable All
#pragma once
#include "ActionBase.h"
#include <DirectXMath.h>

// égÇ§Ç©ÇÌÇ©ÇÁÇÒ
// úpújçsìÆ
class WanderAction : public ActionBase
{
public:
	WanderAction(Enemy* enemy) :ActionBase(enemy) {}
	ActionBase::State Run(float elapsedTime)override;

	float wander_speed_facter = 1.0f;

	DirectX::XMFLOAT3 target_pos = {};
};

// égÇ§Ç©ÇÌÇ©ÇÁÇÒ
// úpújçsìÆ
class IdleAction : public ActionBase
{
public:
	IdleAction(Enemy* enemy) :ActionBase(enemy) {}
	ActionBase::State Run(float elapsedTime)override;

	float idleing_time;
};

class PursueAction:public ActionBase
{
public:
	PursueAction(Enemy* enemy):ActionBase(enemy){}
	ActionBase::State Run(float elapsedTime) override;


};

// í‹çUåÇ
class ClawAttackAction : public ActionBase
{
public:
	ClawAttackAction(Enemy* enemy) :ActionBase(enemy) {}
	ActionBase::State Run(float elapsedTime)override;

	float wander_speed_facter = 1.0f;
};
