#include "JudgmentDerived.h"
#include "Windows.h"
#include "Game/MathHelper.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Manager/CharacterManager.h"

bool WanderJudgment::Judgment()
{
    // �p�j���ɍs���ʒu�ɓ��B���ĂȂ�������p�j���ɍs��
    if (!owner->ReachTargetJudge(owner->GetPosition(), owner->GetTargetPosition(), 1.0f))
    {
        return true;
    }

    else return false;
}

bool BattleJudgment::Judgment()
{
    if(owner->SearchPlayer(owner->GetVisionLength()))
    {
        return true;
    }

    return false;
}

bool DeathJudgment::Judgment()
{
    if (owner->death)
        return true;
    return false;
}

bool AttackJudgment::Judgment()
{
    DirectX::XMFLOAT3 player_pos = CharacterManager::Instance().GetPlayer()->GetPosition();
    return owner->ReachTargetJudge(owner->GetPosition(), player_pos, owner->GetNearAttackRange());
}

bool ClawAttackJudgment::Judgment()
{
    if (GetAsyncKeyState(VK_RETURN) & 0x8000)
        return true;
    else
        return false;
}

