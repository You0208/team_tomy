#include "AttackSkillDerived.h"
#include "Game/Player/Player.h"

void StrongArm::Init()
{
    owner->attack_power *= 1.25f;
}

