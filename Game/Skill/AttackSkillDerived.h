// ReSharper disable All
#pragma once
#include "BaseSkill.h"


// 1.���r(�U���͂�1.25�{�オ��)
class StrongArm : public BaseSkill
{
public:
    StrongArm() :BaseSkill("StrongArm"){}
    ~StrongArm() = default;

    void Init() override;
    void Fin() override;

    //todo ���c���� �����炪�e�X�L���̃N���X�ł��B������ɊG���������đI������Ă���͕\�������݂����Ȃ����Ă��������B���肢���܂�

};

// 2.�S��(�G��|���ƍU���͂̃X�e�[�^�X��50�オ��)
class DemonPower:public BaseSkill
{
public:
    DemonPower() :BaseSkill("DemonPower"){}
    ~DemonPower() = default;

    void Update() override;

    int kill_count = 0;
};

// 3.����(�A���̃_���[�W���オ��)
class MagicSword :public BaseSkill
{
public:
    MagicSword():BaseSkill("MagicSword"){}

    void Init() override;
};

// 4.�c��(�A������x�ɍU���͂��オ��)
class Cruel :public BaseSkill
{
public:
    Cruel():BaseSkill("Cruel"){}

    void Update() override;
};

// 5.���Q(HP�����Ȃ����ɍU���͂��オ��)
class Revenge :public BaseSkill
{
public:
    Revenge() :BaseSkill("Revenge",1){}

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

// 8.����(�ʏ�U���̃_���[�W���オ��)
class HolySword :public BaseSkill
{
public:
    HolySword():BaseSkill("HolySword"){}

    void Init()override;
};

// 22.����(�U���͂�1.15�{�オ��)
class SwordSaint:public BaseSkill
{
public:
    SwordSaint():BaseSkill("SwordSaint"){}

    void Init() override;
};