#include "StateMachine.h"
#include "imgui.h"
namespace Nero::Component::AI
{

    void StateMachine::Update()
    {
        _ASSERT_EXPR(current_state, L"���݂̃X�e�[�g���Z�b�g����Ă܂���Bfirst_state�ݒ肵���H");
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
        _ASSERT_EXPR(false, L"�U���X�e�[�g���Ȃ��H");
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
            _ASSERT_EXPR(false, L"�X�e�[�g�z�񂪈�����Ȃ��ł��B����ւ�����B");

        if (next_state_ > states.size() - 1)
            _ASSERT_EXPR(false, L"�X�e�[�g�z����I�[�o�[�����B����ւ�����B");

        next_state = states.at(next_state_).get();
    }
}
