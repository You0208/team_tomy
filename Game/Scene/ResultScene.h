#pragma once
#include "Game/Player/Player.h"
#include "Lemur/Scene/BaseScene.h"


class ResultScene :
    public Lemur::Scene::BaseScene
{
public:
    // �V�[���؂�ւ����ɑ������ɃN�G�X�g���N���A��������ݒ肷��B
    ResultScene(bool clear_) :clear(clear_){}
    ~ResultScene() override {}

    // ������
    void Initialize()override;

    // �I����
    void Finalize()override;

    // �X�V����
    void Update(HWND hwnd, float elapsedTime)override;

    // �`�揈��
    void Render(float elapsedTime)override;

    void DebugImGui();

    // ��V���v���C���[�ɗ^����
    void GiveReward();
private:

    enum SceneStep
    {
        Result,// ���U���g���
        Status,// �X�e�[�^�X�U�蕪�����
    };
    // �V�[���i�s�x
    int step = Result;

    Player* player;

    // �N���A������
    bool clear;

    // for���ŕ`��Ƃ�����Ƃ�����������player�̊e�p�����[�^��z��
    float player_status[3];
    // ��V�󂯎��O�̎��_�ł̃v���C���[�p�����[�^
    float player_status_min[3];

private:/*--------- �X�v���C�g�֌W --------*/

    /*------------ �X�v���C�g�f�[�^ ----------*/
    // �N���A���̔w�i
    // ���s���̔w�i

    // �������̔w�i
    std::shared_ptr<sprite> spr_back_win;
    // �s�k���̔w�i
    std::shared_ptr<sprite> spr_back_lose;
    // �X�e�[�^�X�U�蕪�����̔w�i
    std::shared_ptr<sprite> spr_back_status;
    // ���
    std::shared_ptr<sprite> spr_small_arrow;
    // ����
    std::shared_ptr<sprite> spr_number;
    // �p�����[�^�̍��z�c�݂����Ȃ��
    std::shared_ptr<sprite> spr_betbox;
    // �ǂꂾ���x�b�g���Ă��������p�̃R�C��
    std::shared_ptr<sprite> spr_coin;
    // OK�{�^��
    std::shared_ptr<sprite> spr_OK;


    /*------------- �X�v���C�g���W -------------*/

    DirectX::XMFLOAT2 select_decision_pos;

    // ��������
    DirectX::XMFLOAT2 small_arrow_up_pos[3];
    //���������
    DirectX::XMFLOAT2 small_arrow_down_pos[3];
    DirectX::XMFLOAT2 char_bet_pos[3];

    DirectX::XMFLOAT2 num_bet_pos[3];

    DirectX::XMFLOAT2 coin_bet_pos[3];

    // �p�����[�^�̍��z�c�݂����Ȃ�̈ʒu
    DirectX::XMFLOAT2 bet_boxpos[3];
    // �p�����[�^�̍��z�c�݂����Ȃ�̃T�C�Y
    DirectX::XMFLOAT2 bet_boxsize = { 400,200 };


};
