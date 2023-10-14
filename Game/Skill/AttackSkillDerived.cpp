#include "AttackSkillDerived.h"
#include "Game/Player/Player.h"
#include "Game/StateMachine/StateMachine.h"
#include "imgui.h"
void StrongArm::Init()
{
    owner->attack_power *= 1.25f;
}

void StrongArm::Fin()
{

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

void MagicSword::Init()
{
    owner->GetStateMachine()->GetAttackState()->SetMotionValue(Nero::Component::AI::AttackState::second_attack,1.3f);
    owner->GetStateMachine()->GetAttackState()->SetMotionValue(Nero::Component::AI::AttackState::third_attack,1.4f);
}

void Cruel::Update()
{
    // 何撃目か取得
    int attack_step = owner->GetStateMachine()->GetAttackState()->GetAttackStep();

    // 連撃やなかったら効果発動しない
    if (attack_step == Nero::Component::AI::AttackState::first_attack)return;

    // todo これ二撃目と三撃目の増え方別にする？最大値の設定
    // ダメージを与えたら効果発動
    if(owner->GetAddDamage()>0)
    {
        float motion_value = owner->GetStateMachine()->GetAttackState()->GetMotionValue(attack_step);
#if 0
        motion_value *= 1.1f;

        owner->GetStateMachine()->GetAttackState()->SetMotionValue(Nero::Component::AI::AttackState::second_attack, motion_value);
        owner->GetStateMachine()->GetAttackState()->SetMotionValue(Nero::Component::AI::AttackState::third_attack, motion_value);

#else
        switch (attack_step)
        {
        case Nero::Component::AI::AttackState::second_attack:

            motion_value *= 1.1f;
            owner->GetStateMachine()->GetAttackState()->SetMotionValue(Nero::Component::AI::AttackState::second_attack, motion_value);
            break;

        case Nero::Component::AI::AttackState::third_attack:

            motion_value *= 1.2f;
            owner->GetStateMachine()->GetAttackState()->SetMotionValue(Nero::Component::AI::AttackState::third_attack, motion_value);
            break;
        }
#endif
    }
}

void Revenge::DrawImGui()
{
    ImGui::DragInt("old_health", &old_health);
    ImGui::DragFloat("basic_AP", &basic_AP);
    ImGui::DragFloat("power_up_value", &power_up_value);
}

void Revenge::Init()
{
    old_health = owner->health;
    basic_AP = owner->attack_power;
}

void Revenge::Update()
{
    // Todo　まだちょっと不安定(マイナスになったりする。ふつうにプレイしてたら問題はない)、あとこれやったら途中で攻撃力増えても反映されない
    // HPが減ってなかったら攻撃力の計算しない
    if (owner->health == owner->max_health)
        return;
    // HPに変動がなかったら計算しない
    if (old_health == owner->health)
        return;

    // HP減少率 = (最大HP - HP) ÷ 100.0f
    float decrease_rate = (static_cast<float>(owner->max_health) - static_cast<float>(owner->health))/owner->max_health;
    owner->attack_power = basic_AP * (decrease_rate * 5.0f);

    power_up_value = owner->attack_power - basic_AP;

    old_health = owner->health;
}

void HolySword::Init()
{
    owner->GetStateMachine()->GetAttackState()->SetMotionValue(Nero::Component::AI::AttackState::first_attack, 1.5f);
}

void SwordSaint::Init()
{
    owner->attack_power *= 1.15f;
}

