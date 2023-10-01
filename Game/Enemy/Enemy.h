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
    Enemy() {}
    Enemy(InputComponent* input_,
        PhysicsComponent* physics_,
        GraphicsComponent* graphics_) :GameObject(input_, physics_, graphics_) {}

    // ビヘイビアツリーの初期化
    void BehaviorTreeInitialize();
    // ビヘイビアツリーの更新処理
    void BehaviorTreeUpdate();

    // 方向転換
    void Turn(float vx, float vz, float speed);

    // ターゲットに到達したか判定する(第三引数には到達したって判定する半径)
    bool ReachTargetJudge(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 target_pos, float judge_range);

    // プレイヤー索敵(found_rangeは見つかる範囲、内積で比較)
    // 距離判定と角度判定別々にするより一緒にしたほうがいい？
    //一つだけ判定したいときはもう一つの判定を無視するような引数を設定しよう
    //距離だけ判定：found_range = -1.1f;
    //内積だけ判定：found_distance = FLT_MAX;
    bool SearchPlayer(float found_distance, float found_range = 0.0f);

    // 目標地点へ進む
    void Move_to_Target(float elapsedTime, float move_speed_rate = 1.0f, float turn_speed_rate = 1.0f);


    DirectX::XMFLOAT3 GetTargetPosition()const { return target_position; }
    DirectX::XMFLOAT3 GetTerritoryOrigin()const { return territory_origin; }
    float GetTerritoryRange()const { return territory_range; }
    float GetAttackRange()const { return attack_range; }
    float GetWalkSpeed()const { return walk_speed; }

    // ターゲット位置をランダム設定
    void SetRandomTargetPosition();

private:

    // todo ここら辺の変数と値は企画が決まってないから仮です

    // 目標地点
    DirectX::XMFLOAT3 target_position = { 0.0f,0.0f,0.0f };

    // 縄張り原点
    DirectX::XMFLOAT3 territory_origin{};
    // 縄張りの半径
    float territory_range = 30;
    // 視野範囲(索敵用)
    float vision_length = 10.0f;

    // 攻撃可能範囲
    float attack_range = 3.0f;

    // 歩きの速さ
    float walk_speed = 3.0f;

    BehaviorTree* ai_tree;
    NodeBase* activeNode = nullptr;
    BehaviorData* behaviorData = nullptr;
};

