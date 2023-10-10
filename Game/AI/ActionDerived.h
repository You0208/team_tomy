// ReSharper disable All
#pragma once
#include "ActionBase.h"
#include <DirectXMath.h>

// 使うかわからん
// 徘徊行動
class WanderAction : public ActionBase
{
public:
	WanderAction(Enemy* enemy) :ActionBase(enemy) {}
	ActionBase::State Run(float elapsedTime)override;

	float wander_speed_facter = 1.0f;

	DirectX::XMFLOAT3 target_pos = {};
};

// 待機
class IdleAction : public ActionBase
{
public:
	IdleAction(Enemy* enemy) :ActionBase(enemy) {}
	ActionBase::State Run(float elapsedTime)override;

	float idleing_time;
};

//追跡
class PursueAction:public ActionBase
{
public:
	PursueAction(Enemy* enemy):ActionBase(enemy){}
	ActionBase::State Run(float elapsedTime) override;

};

//近攻撃
class NearAttackAction:public ActionBase
{
public:
	NearAttackAction(Enemy* enemy):ActionBase(enemy){}
	ActionBase::State Run(float elapsedTime) override;

	// 当たり判定開始フレーム
	int start_collision__frame = 30;
	// 当たり判定終了フレーム
	int end_collision__frame = 60;

};

// 死亡
class DeathAction:public ActionBase
{
public:
	DeathAction(Enemy* enemy) :ActionBase(enemy) {}
	ActionBase::State Run(float elapsedTime) override;

};

// 爪攻撃
class ClawAttackAction : public ActionBase
{
public:
	ClawAttackAction(Enemy* enemy) :ActionBase(enemy) {}
	ActionBase::State Run(float elapsedTime)override;

	float wander_speed_facter = 1.0f;
};
