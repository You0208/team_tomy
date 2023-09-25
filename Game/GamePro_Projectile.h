#pragma once

#include "../Lemur/Component/GameObject.h"
#include "../Lemur/Component/Component.h"
#include "../Lemur/Graphics/skinned_mesh.h"
#include "../Lemur/Graphics/Graphics.h"
#include "../Lemur/Resource/ResourceManager.h"
#include"../Lemur/Input/Input.h"

class GamePro_Projectile
{
public:
    GamePro_Projectile() {}
    virtual ~GamePro_Projectile(){}
    //�X�V����
    virtual void Update(float elapsedTime) = 0;
    //�`�揈��
    virtual void Render(ID3D11DeviceContext* dc, ID3D11PixelShader* replaced_pixel_shader) = 0;
    //�f�o�b�O�v���~�e�B�u�`��
    virtual void DrawDebugPrimitive();
    //�ʒu�擾
    const DirectX::XMFLOAT3& GetPosition() const { return position; }
    //�����擾
    const DirectX::XMFLOAT3& GetDirection() const { return direction; }
    //�X�P�[���擾
    const DirectX::XMFLOAT3& GetScale() const { return scale; }

protected:
    //�s��X�V����
    void UpdateTransform();

protected:
    DirectX::XMFLOAT3 position = { 0,0,0 };
    DirectX::XMFLOAT3 direction = { 0,0,1 };
    DirectX::XMFLOAT3 scale = { 1,1,1 };
    DirectX::XMFLOAT4X4 transform = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
};