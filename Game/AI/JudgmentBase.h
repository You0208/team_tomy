// ReSharper disable All
#pragma once

class Enemy;

// ���s����
class JudgmentBase
{
public:
	// �I�[�i�[���Z�b�g
	JudgmentBase(Enemy* enemy) :owner(enemy) {}
	virtual ~JudgmentBase() = default;
	virtual bool Judgment() = 0;
protected:
	Enemy* owner;
};
