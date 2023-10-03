#include "JudgmentDerived.h"
#include "Windows.h"
#include "Game/MathHelper.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Manager/CharacterManager.h"

bool WanderJudgment::Judgment()
{
    // 徘徊しに行く位置に到達してなかったら徘徊しに行く
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
    if (owner->health <= 0)
        return true;
    return false;
}

bool ClawAttackJudgment::Judgment()
{
    if (GetAsyncKeyState(VK_RETURN) & 0x8000)
        return true;
    else
        return false;
}

