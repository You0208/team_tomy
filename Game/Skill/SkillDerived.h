// ReSharper disable All
#pragma once
#include "Game/Skill/BaseSkill.h"

// 9.吸血(敵にダメージを与えるとHP回復)
class BloodSucking:public BaseSkill
{
public:
    BloodSucking() :BaseSkill("BloodSucking") {}
    ~BloodSucking() = default;

    void Update() override;
};

// 11.疾走(スピードが1.25倍上がる)
class Sprint:public BaseSkill
{
public:
    Sprint():BaseSkill("Sprint"){}
    ~Sprint();

    void Init() override;
};

// 12.加速(時間が経つたびにスピードが上がる)
class Acceleration:public BaseSkill
{
public:
    Acceleration():BaseSkill("Acceleration",1){}

    void Init() override;
    void Update() override;

    float max_SP;
};

// 13.我慢(HPが超えるダメージでも１撃は１回耐えることができる)
class Patience:public BaseSkill
{
public:
    Patience():BaseSkill("Patience"){}

    void Update() override;
    bool is_used = false;
};

// 14.再生(HPが少しずつ回復する)
class Regeneration:public BaseSkill
{
public:
    Regeneration():BaseSkill("Regeneration"){}

    void Update() override;
    float heal_time = 20.0f;
    float heal_timer = 0.0f;
    int heal_power = 5;
};

// 16.逆転(敵を倒した３０秒間だけステータスが全体的に上がる)
class Reverse:public BaseSkill
{
public:
    Reverse():BaseSkill("Reverse"){}

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
    SuperMan() :BaseSkill("SuperMan"){}
    void Init() override;
};

// 23.疾風(スピードが1.15倍上がる)
class Gale :public BaseSkill
{
public:
    Gale() :BaseSkill("Gale"){}

    void Init() override;
};

// 24.肥満(HPが1.25倍上がる)
class Obesity:public BaseSkill
{
public:
    Obesity():BaseSkill("Obesity"){}

    void Init() override;
};