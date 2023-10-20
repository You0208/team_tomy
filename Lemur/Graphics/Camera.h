#pragma once

#include <windows.h>
#include <tchar.h>
#include <sstream>
#include <d3d11.h>
#include <DirectXMath.h>
#include "misc.h"
#include <wrl.h>

#include "Game/Enemy/Enemy.h"

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

    void DrawDebug();

    // HP�Q�[�W�̕`��
    void RenderEnemyHP(sprite* gauge);
private:
    void UpdateDebug();

    // ���b�N�I���؂�ւ�
    void InputLockOn();

    // ���b�N�I�����̃J�����̋���
    void LockOnUpdate(float elapsedTime);

    // ���b�N�I�����ĂȂ����̃J�����̋���
    void NonLockOnUpdate(float elapsedTime);

    
public:
    DirectX::XMMATRIX GetViewMatrix() { return V; }         // view�s��擾
    DirectX::XMMATRIX GetProjectionMatrix() { return P; }   // projection�s��擾
    DirectX::XMFLOAT4X4 GetWorld() { return world; }   // projection�s��擾
    DirectX::XMVECTOR GetEye() { return Eye; }   // projection�s��擾
    DirectX::XMVECTOR GetFocus() { return Focus; }


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

    DirectX::XMFLOAT3 eye = {};

    // �����Ɍ�����Ȃ��Ă�����Əォ����Ղ��邽�߂̕␳
    float eye_y_offset = 4.0f;

    // �������l�̑����݂���Ȃ��ă`���C��
    float target_y_offset = 1.5f;

    DirectX::XMFLOAT4X4 world;
    // �f�o�b�O�p
    DirectX::XMFLOAT3   target = { 0,0,0 };
    DirectX::XMFLOAT3   angle = { 0,0,0 };
    float               rollSpeed = DirectX::XMConvertToRadians(180);
    float               range = 12.0f;

    float               maxAngleX = DirectX::XMConvertToRadians(45);
    float               minAngleX = DirectX::XMConvertToRadians(-45);

    Enemy* lock_on_enemy;

    // ���b�N�I�����Ă邩
    bool is_lockOn = false;

};
