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
private: // �V���O���g����
    Camera() {}
    ~Camera() {}

public:
    // �B��̃C���X�^���X�擾
    static Camera& Instance()
    {
        static Camera camera;
        return camera;
    }

    // �p�[�X�y�N�e�B�u�ݒ�
    void SetPerspectiveFov(Microsoft::WRL::ComPtr<ID3D11DeviceContext> dc);

    // �w�����������
    void SetLookAt(const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& focus, const DirectX::XMFLOAT3& up);

    void Update(float elapsedTime);

private:
    void DrawDebug();
    void UpdateDebug();


public:
    DirectX::XMMATRIX GetViewMatrix() { return V; }         // view�s��擾
    DirectX::XMMATRIX GetProjectionMatrix() { return P; }   // projection�s��擾
    DirectX::XMFLOAT4X4 GetWorld() { return world; }   // projection�s��擾
    DirectX::XMVECTOR GetEye() { return Eye; }   // projection�s��擾

private:
    // view�s��
    DirectX::XMMATRIX V = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
    // projection�s��
    DirectX::XMMATRIX P = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };


    DirectX::XMVECTOR Eye{};
    DirectX::XMVECTOR Focus{};
    DirectX::XMVECTOR Up{};

    DirectX::XMFLOAT3 up{};
    DirectX::XMFLOAT3 front{};
    DirectX::XMFLOAT3 right{};

    DirectX::XMFLOAT4X4 world;
    // �f�o�b�O�p
    DirectX::XMFLOAT3   target = { 0,0,0 };
    DirectX::XMFLOAT3   angle = { 0,0,0 };
    float               rollSpeed = DirectX::XMConvertToRadians(180);
    float               range = 10.0f;

    float               maxAngleX = DirectX::XMConvertToRadians(45);
    float               minAngleX = DirectX::XMConvertToRadians(-45);
};