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
};