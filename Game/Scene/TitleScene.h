#pragma once
#include "Lemur/Scene/BaseScene.h"

class TitleScene :
    public Lemur::Scene::BaseScene
{
public:
    TitleScene() {}
    ~TitleScene() override {}

    // 初期化
    void Initialize()override;

    // 終了化
    void Finalize()override;

    // 更新処理
    void Update(HWND hwnd, float elapsedTime)override;

    // 描画処理
    void Render(float elapsedTime)override;

    void DrawImGui();

    int step = 0;
private:
    std::unique_ptr<sprite> title;
    std::unique_ptr<sprite> spider_anim;
    float anim_timer = 0.0f;
    int tex_pos_x = 0;
    DirectX::XMFLOAT2 spider_pos{};
};

