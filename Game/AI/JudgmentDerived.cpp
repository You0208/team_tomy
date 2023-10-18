#include "JudgmentDerived.h"
#include "Windows.h"
#include "Game/MathHelper.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Manager/CharacterManager.h"

bool WanderJudgment::Judgment()
{
    // �p�j���ɍs���ʒu�ɓ��B���ĂȂ�������p�j���ɍs��
    if (!owner->DistanceJudge(owner->GetPosition(), owner->GetTargetPosition(), 1.0f))
    {
        return true;
    }

    else return false;
}

bool BattleJudgment::Judgment()
{
    // ����ԂȂ�true
    if (owner->belligerency)return true;

    if(owner->SearchPlayer(owner->GetVisionLength()))
    {
        owner->belligerency = true;
        return true;
    }

    return false;
}

bool TurnJudgment::Judgment()
{
    // ����Ȃ����ʂɋ߂��p�x�Ȃ玲���킹���Ȃ��B
    if (owner->SearchPlayer(FLT_MAX, 0.995f))
    {
        // ��񎲍��킹���Ȃ������玟�̍s������͎����킹�ł���B
        owner->turned = false;
        return false;
    }

    // ���łɎ����킹�����Ă���false(�i���Ɏ����킹���Ȃ����߂�)
    if (owner->turned == true)
    {
        // ��񎲍��킹���Ȃ������玟�̍s������͎����킹�ł���B
        owner->turned = false;
        return false;
    }

    owner->turned = true;
    return true;

}

bool DeathJudgment::Judgment()
{
    if (owner->health <= 0)
        owner->death = true;

    if (owner->death)
        return true;
    
    return false;
}

bool MiddleJudgment::Judgment()
{
    DirectX::XMFLOAT3 player_pos = CharacterManager::Instance().GetPlayer()->GetPosition();
    return owner->DistanceJudge(owner->GetPosition(), player_pos, owner->GetMiddleAttackRange());

}

bool NearJudgment::Judgment()
{
    DirectX::XMFLOAT3 player_pos = CharacterManager::Instance().GetPlayer()->GetPosition();
    return owner->DistanceJudge(owner->GetPosition(), player_pos, owner->GetNearAttackRange());
}

bool FearJudgment::Judgment()
{
    return owner->fear_frag;
}

bool ClawAttackJudgment::Judgment()
{
    if (GetAsyncKeyState(VK_RETURN) & 0x8000)
        return true;
    else
        return false;
}

