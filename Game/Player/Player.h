#pragma once
#include "Lemur/Input/Input.h"
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
    //enum AnimIndex
    //{
    //    Run_Anim,
    //    Idle_Anim,
    //    Avoid_Anim,

    //    Max_Anim
    //};

    enum TestAnimIndex
    {
        FullAttack,
        First_Attack,
        Second_Attack,
        Third_Attack,

        Max_Anim,
    };

    enum StateIndex
    {
        Idle_State,
        Walk_State,
        Avoid_State,
        Attack_State,

        Max_State,
    };
public:
    Player(){}
    Player(InputComponent* input_,
        PhysicsComponent* physics_,
        GraphicsComponent* graphics_) :GameObject(input_, physics_, graphics_)
    {
        height = 1.7f;
        radius = 0.4f;
    }

    virtual void DebugImgui() override;
    virtual void DrawDebugPrimitive() override;

    void StateMachineInitialize();
    void StateMachineUpdate();
    Nero::Component::AI::StateMachine* GetStateMachine()const { return state_machine; }

    bool InputMove();


    /*-------------- ���͏�Ԃ̎擾���֐��� --------------*/

    bool GetButtonDownB()
    {
        GamePad& game_pad = Input::Instance().GetGamePad();
        return game_pad.GetButtonDown() & GamePad::BTN_B;
    }



private:
    DirectX::XMFLOAT3 GetMoveVec(float input_x, float input_y);


private:

    // �����̑���
    float walk_speed = 3.0f;

    // ����̑���
    float turn_speed = 10.0f;

    Nero::Component::AI::StateMachine* state_machine = nullptr;

private:
    /*----------------- �X�L���֌W -----------------*/

    // �����X�L����Init���Ă�
    void SkillInit();
    // �����X�L����update���Ă�
    void SkillUpdate();
    // �X�L�������Z�b�g
    void SkillFin();

private:
    std::unordered_map<std::string, std::unique_ptr<BaseSkill>> skills;

};

//������͎��̂ɂ͂Ȃ�Ȃ��B�R���|�[�l���g�Ƃ��Ď��̂ɂȂ��ɓ��ڂ����B
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