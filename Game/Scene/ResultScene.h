#pragma once
#include "Game/Player/Player.h"
#include "Lemur/Scene/BaseScene.h"


class ResultScene :
    public Lemur::Scene::BaseScene
{
public:
    // �V�[���؂�ւ����ɑ������Ƀx�b�g�̃��[�g�A
    //�������ɃN�G�X�g���N���A��������ݒ肷��B
    ResultScene(float bet_rate_ ,bool clear_) :bet_rate(bet_rate_),clear(clear_){}
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
    Player* player;

    float bet_rate;

    // �N���A������
    bool clear;
};
