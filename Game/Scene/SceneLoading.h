#pragma once
#include "Lemur/Scene/BaseScene.h"
#include <thread>


class LoadingScene :
    public Lemur::Scene::BaseScene
{
public:
    LoadingScene(Lemur::Scene::BaseScene* next_scene) :nextScene(next_scene){}
    LoadingScene() {}
    ~LoadingScene() override {}

    // 初期化
    void Initialize()override;

    // 終了化
    void Finalize()override;

    // 更新処理
    void Update(HWND hwnd, float elapsedTime)override;

    // 描画処理
    void Render(float elapsedTime)override;

private:
    // ローディングスレッド
    static void LoadingThread(LoadingScene* scene);

private:
    BaseScene* nextScene = nullptr;
    std::thread* thread = nullptr;
    std::unique_ptr<sprite> spr_icon;
    std::unique_ptr<sprite> spr_back;

    DirectX::XMFLOAT2 poo;

    float time;
};

