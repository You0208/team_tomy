#pragma once
#include "Lemur/Scene/BaseScene.h"

class ResultScene :
    public Lemur::Scene::BaseScene
{
public:
    ResultScene() {}
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

};
