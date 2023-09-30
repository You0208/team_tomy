#pragma once
#include "Component.h"
#include "../Graphics/skinned_mesh.h"
#include "../Graphics/Graphics.h"
#include "../Resource/ResourceManager.h"
#include "./Lemur/Input/Mouse.h"
#include <d3d11.h>
#include <wrl.h>
#include <directxmath.h>
#include <vector>
#include <unordered_map>
#include <string>
#include <fbxsdk.h>

class GameObject
{
public:
    GameObject() {};
    GameObject(InputComponent* input_,
        PhysicsComponent* physics_,
        GraphicsComponent* graphics_)
        :input(input_),
        physics(physics_),
        graphics(graphics_)
    {}


    // コンポーネント
    virtual void Initialize()
    {
        input->Initialize(this);
        physics->Initialize(this);
        graphics->Initialize(this);
    }

    virtual void EnemyInitialize(int e_t,int s_t)
    {
        input->Initialize(this);
        physics->EnemyInitialize(this,e_t,s_t);
        graphics->Initialize(this);
    }

    void Update(float elapsedTime)
    {
        input->Update(this, elapsedTime);
        physics->Update(this, elapsedTime);
        graphics->Update(this);
    }

    void Render(float elapsedTime)
    {
        graphics->Render(this,elapsedTime, pixelShader);
    }

    void Delete()
    {
        delete input;
        delete physics;
        delete graphics;
    }

    virtual void DebugImgui(){};
public:
    // 位置取得
    const DirectX::XMFLOAT3& GetPosition() const { return position; }

    // 位置更新
    void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

    // 回転取得
    const DirectX::XMFLOAT3& GetAngle() const { return rotation; }

    // 回転更新
    void SetAngle(const DirectX::XMFLOAT3& rotation) { this->rotation = rotation; }

    // スケール取得
    const DirectX::XMFLOAT3& GetScale() const { return scale; }

    // スケール更新
    void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }

    // 半径取得
    float GetRadius() const { return radius; }

    // 高さ取得
    float GetHeight() const { return height; }

    // 地面に設置しているか
    bool IsGround() const { return isGround; }

    // 健康状態を取得
    int GetHealth() const { return health; }

    // 最大健康状態を取得
    int GetMaxHealth() const { return maxHealth; }

    // ダメージを与える
    bool ApplyDamage(int damage, float invincibleTime);

    // 色取得
    const DirectX::XMFLOAT4& GetColor() { return material_color; }

    // モデル設定
    void SetModel(std::shared_ptr<skinned_mesh> Model) { this->Model = Model; }

    // モデル取得(AI側でアニメーションの切り替えしたいから作りました Byトミー)
    skinned_mesh* GetModel()const { return Model.get(); }

    // 描画設定
    void Render(float elapsedTime, ID3D11PixelShader* replaced_pixel_shader);

    // 移動処理
    void Move(float vx, float vz, float speed);

protected:


    // ジャンプ処理
    void Jump(float speed);

    // 無敵時間更新
    void UpdateInvincibleTimer(float elapsedTime);

    // 着地した時に呼ばれる
    virtual void OnLanding() {};

    // ダメージを受けたときに呼ばれる
    virtual void OnDamaged() {}

    // 死亡した時に呼ばれる
    virtual void OnDead() {}

private:

    // 垂直速力更新処理
    void UpdataVerticalVelocity(float elapsedFrame);

    // 垂直移動更新処理
    void UpdateVerticalMove(float elapsedTime);

    // 水平速力更新処理
    void UpdataHorizontalVelocity(float elapsedFrame);

    // 水平移動更新処理
    void UpdateHorizontalMove(float elapsedTime);

protected:
    DirectX::XMFLOAT4X4 World;

    DirectX::XMFLOAT3 velocity          ={ 0, 0, 0 };  // 速度
    DirectX::XMFLOAT3 position          ={ 0, 0, 0 };  // 位置
    DirectX::XMFLOAT3 scale             ={ 3.0f, 3.0f, 3.0f }; // スケール
    DirectX::XMFLOAT3 rotation          ={ 0, 0, 0 }; //　回転
    DirectX::XMFLOAT4 material_color    ={ 1, 1, 1, 1 }; // 色

    DirectX::XMFLOAT3 direction         = { 0,0,1 };// 方向
    DirectX::XMFLOAT4X4 transform       = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };// 行列

    float   invincibleTimer             = 1.0f; // 無敵時間
    float   maxMoveSpeed                = 5.0f; // 最大速度
    float   moveVecX                    = 0.0f; // X方向移動
    float   moveVecZ                    = 0.0f; // Z方向移動
    float   friction                    = 0.5f; // 摩擦力
    float   acceleration                = 1.0f; // 加速力
    float   gravity                     = -1.0f; // 重力
    float   radius                      = 1.0f; // 半径
    float   height                      = 0.0f; // 高さ 

    float	airControl                  = 0.3f; // 空気抵抗

    bool    isGround                    = false; // 着地フラグ
    int     health                      = 5; // 健康状態
    int     maxHealth                   = 5; // 最大健康状態

    ID3D11PixelShader* pixelShader      = nullptr; // シェーダー

    float animation_tick                = 0; // アニメーション
    std::shared_ptr<skinned_mesh> Model = nullptr;
    // アニメーション用(メンバ化しときました。これに相当する変数とかもしあったらごめん)
    int clip_index = 0;
    int frame_index = 0;


    Mouse* mouse;
private:
    InputComponent* input;
    PhysicsComponent* physics;
    GraphicsComponent* graphics;
};