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
