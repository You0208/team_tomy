#pragma once
#include "Game/AI/BehaviorTree.h"
#include "Lemur/Component/GameObject.h"


//������͎��̂ɂ͂Ȃ�Ȃ��B�R���|�[�l���g�Ƃ��Ď��̂ɂȂ��ɓ��ڂ����B

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
    std::string enemy_type;

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
        GraphicsComponent* graphics_):GameObject(input_, physics_, graphics_)
    {
        // todo ����

        height=1.0f;
        radius = 0.5f;


        max_health = 1000;
        health = max_health;
        defense_power = 50.5f;

        attack_power = 1;
        attack_collision_range = 0.3f;

        near_attack_range = 1.5f;
        middle_attack_range = 5.0f;


    }

    // �U�������蔻��Ŏg��
    std::string meshName = "polySurface";
    
    

    // �r�w�C�r�A�c���[�̏�����
    virtual void BehaviorTreeInitialize(){};

    /*------------- �eAI���x���̃r�w�C�r�A�c���[�̍\�z -------------*/


    virtual void BehaviorTreeInitialize_Level1();
    virtual void BehaviorTreeInitialize_Level2();
    virtual void BehaviorTreeInitialize_Level3();
    virtual void BehaviorTreeInitialize_Level4();

    // �{�X�X�p�C�_�[�p��AI
    virtual void BehaviorTreeInitialize_Level5();


    // �r�w�C�r�A�c���[�̍X�V����
    void BehaviorTreeUpdate();


    // �����𔻒肷��֐�(��O�����ɂ͓��B�������Ĕ��肷�锼�a)
    bool DistanceJudge(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 target_pos, float judge_range);

    // �v���C���[���G(found_range�͌�����͈́A���ςŔ�r)
    // ��������Ɗp�x����ʁX�ɂ�����ꏏ�ɂ����ق��������H
    //��������肵�����Ƃ��͂�����̔���𖳎�����悤�Ȉ�����ݒ肵�悤
    //������������Ffound_range = -1.1f;
    //���ς�������Ffound_distance = FLT_MAX;
    bool SearchPlayer(float found_distance, float found_range = 0.0f);

    // �ڕW�n�_�֐i��
    void Move_to_Target(float elapsedTime, float move_speed_rate = 1.0f, float turn_speed_rate = 1.0f);

    void CollisionNodeVsPlayer(const char* mesh_name, const char* bone_name, float node_radius);

    DirectX::XMFLOAT3 GetTargetPosition()const { return target_position; }
    DirectX::XMFLOAT3 GetTerritoryOrigin()const { return territory_origin; }
    float GetTerritoryRange()const { return territory_range; }
    float GetVisionLength()const { return vision_length; }
    float GetNearAttackRange()const { return near_attack_range; }
    float GetMiddleAttackRange()const { return middle_attack_range; }

    // �^�[�Q�b�g�ʒu��ݒ�
    void SetTargetPosition(DirectX::XMFLOAT3 position) { target_position = position; };
    // �^�[�Q�b�g�ʒu�������_���ݒ�
    void SetRandomTargetPosition();

    void DamageRender(const float damage);
    void DebugImgui() override;

    // �j��
    void Destroy();

    // ���i�q�b�g���Ȃ��悤�ɍU����������true
    bool is_hit = false;

    // ���݃t���O
    bool fear_frag;

    // �����
    bool belligerency = false;

    // �����킹������(�i���Ɏ����킹���Ȃ����߂�)
    //�߂�ǂ�����p�u���b�N
    bool turned = false;

protected:

    // todo ������ӂ̕ϐ��ƒl�͊�悪���܂��ĂȂ����牼�ł�

    // �ڕW�n�_
    DirectX::XMFLOAT3 target_position = { 0.0f,0.0f,0.0f };

    // �꒣�茴�_
    DirectX::XMFLOAT3 territory_origin{};
    // �꒣��̔��a
    float territory_range = 30;
    // ����͈�(���G�p)
    float vision_length = 30.0f;

    // �ߍU���\�͈�
    float near_attack_range = 0.0f;
    // �ߍU���\�͈�
    float middle_attack_range = 0.0f;


    BehaviorTree* ai_tree = nullptr;
    NodeBase* activeNode = nullptr;
    BehaviorData* behaviorData = nullptr;

protected:/*----------- �����蔻��n -------------*/

    struct NodeCollision
    {
        std::string bone_name;
        float node_radius;
    };

public:
    // ��炢�����蔻��z��
    std::vector<std::unique_ptr<NodeCollision>> hit_collisions;

    // �r�U�������蔻��z��
    std::vector<std::unique_ptr<NodeCollision>> arm_attack_collisions;
    virtual void SetUpHitCollision(){}

public:
    std::unique_ptr<sprite> damage_spr;
};

