// ReSharper disable All
#pragma once
#include "BaseSkill.h"

// 1.����(�ꌂ�ł��_���[�W���󂯂Ă��܂��Ǝ���ł��܂�(�o�b�g�X�L��))
class Tofu :
    public BaseSkill
{
public:
    Tofu() :BaseSkill("Tofu"){}
    ~Tofu() = default;

    // ���O�����Ŏg�����炱���ɂ͏��������Ȃ�
};

// 2.���f(������HP�������Ă������U���͂����Ԍo�߂ŏオ��(�o�b�g�X�L��))
class Curse :public BaseSkill
{
public:
    Curse():BaseSkill("Curse"){}
    
    void Update() override;
    float damage_time = 5.0f;
    float damage_timer = 0.0f;
    int damage_value = 50;

};

// 6.����(���Ԍo�߂ɂ��U���͂�������)
class Arrogance :public BaseSkill
{
public:
    Arrogance():BaseSkill("Arrogance"){}
    
    void Update() override;
    float time_ms = 5.0f;
    float timer_ms = 0.0f;
    float value = 5;

};