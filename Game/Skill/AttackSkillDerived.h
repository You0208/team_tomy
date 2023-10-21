// ReSharper disable All
#pragma once
#include "BaseSkill.h"


// 1.���r(�U���͂�1.25�{�オ��)
class StrongArm : public BaseSkill
{
public:
    StrongArm(const wchar_t* ui_spr_filename) :BaseSkill("StrongArm",ui_spr_filename){}
    ~StrongArm() = default;

    void Init() override;
    void Fin() override;

    //todo ���c���� �����炪�e�X�L���̃N���X�ł��B������ɕ�����ݒ肵�Ă�������

};

// 2.�S��(�G��|���ƍU���͂̃X�e�[�^�X��50�オ��)
class DemonPower:public BaseSkill
{
public:
    DemonPower(const wchar_t* ui_spr_filename) :BaseSkill("DemonPower", ui_spr_filename){}
    ~DemonPower() = default;

    void Update() override;

    int kill_count = 0;
};

// 3.����(�A���̃_���[�W���オ��)
class MagicSword :public BaseSkill
{
public:
    MagicSword(const wchar_t* ui_spr_filename):BaseSkill("MagicSword", ui_spr_filename){}

    void Init() override;
};

// 4.�c��(�A������x�ɍU���͂��オ��)
class Cruel :public BaseSkill
{
public:
    Cruel(const wchar_t* ui_spr_filename):BaseSkill("Cruel", ui_spr_filename){}

    void Update() override;
};

// 5.���Q(HP�����Ȃ����ɍU���͂��オ��)
class Revenge :public BaseSkill
{
public:
    Revenge(const wchar_t* ui_spr_filename) :BaseSkill("Revenge", ui_spr_filename,1){}

    void DrawImGui() override;
    void Init() override;
    void Update() override;
    // �O�t���[����HP�ێ�
    int old_health;

    // HP�������ɂ�����U����
    float basic_AP;

    // ���̃X�L���łǂꂾ���̍U���͂���������
    float power_up_value;
};

// 6.����(����U���̗^����_���[�W���オ�邪�󂯂�_���[�W���オ��)
class Frenzy :public BaseSkill
{
public:
    Frenzy(const wchar_t* ui_spr_filename) :BaseSkill("Frenzy", ui_spr_filename){}

    void Init() override;

};

// 7.�Z�p(����U���̃_���[�W���オ�邪�A�N�[���^�C�����L�т�)
class Technique :public BaseSkill
{
public:
    Technique(const wchar_t* ui_spr_filename) :BaseSkill("Technique", ui_spr_filename){}

    void Init() override;

};

// 8.����(�ʏ�U���̃_���[�W���オ��)
class HolySword :public BaseSkill
{
public:
    HolySword(const wchar_t* ui_spr_filename):BaseSkill("HolySword", ui_spr_filename){}

    void Init()override;
};

// 22.����(�U���͂�1.15�{�オ��)
class SwordSaint:public BaseSkill
{
public:
    SwordSaint(const wchar_t* ui_spr_filename):BaseSkill("SwordSaint", ui_spr_filename){}

    void Init() override;
};