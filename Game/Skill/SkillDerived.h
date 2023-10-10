// ReSharper disable All
#pragma once
#include "Game/Skill/BaseSkill.h"



// ‹zŒŒ(“G‚Éƒ_ƒ[ƒW‚ğ—^‚¦‚é‚ÆHP‰ñ•œ)
class BloodSucking:public BaseSkill
{
public:
    BloodSucking() :BaseSkill("BloodSucking") {}
    ~BloodSucking() = default;

    void Update() override;
};