#include "JudgmentDerived.h"
#include "Windows.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Manager/CharacterManager.h"


bool WanderJudgment::Judgment()
{
    if(GetAsyncKeyState(VK_SHIFT)&0x8000)
    {
        return true;
    }
    else return false;
}

bool BattleJudgment::Judgment()
{
    DirectX::XMFLOAT3 player_pos = CharacterManager::Instance().GetPlayer()->GetPosition();
    DirectX::XMFLOAT3 enemy_pos = owner->GetPosition();
    return false;
}

bool ClawAttackJudgment::Judgment()
{
    if (GetAsyncKeyState(VK_RETURN) & 0x8000)
        return true;
    else
        return false;
}

