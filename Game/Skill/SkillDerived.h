// ReSharper disable All
#pragma once
#include "Game/Skill/BaseSkill.h"

class StrongArm: public BaseSkill
{
    StrongArm(Player* player, std::string name) :BaseSkill(player, name){}
    ~StrongArm() = default;

    void Init() override;
    void Update() override;
    void Fin() override;
};

// �z��(�G�Ƀ_���[�W��^�����HP��)
class Bloodsucking:public BaseSkill
{
    
};