#pragma once
#include "Game/AI/BehaviorTree.h"
#include "Lemur/Component/GameObject.h"


//こいつらは実体にはならない。コンポーネントとして実体になるやつに搭載される。

class EnemyInputComponent :public InputComponent
{
    void Initialize(GameObject* gameobj) override {}
    void Update(GameObject* gameobj, float elapsedTime) override;

    DirectX::XMFLOAT3 GetMoveVec(float input_x, float input_y);

};

class EnemyPhysicsComponent :public PhysicsComponent
{
    void Initialize(GameObject* gameobj) override;
    void Update(GameObject* gameobj, float elapsedTime) override;
};

class EnemyGraphicsComponent :public GraphicsComponent
{
    void Initialize(GameObject* gameobj) override;
    void Update(GameObject* gameobj) override;
    void Render(GameObject* gameobj, float elapsedTime, ID3D11PixelShader* replaced_pixel_shader) override;
private:

};

class Enemy :public GameObject
{
public:
    enum EnemyType
    {
        SmallSpider,

        A_Spider,
        B_Spider,
        C_Spider,
        D_Spider,
        E_Spider,
        F_Spider,
        G_Spider,
        H_Spider,

        BossSpider,
    }enemy_type;

    enum AnimIndex
    {
        Walk_Anim,
        NearAttack_Anim,
        ShotAttack_Anim,
        BackStep_Anim,
        Fear_Anim,
        JumpAttack_Anim,
        //Death_Anim,

        Max_Anim,
    };
public:
    Enemy(){}
    Enemy(InputComponent* input_,
        PhysicsComponent* physics_,
        GraphicsComponent* graphics_,
        EnemyType type):enemy_type(type),GameObject(input_, physics_, graphics_)
    {
        // todo 調整

        height=1.0f;
        radius = 0.5f;

        walk_speed = 1.5f;


        max_health = 1000;
        health = max_health;
        defense_power = 50.5f;

        attack_power = 1;
        attack_collision_range = 0.3f;

        near_attack_range = 1.5f;
        middle_attack_range = 5.0f;

    }

    // 攻撃当たり判定で使う
    std::string meshName;

    // ビヘイビアツリーの初期化
    virtual void BehaviorTreeInitialize(){};
    // ビヘイビアツリーの更新処理
    void BehaviorTreeUpdate();


    // 距離を判定する関数(第三引数には到達したって判定する半径)
    bool DistanceJudge(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 target_pos, float judge_range);

    // プレイヤー索敵(found_rangeは見つかる範囲、内積で比較)
    // 距離判定と角度判定別々にするより一緒にしたほうがいい？
    //一つだけ判定したいときはもう一つの判定を無視するような引数を設定しよう
    //距離だけ判定：found_range = -1.1f;
    //内積だけ判定：found_distance = FLT_MAX;
    bool SearchPlayer(float found_distance, float found_range = 0.0f);

    // 目標地点へ進む
    void Move_to_Target(float elapsedTime, float move_speed_rate = 1.0f, float turn_speed_rate = 1.0f);

    void CollisionNodeVsPlayer(const char* mesh_name, const char* bone_name, float node_radius);

    DirectX::XMFLOAT3 GetTargetPosition()const { return target_position; }
    DirectX::XMFLOAT3 GetTerritoryOrigin()const { return territory_origin; }
    float GetTerritoryRange()const { return territory_range; }
    float GetVisionLength()const { return vision_length; }
    float GetNearAttackRange()const { return near_attack_range; }
    float GetMiddleAttackRange()const { return middle_attack_range; }

    // ターゲット位置を設定
    void SetTargetPosition(DirectX::XMFLOAT3 position) { target_position = position; };
    // ターゲット位置をランダム設定
    void SetRandomTargetPosition();

    void DebugImgui() override;

    // 破棄
    void Destroy();

    // 多段ヒットしないように攻撃喰らったらtrue
    bool is_hit = false;

    // 怯みフラグ
    bool fear_frag;
protected:

    // todo ここら辺の変数と値は企画が決まってないから仮です

    // 目標地点
    DirectX::XMFLOAT3 target_position = { 0.0f,0.0f,0.0f };

    // 縄張り原点
    DirectX::XMFLOAT3 territory_origin{};
    // 縄張りの半径
    float territory_range = 30;
    // 視野範囲(索敵用)
    float vision_length = 30.0f;

    // 近攻撃可能範囲
    float near_attack_range = 0.0f;
    float middle_attack_range = 0.0f;


    BehaviorTree* ai_tree = nullptr;
    NodeBase* activeNode = nullptr;
    BehaviorData* behaviorData = nullptr;
};

