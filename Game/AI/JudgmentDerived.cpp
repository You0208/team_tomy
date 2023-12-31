#include "JudgmentDerived.h"
#include "Windows.h"
#include "Game/MathHelper.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Manager/CharacterManager.h"

bool WanderJudgment::Judgment()
{
    // 徘徊しに行く位置に到達してなかったら徘徊しに行く
    if (!owner->DistanceJudge(owner->GetPosition(), owner->GetTargetPosition(), 1.0f))
    {
        return true;
    }

    else return false;
}

bool BattleJudgment::Judgment()
{
    // 交戦状態ならtrue
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
    // 限りなく正面に近い角度なら軸合わせしない。
    if (owner->SearchPlayer(FLT_MAX, 0.995f))
    {
        // 一回軸合わせしなかったら次の行動からは軸合わせできる。
        owner->turned = false;
        return false;
    }

    // すでに軸合わせをしてたらfalse(永遠に軸合わせしないために)
    if (owner->turned == true)
    {
        // 一回軸合わせしなかったら次の行動からは軸合わせできる。
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
    if (owner->fear_frag)
    {
        owner->fear_frag = false;
        return true;
    }
    else return false;
}

bool ClawAttackJudgment::Judgment()
{
    if (GetAsyncKeyState(VK_RETURN) & 0x8000)
        return true;
    else
        return false;
}

