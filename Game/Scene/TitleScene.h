#pragma once
#include "Lemur/Scene/BaseScene.h"

class TitleScene :
    public Lemur::Scene::BaseScene
{
public:
    TitleScene() {}
    ~TitleScene() override {}

    // ‰Šú‰»
    void Initialize()override;

    // I—¹‰»
    void Finalize()override;

    // XVˆ—
    void Update(HWND hwnd, float elapsedTime)override;

    // •`‰æˆ—
    void Render(float elapsedTime)override;

private:
    std::unique_ptr<sprite> title;
};

