#pragma once
#include "..\Skill\BaseSkill.h"
#include "Lemur/Component/GameObject.h"
//#include "Game/StateMachine/StateMachine.h"

namespace Nero::Component::AI
{
    class StateMachine;
}

class Player:public GameObject
{
public:
    enum AnimIndex
    {
        Run_Anim,
        Idle_Anim,
        Avoid_Anim,

        Max_Anim
    };

    enum StateIndex
    {
        Idle_State,
        Walk_State,
        Avoid_State,

        Max_State,
    };
public:
    Player() {}
    Player(InputComponent* input_,
        PhysicsComponent* physics_,
        GraphicsComponent* graphics_) :GameObject(input_, physics_, graphics_) {}

    virtual void DebugImgui() override;

    void StateMachineInitialize();
    void StateMachineUpdate();
    Nero::Component::AI::StateMachine* GetStateMachine()const { return state_machine; }

    bool InputMove();
private:
    DirectX::XMFLOAT3 GetMoveVec(float input_x, float input_y);


private:
    // 歩きの速さ
    float walk_speed = 3.0f;

    // 旋回の速さ
    float turn_speed = 10.0f;

    Nero::Component::AI::StateMachine* state_machine = nullptr;

    /*----------------- スキル関係 -----------------*/
    std::unordered_map<std::string, std::unique_ptr<BaseSkill>> skills;

};

//こいつらは実体にはならない。コンポーネントとして実体になるやつに搭載される。
class PlayerInputComponent :public InputComponent
{
    void Initialize(GameObject* gameobj) override {}
    void Update(GameObject* gameobj, float elapsedTime) override;

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