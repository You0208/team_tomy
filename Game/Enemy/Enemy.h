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
    void Update(GameObject* gameobj, float elapsedTime) override {}
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

    // �r�w�C�r�A�c���[�̏�����
    void BehaviorTreeInitialize();

    // �����]��
    void Turn(float vx, float vz, float speed);


    // �v���C���[���G(found_range�͌�����͈́A���ςŔ�r)
    // ��������Ɗp�x����ʁX�ɂ�����ꏏ�ɂ����ق��������H
    //��������肵�����Ƃ��͂�����̔���𖳎�����悤�Ȉ�����ݒ肵�悤
    //������������Ffound_range = -1.1f;
    //���ς�������Ffound_distance = FLT_MAX;
    bool SearchPlayer(float found_distance, float found_range = 0.0f);

    // �ڕW�n�_�֐i��
    void Move_to_Target(float elapsedTime, float move_speed_rate = 1.0f, float turn_speed_rate = 1.0f);


    DirectX::XMFLOAT3 GetTargetPosition()const { return target_position; }
    float GetWalkSpeed()const { return walk_speed; }
    float GetAttackRange()const { return attack_range; }

    // �^�[�Q�b�g�ʒu�������_���ݒ�
    void SetRandomTargetPosition();

private:

    // todo ������ӂ̕ϐ��ƒl�͊�悪���܂��ĂȂ����牼�ł�

    // �ڕW�n�_
    DirectX::XMFLOAT3 target_position = { 0.0f,0.0f,0.0f };

    // ����͈�(���G�p)
    float vision_length = 10.0f;

    // �U���\�͈�
    float attack_range = 3.0f;

    // �����̑���
    float walk_speed = 3.0f;

    BehaviorTree* ai_tree;
};

