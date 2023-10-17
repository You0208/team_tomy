// ReSharper disable All
#pragma once
#include "JudgmentBase.h"

class WanderJudgment : public JudgmentBase
{
public:
	WanderJudgment(Enemy* enemy) :JudgmentBase(enemy) {}
	// ”»’è
	bool Judgment();
};

class BattleJudgment:public JudgmentBase
{
public:
	BattleJudgment(Enemy* enemy):JudgmentBase(enemy){}
	// ”»’è
	bool Judgment() override;
};

class DeathJudgment:public JudgmentBase
{
public:
	DeathJudgment(Enemy* enemy):JudgmentBase(enemy){}
	// ”»’è
	bool Judgment() override;
};

class MiddleJudgment:public JudgmentBase
{
public:
	MiddleJudgment(Enemy* enemy):JudgmentBase(enemy){}
	// ”»’è
	bool Judgment() override;
};

class NearJudgment:public JudgmentBase
{
public:
	NearJudgment(Enemy* enemy):JudgmentBase(enemy){}
	// ”»’è
	bool Judgment() override;
};

class FearJudgment:public JudgmentBase
{
public:
	FearJudgment(Enemy* enemy):JudgmentBase(enemy){}
	// ”»’è
	bool Judgment() override;
};

class ClawAttackJudgment : public JudgmentBase
{
public:
	ClawAttackJudgment(Enemy* enemy) :JudgmentBase(enemy) {}
	// ”»’è
	bool Judgment();
};