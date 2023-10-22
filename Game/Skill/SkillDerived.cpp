#include "SkillDerived.h"

#include "Game/Player/Player.h"

void BloodSucking::Update()
{
    // オーナーがダメージを与えていたら
    if(owner->GetAddDamage()>0)
    {
        //回復する
        owner->health += static_cast<int>(static_cast<float>(owner->GetAddDamage()) * 0.1f);

        owner->MaxHealthCheck();
    }
}

void Sprint::Init()
{
    owner->speed_power *= 1.25f;
}

void Acceleration::Init()
{
    max_SP = owner->speed_power * 3.0f;
}

void Acceleration::Update()
{
    // todo 逆転とかの途中で増えるやつ考慮出来てない(max_SPが変わるようにしなきゃいけないかな)
    // 毎フレーム 経過時間(秒)×0.5 を足す
    float elapsed_time = high_resolution_timer::Instance().time_interval();
    float new_SP = owner->speed_power + elapsed_time * 0.5f;

    // 最大値まで行ってなかったら強化する。
    if (new_SP < max_SP)
        owner->speed_power = new_SP;
    return;
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
    heal_timer_ms += high_resolution_timer::Instance().time_interval();
    if (heal_timer_ms >= heal_time_ms)
    {
        owner->health += heal_power;
        heal_timer_ms = 0.0f;
    }

    owner->MaxHealthCheck();
}

void Rest::Fin()
{
    owner->health += static_cast<int>(static_cast<float>(owner->max_health) * 0.3f);
}

void Reverse::Init()
{
    basic_max_health = owner->max_health;
    basic_attack_power = owner->attack_power;
    basic_speed_power = owner->speed_power;
}
void Reverse::Update()
{
    power_up_timer -= high_resolution_timer::Instance().time_interval();

    int owner_kill_count = owner->GetKillCount();

    // キルしたら
    if (kill_count < owner_kill_count)
    {
        // パワーアップ中じゃないなら
        if (power_up_timer <= 0.0f)
        {
            // タイマーのセット
            power_up_timer = 30;

            // 強化前のステータスを保持
            basic_max_health = owner->max_health;
            basic_health = owner->health;
            basic_attack_power = owner->attack_power;
            basic_speed_power = owner->speed_power;
            basic_defense_power = owner->defense_power;

            // 強化
            owner->max_health *= 1.2f;
            owner->health *= 1.2f;
            owner->attack_power *= 1.2f;
            owner->defense_power *= 1.5f;
            owner->speed_power *= 1.5f;

        }
        // パワーアップ中なら延長
        else
        {
            // タイマーの再セット
            power_up_timer = 30;
        }

        kill_count = owner_kill_count;

    }
    else
    {
        if (power_up_timer <= 0)
        {
            owner->max_health = basic_max_health;
            owner->attack_power = basic_attack_power;
            owner->defense_power = basic_defense_power;
            owner->speed_power = basic_speed_power;
        }
    }


}

void SuperMan::Init()
{
    owner->max_health *= 2;
    owner->health *= 2;
    owner->attack_power *= 2.0f;
    owner->defense_power *= 2.0f;
    owner->speed_power *= 2.0f;

}

void Gale::Init()
{
    owner->speed_power *= 1.15f;
}

void Obesity::Init()
{
    owner->max_health *= 1.25f;
    owner->health *= 1.25f;
}
