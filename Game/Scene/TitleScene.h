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

    void DrawImGui();
private:
    std::unique_ptr<sprite> title;
    std::unique_ptr<sprite> spider_anim;
    float anim_timer = 0.0f;
    int tex_pos_x = 0;
    DirectX::XMFLOAT2 spider_pos{};
};

