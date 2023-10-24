#pragma once
#include "Lemur/Scene/BaseScene.h"

class TitleScene :
    public Lemur::Scene::BaseScene
{
public:
    TitleScene() {}
    ~TitleScene() override {}

    // ������
    void Initialize()override;

    // �I����
    void Finalize()override;

    // �X�V����
    void Update(HWND hwnd, float elapsedTime)override;

    // �`�揈��
    void Render(float elapsedTime)override;

private:
    std::unique_ptr<sprite> title;
    
};

