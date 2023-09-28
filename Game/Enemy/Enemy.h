#pragma once
#include "Game/AI/BehaviorTree.h"
#include "Lemur/Component/GameObject.h"

class Enemy:public GameObject
{
public:
    Enemy() {}
    Enemy(InputComponent* input_,
        PhysicsComponent* physics_,
        GraphicsComponent* graphics_) :GameObject(input_, physics_, graphics_) {}

    // �r�w�C�r�A�c���[�̏�����
    void BehaviorTreeInitialize();
private:

    BehaviorTree* ai_tree;
};


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
    std::shared_ptr<skinned_mesh> EnemyModel;
};