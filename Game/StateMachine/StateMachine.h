// ReSharper disable All
#pragma once
#include <vector>
#include <memory>
#include "Game/Player/Player.h"
#include "Game/StateMachine/StateBase.h"


namespace Nero::Component::AI
{
    /* ��Ԃ̕ω����Ǘ�����R���|�[�l���g�B
     * ���������������Ə�Ԃ���ʂ̏�ԂɑJ�ڂ���
     * 
     *  �g����:
     * �e����A�X�e�[�g(���)�� StateDerived.h �Ŏ����ō��B
     * �����ŏ�Ԃ����Ԃ̐؂�ւ��Ȃǂ��s���B
     * �g�p����l��Init�֐��� SetUpState�֐� ���g����
     * ������X�e�[�g���Z�b�g�ł���B
     * ���̎��ɕK��first_state��ݒ肷�邱�ƁB
     */
    class StateMachine
    {
    public:
        StateMachine(){}
        ~StateMachine() = default;

        void Update();
        void ImGui();

        //���̃X�e�[�g���Z�b�g
        void SetNextState(int next_state_);

        // �X�e�[�g�̐����ƃZ�b�g
        template<class State>
        void SetUpState(Player* player, bool is_first_state = false)// todo ����I�z���ndex���ߕ����ꂢ����Ȃ��������Ĕ[��������APlayer�Ƃ���Enum����������Ȃ���
            // ���������N���X�Ƃ��l�[���X�y�[�X���ׂ��H�}�b�v�z��͂����Ȃ邩������ւ�
        {

            // todo ���ꃁ�������[�N�N����H
            State* state = new State(player);
            states.emplace_back(state);

            // todo �����ōŏ��̃X�e�[�g��ݒ�ł���悤�ɂ������B���̓f�t�H���g�����őË�
            // ���߂̃X�e�[�g�Ȃ�Z�b�g����
            if (is_first_state)
            {
                current_state = state;
                current_state->Begin();
            }

        }

        StateBase* GetCurrentState()const { return current_state; }

        float GetMotionValue();
    private:
        // �X�e�[�g��؂�ւ���֐�
        //���̃X�e�[�g���Z�b�g���ꂽ�珟��Ɏ��s�����
        void ChangeState();

    private:

        std::vector<std::unique_ptr<StateBase>>states;

        // ���݂̃X�e�[�g
        StateBase* current_state = nullptr;

        // ���̃X�e�[�g(�Z�b�g���ꂽ�瑦����ւ��)
        StateBase* next_state = nullptr;
    };

}
