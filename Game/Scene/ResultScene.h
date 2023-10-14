#pragma once
#include "Lemur/Scene/BaseScene.h"

class ResultScene :
    public Lemur::Scene::BaseScene
{
public:
    ResultScene() {}
    ~ResultScene() override {}

    // 初期化
    void Initialize()override;

    // 終了化
    void Finalize()override;

    // 更新処理
    void Update(HWND hwnd, float elapsedTime)override;

    // 描画処理
    void Render(float elapsedTime)override;


    void DebugImGui();

};
