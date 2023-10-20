#include "BadSkillDerived.h"

#include "high_resolution_timer.h"
#include "Game/Player/Player.h"

void Curse::Update()
{
    damage_timer += high_resolution_timer::Instance().time_interval();
    if (damage_timer >= damage_time)
    {
        owner->health -= damage_value;
        owner->attack_power += static_cast<float>(damage_value) / 10.0f;
        damage_timer = 0.0f;
    }

    // ‚±‚ÌƒXƒLƒ‹‚Å‚ÍŽ€‚È‚È‚¢‚æ‚¤‚É‚·‚é
    if(owner->health<1)
    {
        owner->health = 1;
    }
}

void Arrogance::Update()
{
    timer_ms += high_resolution_timer::Instance().time_interval();
    if (timer_ms >= time_ms)
    {
        owner->attack_power -= value;
        timer_ms = 0.0f;
    }
    if (owner->attack_power < 10)
        owner->attack_power = 10;
}
