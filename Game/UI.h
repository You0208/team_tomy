#pragma once

#include "../Lemur/Component/GameObject.h"
#include "../Lemur/Graphics/sprite.h"
#include "../Lemur/Graphics/Graphics.h"
#include "../Lemur/Resource/ResourceManager.h"

#include <wrl.h>
#include"./Lemur/Audio/audio.h"

#include "Lemur/Input/Mouse.h"
#include <sstream>

class Ui :public GameObject
{
public:
    Ui(InputComponent* input_,
        PhysicsComponent* physics_,
        GraphicsComponent* graphics_) :GameObject(input_, physics_, graphics_) {}


    DirectX::XMFLOAT2 pausePosition = { 1920,0 };

    bool ShopFlag;
    int UiTypes;
    int shopUiNum;
    enum ShopNumber {
        SpeedUp_A = 0,
        SpeedUp_P,
        Mine,
        Canon,
        PowerUp,
    };
    int player_HP;
    int player_MAXHP;
    int junkDigits = 1;
    int levelDigits = 1;
    float AddParameter;
    int saveJank[30];
    int savelavel_min[10][3];
    int savelevel[5];
    int price;
    int jank;
    int jankdig_;
    int laveldig_[10];
    float uitimer;
    DirectX::XMFLOAT2 Uiposition;
    DirectX::XMFLOAT2 Uiposition2;
    DirectX::XMFLOAT2 Uiposition_[10];
    DirectX::XMFLOAT2 Uisize;
    DirectX::XMFLOAT2 Uisize2;
    DirectX::XMFLOAT2 Uisize_[10];
    DirectX::XMFLOAT4 UiColor;
    DirectX::XMFLOAT4 HPUiColor[30];

    Microsoft::WRL::ComPtr<IXAudio2> xaudio2;
    IXAudio2MasteringVoice* master_voice = nullptr;
    std::unique_ptr<Lemur::Audio::audio> shop;
    std::unique_ptr<Lemur::Audio::audio> result;
    

};

class UiInputComponent :public InputComponent
{
    void Initialize(GameObject* gameobj) override {}
    void Update(GameObject* gameobj, float elapsedTime) override;
};

class UiPhysicsComponent :public PhysicsComponent
{
    void Initialize(GameObject* gameobj) override;
    void Update(GameObject* gameobj, float elapsedTime) override;
};

class UiGraphicsComponent :public GraphicsComponent
{
    void Initialize(GameObject* gameobj) override;
    void Update(GameObject* gameobj) override;
    void Render(GameObject* gameobj, float elapsedTime, ID3D11PixelShader* replaced_pixel_shader) override;

private:
    std::shared_ptr<sprite> pause;
    std::shared_ptr<sprite> option[5];
    std::shared_ptr<sprite> UiBase[5];
    std::shared_ptr<sprite> Uiitem[10];
    std::shared_ptr<sprite> HPbit;
};