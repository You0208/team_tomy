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
    enum AnimIndex
    {
        FirstAttack_Anim,
        SecondAttack_Anim,
        ThirdAttack_Anim,

        Run_Anim,
        Idle_Anim,
        Avoid_Anim,
        Death_Anim,

        Max_Anim
    };

    enum StateIndex
    {
        Idle_State,
        Run_State,
        Avoid_State,
        Attack_State,
        Death_State,

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

        // 歩きの速さ(スピードパラメータがあるからベースの速度は激おそ)
        walk_speed = 0.1f;

        attack_power = 50;


        // 最初に所持できるスキルは三つ
        //skills.resize(skill_capacity);
    }

    virtual void DebugImgui() override;
    virtual void DrawDebugPrimitive() override;

    void StateMachineInitialize();
    void StateMachineUpdate();
    Nero::Component::AI::StateMachine* GetStateMachine()const { return state_machine; }

    bool InputMove();

    // 引数のスキル持ってるか検索する関数
    bool HaveSkill(const char* search_skill_name);
    /*-------------- 入力状態の取得を関数化 --------------*/

    bool GetButtonDownB()
    {
        GamePad& game_pad = Input::Instance().GetGamePad();
        return game_pad.GetButtonDown() & GamePad::BTN_B;
    }

    // 敵撃破数取得
    int GetKillCount()const { return kill_count; }

    // 与えたダメージ取得
    int GetAddDamage()const { return add_damage; }
    // 与えたダメージリセット
    void ResetAddDamage() { add_damage = 0; }
private:

    // 敵を倒した数
    int kill_count = 0;

    // スキル系でいろいろ都合がいいから与えたダメージを変数で保持
    int add_damage = 0;

public:/*---------- 当たり判定系 ----------*/

    // ノードとエネミーの衝突処理
    void CollisionNodeVsEnemies(const char* mesh_name,const char* bone_name, float node_radius);

    // 無敵状態(喰らい当たり判定するか)
    bool invincible = false;

    int GetInvincibleFrame()const { return invincible_frame; }
private:

    // 無敵フレーム
    int invincible_frame = 20;

private:/*---------- 移動、速度関係 -----------*/

    DirectX::XMFLOAT3 GetMoveVec(float input_x, float input_y);

private:

    // 旋回の速さ
    float turn_speed = 10.0f;
public:
    // スピードパラメータ
    float speed_power = 10.0f;

private:
    Nero::Component::AI::StateMachine* state_machine = nullptr;

public:/*----------------- スキル関係 -----------------*/

    // プレイヤーにスキルをセット
    void SetSkill(BaseSkill* skill)
    {
        skill->SetOwner(this);
        skills.emplace_back(skill);
    }
    // 所持スキルのInitを呼ぶ
    void SkillInit();
    // 所持スキルのupdateを呼ぶ
    void SkillUpdate();
    // スキルをリセット
    void SkillFin();

    // 所持できるスキル数
    int skill_capacity = 3;
private:
    // 所持してるスキル
    std::vector<BaseSkill*> skills;

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