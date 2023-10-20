// ReSharper disable All
#pragma once
#include "Game/Skill/BaseSkill.h"

// 9.吸血(敵にダメージを与えるとHP回復)
class BloodSucking:public BaseSkill
{
public:
    BloodSucking(const wchar_t* ui_spr_filename) :BaseSkill("BloodSucking", ui_spr_filename) {}
    ~BloodSucking() = default;

    void Update() override;
};

// 11.疾走(スピードが1.25倍上がる)
class Sprint:public BaseSkill
{
public:
    Sprint(const wchar_t* ui_spr_filename):BaseSkill("Sprint", ui_spr_filename){}
    ~Sprint();

    void Init() override;
};

// 12.加速(時間が経つたびにスピードが上がる)
class Acceleration:public BaseSkill
{
public:
    Acceleration(const wchar_t* ui_spr_filename):BaseSkill("Acceleration",  ui_spr_filename,1){}

    void Init() override;
    void Update() override;

    float max_SP;
};

// 13.我慢(HPが超えるダメージでも１撃は１回耐えることができる)
class Patience:public BaseSkill
{
public:
    Patience(const wchar_t* ui_spr_filename):BaseSkill("Patience", ui_spr_filename){}

    void Update() override;
    bool is_used = false;
};

// 14.再生(HPが少しずつ回復する)
class Regeneration:public BaseSkill
{
public:
    Regeneration(const wchar_t* ui_spr_filename):BaseSkill("Regeneration", ui_spr_filename){}

    void Update() override;
    float heal_time = 20.0f;
    float heal_timer = 0.0f;
    int heal_power = 5;
};

// 16.逆転(敵を倒した３０秒間だけステータスが全体的に上がる)
class Reverse:public BaseSkill
{
public:
    Reverse(const wchar_t* ui_spr_filename):BaseSkill("Reverse", ui_spr_filename){}

    void Update() override;

    int kill_count = 0;

    // 強化する時間
    float power_up_timer = 0.0f;

    int basic_max_health;
    int basic_health;
    float basic_attack_power;
    float basic_defense_power;
    float basic_speed_power;

};

// 21.超人(戦闘時ステータスが2倍になる)
class SuperMan:public BaseSkill
{
public:
    SuperMan(const wchar_t* ui_spr_filename) :BaseSkill("SuperMan", ui_spr_filename){}
    void Init() override;
};

// 23.疾風(スピードが1.15倍上がる)
class Gale :public BaseSkill
{
public:
    Gale(const wchar_t* ui_spr_filename) :BaseSkill("Gale", ui_spr_filename){}

    void Init() override;
};

// 24.肥満(HPが1.25倍上がる)
class Obesity:public BaseSkill
{
public:
    Obesity(const wchar_t* ui_spr_filename):BaseSkill("Obesity", ui_spr_filename){}

    void Init() override;
};