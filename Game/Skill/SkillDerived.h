// ReSharper disable All
#pragma once
#include "Game/Skill/BaseSkill.h"



// z(GÉ_[Wð^¦éÆHPñ)
class BloodSucking:public BaseSkill
{
public:
    BloodSucking() :BaseSkill("BloodSucking") {}
    ~BloodSucking() = default;

    void Update() override;
};