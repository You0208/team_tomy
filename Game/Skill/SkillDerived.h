// ReSharper disable All
#pragma once
#include "Game/Skill/BaseSkill.h"



// �z��(�G�Ƀ_���[�W��^�����HP��)
class BloodSucking:public BaseSkill
{
public:
    BloodSucking() :BaseSkill("BloodSucking") {}
    ~BloodSucking() = default;

    void Update() override;
};