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

class ClawAttackJudgment : public JudgmentBase
{
public:
	ClawAttackJudgment(Enemy* enemy) :JudgmentBase(enemy) {}
	// ”»’è
	bool Judgment();
};