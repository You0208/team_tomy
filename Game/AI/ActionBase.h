// ReSharper disable All
#pragma once

#include "Game/Enemy/Enemy.h"
// 行動処理基底クラス

class ActionBase
{
public:
	ActionBase(Enemy* enemy) :owner(enemy) {}
	virtual ~ActionBase() = default;
	// 実行情報
	enum class State
	{
		Run,		// 実行中
		Failed,		// 実行失敗
		Complete,	// 実行成功
	};

	// 実行処理(純粋仮想関数)
	// これをオーバライドして実際の動きを作る
	virtual ActionBase::State Run(float elapsedTime) = 0;

	// 限定フレームの間だけ攻撃判定可能(めんどいから関数化)
	void CollisionFragJudge()
	{
		if (owner->GetFrameIndex() >= start_collision__frame &&
			owner->GetFrameIndex() <= end_collision__frame)
			owner->attack_collision_flag = true;

		else owner->attack_collision_flag = false;

	}


protected:
	Enemy* owner;

	// 処理進行度
	int step = 0;

	// 当たり判定開始フレーム
	int start_collision__frame;
	// 当たり判定終了フレーム
	int end_collision__frame;

};
