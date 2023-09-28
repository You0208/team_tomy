#pragma once

#include <windows.h>
#include <tchar.h>
#include <sstream>
#include <d3d11.h>
#include <DirectXMath.h>
#include "misc.h"
#include <wrl.h>

class Camera
{
private: // シングルトン化
    Camera() {}
    ~Camera() {}

public:
    // 唯一のインスタンス取得
    static Camera& Instance()
    {
        static Camera camera;
        return camera;
    }

    // パースペクティブ設定
    void SetPerspectiveFov(Microsoft::WRL::ComPtr<ID3D11DeviceContext> dc);

    // 指定方向を向く
    void SetLookAt(const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& focus, const DirectX::XMFLOAT3& up);

    void Update(float elapsedTime);

private:
    void DrawDebug();
    void UpdateDebug();


public:
    DirectX::XMMATRIX GetViewMatrix() { return V; }         // view行列取得
    DirectX::XMMATRIX GetProjectionMatrix() { return P; }   // projection行列取得
    DirectX::XMFLOAT4X4 GetWorld() { return world; }   // projection行列取得
    DirectX::XMVECTOR GetEye() { return Eye; }   // projection行列取得

private:
    // view行列
    DirectX::XMMATRIX V = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
    // projection行列
    DirectX::XMMATRIX P = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };


    DirectX::XMVECTOR Eye{};
    DirectX::XMVECTOR Focus{};
    DirectX::XMVECTOR Up{};

    DirectX::XMFLOAT3 up{};
    DirectX::XMFLOAT3 front{};
    DirectX::XMFLOAT3 right{};

    DirectX::XMFLOAT4X4 world;
    // デバッグ用
    DirectX::XMFLOAT3   target = { 0,0,0 };
    DirectX::XMFLOAT3   angle = { 0,0,0 };
    float               rollSpeed = DirectX::XMConvertToRadians(180);
    float               range = 50.0f;

    float               maxAngleX = DirectX::XMConvertToRadians(45);
    float               minAngleX = DirectX::XMConvertToRadians(-45);
};