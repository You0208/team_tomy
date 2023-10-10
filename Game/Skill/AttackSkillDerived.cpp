#include "AttackSkillDerived.h"
#include "Game/Player/Player.h"

void StrongArm::Init()
{
    owner->attack_power *= 1.25f;
}

void DemonPower::Update()
{
    // �X�L�������҂̃L����
    int owner_kill_count = owner->GetKillCount();

    // �O�t���[�����猂�j���������Ă���U���͂𑝉�
    if(this->kill_count<owner_kill_count)
    {
        owner->attack_power += 50;
        this->kill_count = owner_kill_count;
    }
}

