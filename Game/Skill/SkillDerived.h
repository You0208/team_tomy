// ReSharper disable All
#pragma once
#include "Game/Skill/BaseSkill.h"

// 9.�z��(�G�Ƀ_���[�W��^�����HP��)
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

// 12.����(���Ԃ��o���тɃX�s�[�h���オ��)
class Acceleration:public BaseSkill
{
public:
    Acceleration():BaseSkill("Acceleration",1){}

    void Init() override;
    void Update() override;

    float max_SP;
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
    float heal_time = 20.0f;
    float heal_timer = 0.0f;
    int heal_power = 5;
};

// 16.�t�](�G��|�����R�O�b�Ԃ����X�e�[�^�X���S�̓I�ɏオ��)
class Reverse:public BaseSkill
{
public:
    Reverse():BaseSkill("Reverse"){}

    void Update() override;

    int kill_count = 0;

    // �������鎞��
    float power_up_timer = 0.0f;

    int basic_max_health;
    int basic_health;
    float basic_attack_power;
    float basic_defense_power;
    float basic_speed_power;

};

// 21.���l(�퓬���X�e�[�^�X��2�{�ɂȂ�)
class SuperMan:public BaseSkill
{
public:
    SuperMan() :BaseSkill("SuperMan"){}
    void Init() override;
};

// 23.����(�X�s�[�h��1.15�{�オ��)
class Gale :public BaseSkill
{
public:
    Gale() :BaseSkill("Gale"){}

    void Init() override;
};

// 24.�얞(HP��1.25�{�オ��)
class Obesity:public BaseSkill
{
public:
    Obesity():BaseSkill("Obesity"){}

    void Init() override;
};