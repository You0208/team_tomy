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
    //更新処理
    virtual void Update(float elapsedTime) = 0;
    //描画処理
    virtual void Render(ID3D11DeviceContext* dc, ID3D11PixelShader* replaced_pixel_shader) = 0;
    //デバッグプリミティブ描画
    virtual void DrawDebugPrimitive();
    //位置取得
    const DirectX::XMFLOAT3& GetPosition() const { return position; }
    //方向取得
    const DirectX::XMFLOAT3& GetDirection() const { return direction; }
    //スケール取得
    const DirectX::XMFLOAT3& GetScale() const { return scale; }

protected:
    //行列更新処理
    void UpdateTransform();

protected:
    DirectX::XMFLOAT3 position = { 0,0,0 };
    DirectX::XMFLOAT3 direction = { 0,0,1 };
    DirectX::XMFLOAT3 scale = { 1,1,1 };
    DirectX::XMFLOAT4X4 transform = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
};