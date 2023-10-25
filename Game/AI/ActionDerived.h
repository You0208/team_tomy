// ReSharper disable All
#pragma once
#include "ActionBase.h"
#include <DirectXMath.h>
#include "Lemur/Effekseer/Effect.h"

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

// 軸合わせ
class TuraAction : public ActionBase
{
public:
	TuraAction(Enemy* enemy) :ActionBase(enemy) {}
	ActionBase::State Run(float elapsedTime)override;

	float target_vec_x;
	float target_vec_z;
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
	NearAttackAction(Enemy* enemy):ActionBase(enemy)
	{
		start_collision__frame = 60;
		end_collision__frame = 100;
	}
	ActionBase::State Run(float elapsedTime) override;


};

// 死亡
class DeathAction:public ActionBase
{
public:
	DeathAction(Enemy* enemy) :ActionBase(enemy) {}
	ActionBase::State Run(float elapsedTime) override;

};

// 飛びつき攻撃
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

// 二段攻撃
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

// 両腕攻撃
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

// 毒ブレス攻撃
class PoisonAttackAction : public ActionBase
{
public:
	PoisonAttackAction(Enemy* enemy) :ActionBase(enemy)
	{

		poison = std::make_unique<Effect>("./resources/Effect/team0925/poison_02.efk");
	}
	ActionBase::State Run(float elapsedTime)override;

	// イージングする時間(秒)
	float easing_time_ms = 1.0f;
	float easing_timer_ms;

	// 現在の毒エフェクトの座標
    DirectX::XMFLOAT3 poison_pos{};
    //毒エフェクトの半径
	float poison_radius = 1.2f;

	// 毒エフェクト開始位置
	DirectX::XMFLOAT3 start_pos{};
	// 毒エフェクトが向かう位置
	DirectX::XMFLOAT3 target_pos{};

	std::unique_ptr<Effect> poison;

	Effekseer::Handle handle;
	// 毒弾の飛距離
	float shot_length = 10.0f;
};

// 突進攻撃
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

	// 走り去る距離
	float run_over_length = 5.0f;
};

// バックステップ
class BackStepAction : public ActionBase
{
public:
	BackStepAction(Enemy* enemy) :ActionBase(enemy) {}
	ActionBase::State Run(float elapsedTime)override;

	// 下がる距離
	float length = 13.0f;

	int jump_anim_start_frame = 30;
	int jump_anim_end_frame = 138;

	// イージングする時間(秒)
	float easing_time_ms = static_cast<float>(jump_anim_end_frame - jump_anim_start_frame)/60;
	float easing_timer_ms;

	// バックステップ開始座標
	DirectX::XMFLOAT3 start_pos;

	// バックステップ終了地点
	DirectX::XMFLOAT3 end_pos;

};

// 怯み
class FearAction : public ActionBase
{
public:
	FearAction(Enemy* enemy) :ActionBase(enemy){}
	ActionBase::State Run(float elapsedTime)override;


};
