#include "SkillDerived.h"

#include "Game/Player/Player.h"

void BloodSucking::Update()
{
    // �I�[�i�[���_���[�W��^���Ă�����
    if(owner->GetAddDamage()>0)
    {
        //�񕜂���
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
    if(heal_time>=heal_timer)
    {
        
    }
}
