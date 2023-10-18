#include "JudgmentDerived.h"
#include "Windows.h"
#include "Game/MathHelper.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Manager/CharacterManager.h"

bool WanderJudgment::Judgment()
{
    // œpœj‚µ‚És‚­ˆÊ’u‚É“ž’B‚µ‚Ä‚È‚©‚Á‚½‚çœpœj‚µ‚És‚­
    if (!owner->DistanceJudge(owner->GetPosition(), owner->GetTargetPosition(), 1.0f))
    {
        return true;
    }

    else return false;
}

bool BattleJudgment::Judgment()
{
    // Œðíó‘Ô‚È‚çtrue
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
    // ŒÀ‚è‚È‚­³–Ê‚É‹ß‚¢Šp“x‚È‚çŽ²‡‚í‚¹‚µ‚È‚¢B
    if (owner->SearchPlayer(FLT_MAX, 0.995f))
    {
        // ˆê‰ñŽ²‡‚í‚¹‚µ‚È‚©‚Á‚½‚çŽŸ‚Ìs“®‚©‚ç‚ÍŽ²‡‚í‚¹‚Å‚«‚éB
        owner->turned = false;
        return false;
    }

    // ‚·‚Å‚ÉŽ²‡‚í‚¹‚ð‚µ‚Ä‚½‚çfalse(‰i‰“‚ÉŽ²‡‚í‚¹‚µ‚È‚¢‚½‚ß‚É)
    if (owner->turned == true)
    {
        // ˆê‰ñŽ²‡‚í‚¹‚µ‚È‚©‚Á‚½‚çŽŸ‚Ìs“®‚©‚ç‚ÍŽ²‡‚í‚¹‚Å‚«‚éB
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

