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
