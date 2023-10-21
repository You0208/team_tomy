// ReSharper disable All
#pragma once
#include "Game/Skill/BaseSkill.h"

// 9.�z��(�G�Ƀ_���[�W��^�����HP��)
class BloodSucking:public BaseSkill
{
public:
    BloodSucking(const wchar_t* ui_spr_filename) :BaseSkill("BloodSucking", ui_spr_filename) {}
    ~BloodSucking() = default;

    void Update() override;
};

// 11.����(�X�s�[�h��1.25�{�オ��)
class Sprint:public BaseSkill
{
public:
    Sprint(const wchar_t* ui_spr_filename):BaseSkill("Sprint", ui_spr_filename){}
    ~Sprint();

    void Init() override;
};

// 12.����(���Ԃ��o���тɃX�s�[�h���オ��)
class Acceleration:public BaseSkill
{
public:
    Acceleration(const wchar_t* ui_spr_filename):BaseSkill("Acceleration",  ui_spr_filename,1){}

    void Init() override;
    void Update() override;

    float max_SP;
};

// 13.�䖝(HP��������_���[�W�ł��P���͂P��ς��邱�Ƃ��ł���)
class Patience:public BaseSkill
{
public:
    Patience(const wchar_t* ui_spr_filename):BaseSkill("Patience", ui_spr_filename){}

    void Update() override;
    // �X�L���g�p������
    bool is_used = false;
};

// 14.�Đ�(HP���������񕜂���)
class Regeneration:public BaseSkill
{
public:
    Regeneration(const wchar_t* ui_spr_filename):BaseSkill("Regeneration", ui_spr_filename){}

    void Update() override;
    // �񕜂̃N�[���^�C��
    float heal_time_ms = 20.0f;
    // �^�C�}�[�A�^�C���ɒB�������
    float heal_timer_ms = 0.0f;
    // �񕜗�
    int heal_power = 5;
};

// 15.�x�e(�퓬�I����R�O % �񕜂���)
class Rest:public BaseSkill
{
public:
    Rest(const wchar_t* ui_spr_filename):BaseSkill("Rest", ui_spr_filename){}

    void Fin() override;
};

// 16.�t�](�G��|�����R�O�b�Ԃ����X�e�[�^�X���S�̓I�ɏオ��)
class Reverse:public BaseSkill
{
public:
    Reverse(const wchar_t* ui_spr_filename):BaseSkill("Reverse", ui_spr_filename){}

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

// 17.��m(�����Ă��q�����X�e�[�^�X������Ȃ�)
class Schemer :public BaseSkill
{
public:
    Schemer(const wchar_t* ui_spr_filename):BaseSkill("Schemer", ui_spr_filename){}

    // ���O�����Ŏg�p����
};

// 21.���l(�퓬���X�e�[�^�X��2�{�ɂȂ�)
class SuperMan:public BaseSkill
{
public:
    SuperMan(const wchar_t* ui_spr_filename) :BaseSkill("SuperMan", ui_spr_filename){}
    void Init() override;
};

// 23.����(�X�s�[�h��1.15�{�オ��)
class Gale :public BaseSkill
{
public:
    Gale(const wchar_t* ui_spr_filename) :BaseSkill("Gale", ui_spr_filename){}

    void Init() override;
};

// 24.�얞(HP��1.25�{�オ��)
class Obesity:public BaseSkill
{
public:
    Obesity(const wchar_t* ui_spr_filename):BaseSkill("Obesity", ui_spr_filename){}

    void Init() override;
};