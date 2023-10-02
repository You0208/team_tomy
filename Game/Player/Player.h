#pragma once
#include "Game/StateMachine/StateMachine.h"
#include "Lemur/Component/GameObject.h"

class Player:public GameObject
{
public:
    Player() {}
    Player(InputComponent* input_,
        PhysicsComponent* physics_,
        GraphicsComponent* graphics_) :GameObject(input_, physics_, graphics_) {}

    virtual void DebugImgui() override;

    void StateMachineInitialize();
    void StateMachineUpdate();
private:
    Nero::Component::AI::StateMachine* state_machine = nullptr;
};

//こいつらは実体にはならない。コンポーネントとして実体になるやつに搭載される。
class PlayerInputComponent :public InputComponent
{
    void Initialize(GameObject* gameobj) override {}
    void Update(GameObject* gameobj, float elapsedTime) override;

    DirectX::XMFLOAT3 GetMoveVec(float input_x, float input_y);

};

class PlayerPhysicsComponent :public PhysicsComponent
{
    void Initialize(GameObject* gameobj) override;
    void Update(GameObject* gameobj, float elapsedTime) override;
};

class PlayerGraphicsComponent :public GraphicsComponent
{
    void Initialize(GameObject* gameobj) override;
    void Update(GameObject* gameobj) override;
    void Render(GameObject* gameobj, float elapsedTime, ID3D11PixelShader* replaced_pixel_shader) override;
};