#include "JudgmentDerived.h"
#include "Windows.h"


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

}

bool ClawAttackJudgment::Judgment()
{
    if (GetAsyncKeyState(VK_RETURN) & 0x8000)
        return true;
    else
        return false;
}

