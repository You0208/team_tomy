#pragma once
#include <memory>
#include "Lemur/Model/Model.h"
class Stage
{
public:
    Stage() {}
    ~Stage() {}

    // ToDo レイキャスト
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
};
