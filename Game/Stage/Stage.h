#pragma once
#include <memory>
#include "Lemur/Model/Model.h"
class Stage
{
public:
    Stage() {}
    ~Stage() {}

    void Init();

    void Render();

    void DrawImGui();

    DirectX::XMFLOAT4X4 world;
private:

    DirectX::XMFLOAT4X4 World;
    DirectX::XMFLOAT4 material_color = { 1, 1, 1, 1 }; // 色


    DirectX::XMFLOAT3 position = { 0, 0, 0 };  // 位置
    DirectX::XMFLOAT3 scale = { 1.0f, 1.0f, 1.0f }; // スケール
    DirectX::XMFLOAT3 rotation = { 0, 0, 0 }; //　回転
    float scale_factor = 0.01f;

    std::unique_ptr<skinned_mesh> model;
    std::unique_ptr<skinned_mesh> ceiling_model;
    std::unique_ptr<skinned_mesh> wall_model;
    std::unique_ptr<skinned_mesh> floor_model;
    std::unique_ptr<skinned_mesh> gate_model;

    Microsoft::WRL::ComPtr<ID3D11PixelShader> stage_1;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> gate;
};
