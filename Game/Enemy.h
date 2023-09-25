#pragma once

#include "../Lemur/Component/GameObject.h"
#include "../Lemur/Graphics/skinned_mesh.h"
#include "../Lemur/Graphics/Graphics.h"
#include "../Lemur/Resource/ResourceManager.h"
#include"./Lemur/Effekseer/EffekseerManager.h"


// Effect
#include"./Lemur/Effekseer/Effect.h"

#include <wrl.h>
#include "../Lemur/Audio/audio.h"

#include <random>

class Enemy :public GameObject
{
public:
    Enemy(InputComponent* input_,
        PhysicsComponent* physics_,
        GraphicsComponent* graphics_) :GameObject(input_, physics_, graphics_) {}
    float Speed = 0.001f;
    float Atk;
    float AnimSpeed = 1.0f;
    int EnemyType;
    int EnemyMoney;
    int clip_index = 0.0f;
    float jank;
    Effect* explosionEffect;
    Effect* firesmokeEffect;
    Effekseer::Handle Effecthandle;

    Microsoft::WRL::ComPtr<IXAudio2> xaudio2;
    IXAudio2MasteringVoice* master_voice = nullptr;
    std::unique_ptr<Lemur::Audio::audio> explosion;

    DirectX::XMFLOAT4 SaveColor = { 1.0f, 1.0f, 1.0f, 1.0f };
    //std::mt19937 mt{ std::random_device{}() };
};

class EnemyInputComponent :public InputComponent
{
    void Initialize(GameObject* gameobj) override {}
    void Update(GameObject* gameobj, float elapsedTime) override;
};

class EnemyPhysicsComponent :public PhysicsComponent
{
    void Initialize(GameObject* gameobj) override;
    void EnemyInitialize(GameObject* gameobj,int StartTime,int EnemyTime) override;
    void Update(GameObject* gameobj, float elapsedTime) override;
    void Reaction(GameObject* gameobj, float elapsedTime);
    int TimerPhisics;
};

class EnemyGraphicsComponent :public GraphicsComponent
{
    void Initialize(GameObject* gameobj) override;
    void Update(GameObject* gameobj) override;
    void Render(GameObject* gameobj, float elapsedTime, ID3D11PixelShader* replaced_pixel_shader) override;


private:
    std::shared_ptr<skinned_mesh> EnemyModel;
    float animation_tick = 0;
};