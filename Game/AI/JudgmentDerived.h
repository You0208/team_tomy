// ReSharper disable All
#pragma once
#include "JudgmentBase.h"

class WanderJudgment : public JudgmentBase
{
public:
	WanderJudgment(Enemy* enemy) :JudgmentBase(enemy) {}
	// ����
	bool Judgment();
};

class BattleJudgment:public JudgmentBase
{
public:
	BattleJudgment(Enemy* enemy):JudgmentBase(enemy){}
	// ����
	bool Judgment() override;
};

class DeathJudgment:public JudgmentBase
{
public:
	DeathJudgment(Enemy* enemy):JudgmentBase(enemy){}
	// ����
	bool Judgment() override;
};

class AttackJudgment:public JudgmentBase
{
public:
	AttackJudgment(Enemy* enemy):JudgmentBase(enemy){}
	// ����
	bool Judgment() override;
};

class ClawAttackJudgment : public JudgmentBase
{
public:
	ClawAttackJudgment(Enemy* enemy) :JudgmentBase(enemy) {}
	// ����
	bool Judgment();
};