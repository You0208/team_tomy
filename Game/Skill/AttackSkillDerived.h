// ReSharper disable All
#pragma once
#include "BaseSkill.h"


// 1.剛腕(攻撃力が1.25倍上がる)
class StrongArm : public BaseSkill
{
public:
    StrongArm(const wchar_t* ui_spr_filename) :BaseSkill("StrongArm",ui_spr_filename){}
    ~StrongArm() = default;

    void Init() override;
    void Fin() override;


};

// 2.鬼力(敵を倒すと攻撃力のステータスが50上がる)
class DemonPower:public BaseSkill
{
public:
    DemonPower(const wchar_t* ui_spr_filename) :BaseSkill("DemonPower", ui_spr_filename){}
    ~DemonPower() = default;

    void Update() override;

    int kill_count = 0;
};

// 3.魔剣(連撃のダメージが上がる)
class MagicSword :public BaseSkill
{
public:
    MagicSword(const wchar_t* ui_spr_filename):BaseSkill("MagicSword", ui_spr_filename){}

    void Init() override;
};

// 4.残酷(連撃する度に攻撃力が上がる)
class Cruel :public BaseSkill
{
public:
    Cruel(const wchar_t* ui_spr_filename):BaseSkill("Cruel", ui_spr_filename){}

    void Update() override;
};

// 5.復讐(HPが少ない時に攻撃力が上がる)
class Revenge :public BaseSkill
{
public:
    Revenge(const wchar_t* ui_spr_filename) :BaseSkill("Revenge", ui_spr_filename,1){}

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

// 6.狂乱(特殊攻撃の与えるダメージが上がるが受けるダメージも上がる)
class Frenzy :public BaseSkill
{
public:
    Frenzy(const wchar_t* ui_spr_filename) :BaseSkill("Frenzy", ui_spr_filename){}

    void Init() override;

};

// 7.技術(特殊攻撃のダメージが上がるが、クールタイムが伸びる)
class Technique :public BaseSkill
{
public:
    Technique(const wchar_t* ui_spr_filename) :BaseSkill("Technique", ui_spr_filename){}

    void Init() override;

};

// 8.聖剣(通常攻撃のダメージが上がる)
class HolySword :public BaseSkill
{
public:
    HolySword(const wchar_t* ui_spr_filename):BaseSkill("HolySword", ui_spr_filename){}

    void Init()override;
};

// 22.剣聖(攻撃力が1.15倍上がる)
class SwordSaint:public BaseSkill
{
public:
    SwordSaint(const wchar_t* ui_spr_filename):BaseSkill("SwordSaint", ui_spr_filename){}

    void Init() override;
};