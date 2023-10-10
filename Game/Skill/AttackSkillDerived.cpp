#include "AttackSkillDerived.h"
#include "Game/Player/Player.h"

void StrongArm::Init()
{
    owner->attack_power *= 1.25f;
}

void DemonPower::Update()
{
    // スキル所持者のキル数
    int owner_kill_count = owner->GetKillCount();

    // 前フレームから撃破数が増えてたら攻撃力を増加
    if(this->kill_count<owner_kill_count)
    {
        owner->attack_power += 50;
        this->kill_count = owner_kill_count;
    }
}

