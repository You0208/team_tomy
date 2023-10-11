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

void Sprint::Init()
{
    owner->speed_power *= 1.25f;
}

void Patience::Update()
{
    // 死んでまだスキルを使ってなかったら
    if(owner->death&&!is_used)
    {
        owner->death = false;
        owner->health = 1;
        is_used = true;
    }
}

void Regeneration::Update()
{
    if(heal_time>=heal_timer)
    {
        
    }
}
