#pragma once
#include "Lemur/Scene/BaseScene.h"
#include <thread>


class LoadingScene :
    public Lemur::Scene::BaseScene
{
public:
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
    std::unique_ptr<sprite> title;


    DirectX::XMFLOAT4 camera_position{ 0.0f, 0.0f, -10.0f, 1.0f };
    DirectX::XMFLOAT3 translation{ 0, 0, 0 };
    DirectX::XMFLOAT3 scaling{ 1, 1, 1 };
    DirectX::XMFLOAT3 rotation{ 0, 3.567f, 0 };
 //----------------------------------------------------
 //  シェーダー関連
 //----------------------------------------------------
 
    Microsoft::WRL::ComPtr<ID3D11Buffer> constant_buffers;

    struct scene_constants
    {
        DirectX::XMFLOAT4X4 view_projection;
        DirectX::XMFLOAT4 light_direction;
        DirectX::XMFLOAT4 camera_position;
    };
    scene_constants scene_constants;

};

