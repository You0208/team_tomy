#pragma once
#include "../Lemur/Component/GameObject.h"
#include "../Lemur/Graphics/skinned_mesh.h"
#include "../Lemur/Graphics/Graphics.h"
#include "../Lemur/Resource/ResourceManager.h"
#include"../Lemur/Input/Input.h"
#include "GamePro_ProjectileManager.h"

#include <wrl.h>
#include "../Lemur/Audio/audio.h"

//struct Bullet {
//    bool life; //生存確認
//    DirectX::XMFLOAT3 position; //座標
//    float angle; //角度
//}bullet[100];

class Player :public GameObject
{
public:
    Player(InputComponent* input_,
        PhysicsComponent* physics_,
        GraphicsComponent* graphics_) :GameObject(input_, physics_, graphics_) {}
    
    float ProjectileAtk = 1.0f;
    float AtkSpeed = 1.0f;
    float ProjectileSpeed = 1.0f;
    float ProjectilePerforate = 1.0f;
    float ProjectileSize = 1.0f;

    GamePro_ProjectileManager gamepro_projectilemanager;

    Microsoft::WRL::ComPtr<IXAudio2> xaudio2;
    IXAudio2MasteringVoice* master_voice = nullptr;
    std::unique_ptr<Lemur::Audio::audio> explosion;
};

class PlayerInputComponent :public InputComponent
{
    void Initialize(GameObject* gameobj) override {}
    void Update(GameObject* gameobj, float elapsedTime) override;
public:
    GamePro_ProjectileManager gamepro_projectilemanager;
};

class PlayerPhysicsComponent :public PhysicsComponent
{
    void Initialize(GameObject* gameobj) override;
    void Update(GameObject* gameobj, float elapsedTime) override;
public:
    GamePro_ProjectileManager gamepro_projectilemanager;
};

class PlayerGraphicsComponent :public GraphicsComponent
{
    void Initialize(GameObject* gameobj) override;
    void Update(GameObject* gameobj) override;
    void Render(GameObject* gameobj, float elapsedTime, ID3D11PixelShader* replaced_pixel_shader) override;

private:
    std::shared_ptr<skinned_mesh> PlayerModel;
    std::shared_ptr<skinned_mesh> Player2Model;
    std::shared_ptr<skinned_mesh> BulletModel;
public:
    GamePro_ProjectileManager gamepro_projectilemanager;
};