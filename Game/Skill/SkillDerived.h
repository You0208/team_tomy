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

// 11.����(�X�s�[�h��1.25�{�オ��)
class Sprint:public BaseSkill
{
public:
    Sprint():BaseSkill("Sprint"){}
    ~Sprint();

    void Init() override;
};

// 13.�䖝(HP��������_���[�W�ł��P���͂P��ς��邱�Ƃ��ł���)
class Patience:public BaseSkill
{
public:
    Patience():BaseSkill("Patience"){}

    void Update() override;
    bool is_used = false;
};

// 14.�Đ�(HP���������񕜂���)
class Regeneration:public BaseSkill
{
public:
    Regeneration():BaseSkill("Regeneration"){}

    void Update() override;
    float heal_time = 30.0f;
    float heal_timer = 0.0f;
    int heal_power = 5;
};