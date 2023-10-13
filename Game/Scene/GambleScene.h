#pragma once
#include "Lemur/Scene/BaseScene.h"

// �X�L���z�z�A�G�N�G�X�g���A�X�e�[�^�X������
//������V�[��
class GambleScene :
    public Lemur::Scene::BaseScene
{
public:
    GambleScene() {}
    ~GambleScene() override {}

    // ������
    void Initialize()override;

    // �I����
    void Finalize()override;

    // �X�V����
    void Update(HWND hwnd, float elapsedTime)override;

    // �`�揈��
    void Render(float elapsedTime)override;


    enum Phase
    {
        Skill,
        Quest,
        Gamble,
    };
    int step;

private:


};

