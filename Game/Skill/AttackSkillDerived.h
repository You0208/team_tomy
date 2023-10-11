// ReSharper disable All
#pragma once
#include "BaseSkill.h"


// „˜r(UŒ‚—Í‚ª1.25”{ã‚ª‚é)
class StrongArm : public BaseSkill
{
public:
    StrongArm() :BaseSkill("StrongArm") {}
    ~StrongArm() = default;

    void Init() override;
};

// ‹S—Í(“G‚ğ“|‚·‚ÆUŒ‚—Í‚ÌƒXƒe[ƒ^ƒX‚ª50ã‚ª‚é)
class DemonPower:public BaseSkill
{
public:
    DemonPower() :BaseSkill("DemonPower"){}
    ~DemonPower() = default;

    void Update() override;

    int kill_count = 0;
};