#include "SkillDerived.h"

#include "Game/Player/Player.h"

void BloodSucking::Update()
{
    // �I�[�i�[���_���[�W��^���Ă�����
    if(owner->GetAddDamage()>0)
    {
        //�񕜂���
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
    // todo �t�]�Ƃ��̓r���ő������l���o���ĂȂ�(max_SP���ς��悤�ɂ��Ȃ��Ⴂ���Ȃ�����)
    // ���t���[�� �o�ߎ���(�b)�~0.5 �𑫂�
    float elapsed_time = high_resolution_timer::Instance().time_interval();
    float new_SP = owner->speed_power + elapsed_time * 0.5f;

    // �ő�l�܂ōs���ĂȂ������狭������B
    if (new_SP < max_SP)
        owner->speed_power = new_SP;
    return;
}

void Patience::Update()
{
    // ����ł܂��X�L�����g���ĂȂ�������
    if(owner->death&&!is_used)
    {
        owner->death = false;
        owner->health = 1;
        is_used = true;
    }
}

void Regeneration::Update()
{
    heal_timer += high_resolution_timer::Instance().time_interval();
    if (heal_timer >= heal_time)
    {
        owner->health += heal_power;
        heal_timer = 0.0f;
    }

    owner->MaxHealthCheck();
}

void Reverse::Update()
{
    power_up_timer -= high_resolution_timer::Instance().time_interval();

    int owner_kill_count = owner->GetKillCount();

    // �L��������
    if (kill_count < owner_kill_count)
    {
        // �p���[�A�b�v������Ȃ��Ȃ�
        if (power_up_timer <= 0.0f)
        {
            // �^�C�}�[�̃Z�b�g
            power_up_timer = 30;

            // �����O�̃X�e�[�^�X��ێ�
            basic_max_health = owner->max_health;
            basic_health = owner->health;
            basic_attack_power = owner->attack_power;
            basic_speed_power = owner->speed_power;
            basic_defense_power = owner->defense_power;

            // ����
            owner->max_health *= 1.2f;
            owner->health *= 1.2f;
            owner->attack_power *= 1.2f;
            owner->defense_power *= 1.5f;
            owner->speed_power *= 1.5f;

        }
        // �p���[�A�b�v���Ȃ牄��
        else
        {
            // �^�C�}�[�̍ăZ�b�g
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
