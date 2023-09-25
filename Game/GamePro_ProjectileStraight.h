#pragma once

#include "../Lemur/Graphics/skinned_mesh.h"

#include "../Lemur/Component/GameObject.h"
#include "../Lemur/Graphics/skinned_mesh.h"
#include "../Lemur/Graphics/Graphics.h"
#include "../Lemur/Resource/ResourceManager.h"
#include"./Lemur/Effekseer/Effect.h"

#include <wrl.h>
#include"./Lemur/Audio/audio.h"

class GamePro_ProjectileStraight : public GameObject
{
public:
    GamePro_ProjectileStraight(InputComponent* input_,
        PhysicsComponent* physics_,
        GraphicsComponent* graphics_) :GameObject(input_, physics_, graphics_) {}

    //”­ŽË
    //void Launch(GameObject* gameobj,const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position);


    float speed = 1.0f;
    float attack = 1.0f;
    
    Effect* projectEffect;
    DirectX::XMFLOAT3 GiftAngle;
    DirectX::XMFLOAT4 GiftPosition;

    Effekseer::Handle handle;

    Microsoft::WRL::ComPtr<IXAudio2> xaudio2;
    IXAudio2MasteringVoice* master_voice = nullptr;
    std::unique_ptr<Lemur::Audio::audio> shot;
};


class GamePro_ProjectileStraightInputComponent :public InputComponent
{
    void Initialize(GameObject* gameobj) override {}
    void Update(GameObject* gameobj, float elapsedTime) override;
};

class GamePro_ProjectileStraightPhysicsComponent :public PhysicsComponent
{
    void Initialize(GameObject* gameobj) override;
    void Update(GameObject* gameobj, float elapsedTime) override;
};

class GamePro_ProjectileStraightGraphicsComponent :public GraphicsComponent
{
    void Initialize(GameObject* gameobj) override;
    void Update(GameObject* gameobj) override;
    void Render(GameObject* gameobj, float elapsedTime, ID3D11PixelShader* replaced_pixel_shader) override;

private:
    std::shared_ptr<skinned_mesh> BulletModel;

};