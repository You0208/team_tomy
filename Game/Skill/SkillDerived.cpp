#include "SkillDerived.h"

#include "Game/Player/Player.h"

void BloodSucking::Update()
{
    // オーナーがダメージを与えていたら
    if(owner->GetAddDamage()>0)
    {
        //回復する
        owner->health += static_cast<int>(static_cast<float>(owner->GetAddDamage()) * 0.1f);

        owner->ResetAddDamage();
    }
}
