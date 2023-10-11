// ReSharper disable All
#pragma once
#include "Game/Skill/BaseSkill.h"

// 吸血(敵にダメージを与えるとHP回復)
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
    float heal_time = 5.0f;
    float heal_timer = 0.0f;
    int heal_power = 5;
};