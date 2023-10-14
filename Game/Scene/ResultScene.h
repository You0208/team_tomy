#pragma once
#include "Lemur/Scene/BaseScene.h"

class ResultScene :
    public Lemur::Scene::BaseScene
{
public:
    ResultScene() {}
    ~ResultScene() override {}

    // ‰Šú‰»
    void Initialize()override;

    // I—¹‰»
    void Finalize()override;

    // XVˆ—
    void Update(HWND hwnd, float elapsedTime)override;

    // •`‰æˆ—
    void Render(float elapsedTime)override;


    void DebugImGui();

};
