#include "AttackSkillDerived.h"
#include "Game/Player/Player.h"
#include "Game/StateMachine/StateMachine.h"
#include "imgui.h"
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

void MagicSword::Init()
{
    owner->GetStateMachine()->GetAttackState()->SetMotionValue(Nero::Component::AI::AttackState::second_attack,1.3f);
    owner->GetStateMachine()->GetAttackState()->SetMotionValue(Nero::Component::AI::AttackState::third_attack,1.4f);
}

void Cruel::Update()
{
    // �����ڂ��擾
    int attack_step = owner->GetStateMachine()->GetAttackState()->GetAttackStep();

    // �A����Ȃ���������ʔ������Ȃ�
    if (attack_step == Nero::Component::AI::AttackState::first_attack)return;

    // todo ����񌂖ڂƎO���ڂ̑������ʂɂ���H�ő�l�̐ݒ�
    // �_���[�W��^��������ʔ���
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
    // Todo�@�܂�������ƕs����(�}�C�i�X�ɂȂ����肷��B�ӂ��Ƀv���C���Ă�����͂Ȃ�)�A���Ƃ���������r���ōU���͑����Ă����f����Ȃ�
    // HP�������ĂȂ�������U���͂̌v�Z���Ȃ�
    if (owner->health == owner->max_health)
        return;
    // HP�ɕϓ����Ȃ�������v�Z���Ȃ�
    if (old_health == owner->health)
        return;

    // HP������ = (�ő�HP - HP) �� 100.0f
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

