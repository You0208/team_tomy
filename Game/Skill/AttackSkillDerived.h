// ReSharper disable All
#pragma once
#include "BaseSkill.h"


// ���r(�U���͂�1.25�{�オ��)
class StrongArm : public BaseSkill
{
public:
    StrongArm() :BaseSkill("StrongArm") {}
    ~StrongArm() = default;

    void Init() override;
};

// �S��(�G��|���ƍU���͂̃X�e�[�^�X��50�オ��)
class DemonPower:public BaseSkill
{
public:
    DemonPower() :BaseSkill("DemonPower"){}
    ~DemonPower();

    void Update() override;

    int kill_count = 0;
};