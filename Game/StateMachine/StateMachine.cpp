#include "StateMachine.h"
#include "imgui.h"
namespace Nero::Component::AI
{

    void StateMachine::Update()
    {
        _ASSERT_EXPR(current_state, L"現在のステートがセットされてません。first_state設定した？");
        ChangeState();
        current_state->Update();
    }

    void StateMachine::ImGui()
    {
        if(ImGui::TreeNode("StateMachine"))
        {
            ImGui::Text(current_state->state_name.c_str());
            ImGui::TreePop();
        }
    }


    AttackState* StateMachine::GetAttackState() 
    {
        for (auto& state : states) {
            if (typeid((*state)) == typeid(AttackState))
            {
                AttackState* attack_state = dynamic_cast<AttackState*>(state.get());
                return attack_state;
            }
        }
        _ASSERT_EXPR(false, L"攻撃ステートがない？");
        return nullptr;
    }


    void StateMachine::ChangeState()
    {
        if (!next_state)return;

        if (current_state)
            current_state->End();

        current_state = next_state;
        current_state->Begin();
        next_state = nullptr;
    }

    void StateMachine::SetNextState(int next_state_)
    {
        if (states.size() < 2)
            _ASSERT_EXPR(false, L"ステート配列が一個しかないです。入れ替えられん。");

        if (next_state_ > states.size() - 1)
            _ASSERT_EXPR(false, L"ステート配列をオーバーした。入れ替えられん。");

        next_state = states.at(next_state_).get();
    }
}
