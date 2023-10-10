// ReSharper disable All
#pragma once
#include <string>

class Player;

namespace Nero::Component::AI
{
    /* �X�e�[�g�}�V���Ŏg���������邽�߂̊��N���X
     * �������p�����āA�ق�������A��Ԃ������ō��B
     * ������X�e�[�g�}�V�����g���ē��ڂ���B
     * �쐬���� owner �� state_name ��ݒ肷��B
     */
    class StateBase
    {
    public:
        // todo �e���v���[�g��
        // �������Ɉ�����owner���Z�b�g
        StateBase(Player* player, std::string state_name_)
            :owner(player), state_name(state_name_) {};

        ~StateBase() = default;

        virtual void DrawImGui(){}
        virtual void Begin() = 0;// �X�e�[�g���n�܂�Ƃ�
        virtual void Update() = 0;// �X�e�[�g���A��ɌĂ΂��
        virtual void End() = 0;// �X�e�[�g���I������Ƃ�

        // �X�e�[�g��
        std::string state_name;

        //����X�e�[�g�Ɉڍs���锻��֐�(�߂�ǂ�����֐�������)
        // ����������ɏ����̂����Ԃ���������.�B
        void ChangeJudgeAvoidState();

        // ���S�X�e�[�g�Ɉڍs���锻��֐�(������߂�ǂ�����֐���)
        void ChangeJudgeDeathState();

    protected:
        // todo �e���v���[�g��
        // �X�e�[�g�A�N�V��������l
        Player* owner;

        // �X�e�[�g�̒i�K
        int step = 0;
    };

}
