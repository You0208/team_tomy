// ReSharper disable All
#pragma once

class Enemy;

// 実行判定
class JudgmentBase
{
public:
	// オーナーをセット
	JudgmentBase(Enemy* enemy) :owner(enemy) {}
	virtual ~JudgmentBase() = default;
	virtual bool Judgment() = 0;
protected:
	Enemy* owner;
};
