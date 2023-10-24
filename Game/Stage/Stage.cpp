#include "Stage.h"
#include "imgui.h"
#include "Lemur/Graphics/shader.h"
#include "Lemur/Graphics/Graphics.h"

void Stage::Init()
{
    ceiling_model = std::make_unique<skinned_mesh>(Lemur::Graphics::Graphics::Instance().GetDevice(), "./resources/Stage/ceiling.fbx");
    wall_model = std::make_unique<skinned_mesh>(Lemur::Graphics::Graphics::Instance().GetDevice(), "./resources/Stage/wall.fbx");
    floor_model = std::make_unique<skinned_mesh>(Lemur::Graphics::Graphics::Instance().GetDevice(), "./resources/Stage/floor.fbx");
    gate_model = std::make_unique<skinned_mesh>(Lemur::Graphics::Graphics::Instance().GetDevice(), "./resources/Stage/gate.fbx");

    Lemur::Graphics::Graphics& graphics = Lemur::Graphics::Graphics::Instance();
    create_ps_from_cso(graphics.GetDevice(), "./Shader/stage_ps.cso", stage_1.GetAddressOf());
    create_ps_from_cso(graphics.GetDevice(), "./Shader/gate_ps.cso", gate.GetAddressOf());
}

void Stage::Render()
{
    Lemur::Graphics::Graphics& graphics = Lemur::Graphics::Graphics::Instance();

    ID3D11DeviceContext* immediate_context = graphics.GetDeviceContext();

    // 左手系・Y 軸アップへ変換
    const DirectX::XMFLOAT4X4 coordinate_system_transforms[]{
        { -1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 },	// 0:RHS Y-UP
        { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 },		// 1:LHS Y-UP
        { -1, 0, 0, 0, 0, 0, -1, 0, 0, 1, 0, 0, 0, 0, 0, 1 },	// 2:RHS Z-UP
        { 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1 },		// 3:LHS Z-UP
    };
    DirectX::XMMATRIX C{ DirectX::XMLoadFloat4x4(&coordinate_system_transforms[0]) };
    DirectX::XMMATRIX S{ DirectX::XMMatrixScaling(scale.x* scale_factor, scale.y* scale_factor, scale.z* scale_factor) };
    DirectX::XMMATRIX R{ DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z) };
    DirectX::XMMATRIX T{ DirectX::XMMatrixTranslation(position.x, position.y, position.z) };
    // ワールド変換行列を作成
    DirectX::XMStoreFloat4x4(&world, C* S * R * T);

    ceiling_model->render(immediate_context, world, material_color, nullptr, stage_1.Get());
    wall_model->render(immediate_context, world, material_color, nullptr, stage_1.Get());
    floor_model->render(immediate_context, world, material_color, nullptr, stage_1.Get());
    gate_model->render(immediate_context, world, material_color, nullptr, gate.Get());
}

void Stage::DrawImGui()
{
    ImGui::Begin("Stage");
    ImGui::DragFloat3("position", &position.x);
    ImGui::DragFloat("scale_factor", &scale_factor);
    ImGui::End();
}

