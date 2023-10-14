// ReSharper disable All
#pragma once
#include "BaseSkill.h"


// 1.剛腕(攻撃力が1.25倍上がる)
class StrongArm : public BaseSkill
{
public:
    StrongArm() :BaseSkill("StrongArm"){}
    ~StrongArm() = default;

    void Init() override;
    void Fin() override;
    
};

// 2.鬼力(敵を倒すと攻撃力のステータスが50上がる)
class DemonPower:public BaseSkill
{
public:
    DemonPower() :BaseSkill("DemonPower"){}
    ~DemonPower() = default;

    void Update() override;

    int kill_count = 0;
};

// 3.魔剣(連撃のダメージが上がる)
class MagicSword :public BaseSkill
{
public:
    MagicSword():BaseSkill("MagicSword"){}

    void Init() override;
};

// 4.残酷(連撃する度に攻撃力が上がる)
class Cruel :public BaseSkill
{
public:
    Cruel():BaseSkill("Cruel"){}

    void Update() override;
};

// 5.復讐(HPが少ない時に攻撃力が上がる)
class Revenge :public BaseSkill
{
public:
    Revenge() :BaseSkill("Revenge",1){}

    void DrawImGui() override;
    void Init() override;
    void Update() override;
    // 前フレームのHP保持
    int old_health;

    // HP減少率にかける攻撃力
    float basic_AP;

    // このスキルでどれだけの攻撃力が増えたか
    float power_up_value;
};

// 8.聖剣(通常攻撃のダメージが上がる)
class HolySword :public BaseSkill
{
public:
    HolySword():BaseSkill("HolySword"){}

    void Init()override;
};

// 22.剣聖(攻撃力が1.15倍上がる)
class SwordSaint:public BaseSkill
{
public:
    SwordSaint():BaseSkill("SwordSaint"){}

    void Init() override;
};