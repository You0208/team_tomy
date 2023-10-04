#pragma once
#include "Component.h"
#include "../Graphics/skinned_mesh.h"
#include "../Graphics/Graphics.h"
#include "../Resource/ResourceManager.h"
#include "./Lemur/Input/Mouse.h"
#include <d3d11.h>
#include <wrl.h>
#include <directxmath.h>
#include <vector>
#include <unordered_map>
#include <string>
#include <fbxsdk.h>

class GameObject
{
public:
    GameObject() {};
    GameObject(InputComponent* input_,
        PhysicsComponent* physics_,
        GraphicsComponent* graphics_)
        :input(input_),
        physics(physics_),
        graphics(graphics_)
    {}


    // �R���|�[�l���g
    virtual void Initialize()
    {
        input->Initialize(this);
        physics->Initialize(this);
        graphics->Initialize(this);
    }

    virtual void EnemyInitialize(int e_t,int s_t)
    {
        input->Initialize(this);
        physics->EnemyInitialize(this,e_t,s_t);
        graphics->Initialize(this);
    }

    void Update(float elapsedTime)
    {
        UpdateScale();
        AnimationUpdate(elapsedTime);
        input->Update(this, elapsedTime);
        physics->Update(this, elapsedTime);
        graphics->Update(this);
    }

    void Render(float elapsedTime)
    {
        graphics->Render(this,elapsedTime, pixelShader);
    }

    void Delete()
    {
        delete input;
        delete physics;
        delete graphics;
    }

    virtual void DebugImgui(){};
    virtual void DrawDebugPrimitive() {};
    void AnimationUpdate(float elapsedTime);
    animation animation{};
    animation::keyframe keyframe{};
    DirectX::XMFLOAT4X4 world;
public:
    // �ʒu�擾
    const DirectX::XMFLOAT3& GetPosition() const { return position; }

    // �ʒu�X�V
    void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

    // ��]�擾
    const DirectX::XMFLOAT3& GetAngle() const { return rotation; }

    // ��]�X�V
    void SetAngle(const DirectX::XMFLOAT3& rotation) { this->rotation = rotation; }

    // �X�P�[���擾
    const DirectX::XMFLOAT3& GetScale() const { return scale; }

    // �X�P�[���X�V
    void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }

    // �X�P�[���t�@�N�^�[�擾
    const float& GetScaleFactor() const{ return scaleFactor; }

    // �X�P�[���t�@�N�^�[�X�V
    void SetScaleFactor(const float scaleFactor) { this->scaleFactor = scaleFactor; }

    // ���a�擾
    float GetRadius() const { return radius; }

    // �����擾
    float GetHeight() const { return height; }

    // �n�ʂɐݒu���Ă��邩
    bool IsGround() const { return isGround; }

    // ���N��Ԃ��擾
    int GetHealth() const { return health; }

    // �ő匒�N��Ԃ��擾
    int GetMaxHealth() const { return maxHealth; }

    // �_���[�W��^����
    bool ApplyDamage(int damage, float invincibleTime);

    // �F�擾
    const DirectX::XMFLOAT4& GetColor() { return material_color; }

    // ���f���ݒ�
    void SetModel(std::shared_ptr<skinned_mesh> Model) { this->Model = Model; }

    // ���f���擾(AI���ŃA�j���[�V�����̐؂�ւ�������������܂��� By�g�~�[)
    skinned_mesh* GetModel()const { return Model.get(); }

    // �A�j���[�V�����̐؂�ւ�
    void SetAnimationIndex(int index)
    {
        frame_index = 0;
        animation_tick = 0;
        animation_index = index;
        end_animation = false;
    }

    // �A�j���[�V�����I���t���O�擾
    bool GetEndAnimation()const { return end_animation; }

    // �`��ݒ�
    void Render(float elapsedTime, ID3D11PixelShader* replaced_pixel_shader);

    // �ړ�����
    void Move(float vx, float vz, float speed);
    // �����]��
    void Turn(float vx, float vz, float speed);

    void SetPixelShader(ID3D11PixelShader* ps) { pixelShader = ps; }

    // ���͍X�V
    void UpdateVelocity(float elapsedTime);

protected:


    // �W�����v����
    void Jump(float speed);


    // ���n�������ɌĂ΂��
    virtual void OnLanding() {};

    // �_���[�W���󂯂��Ƃ��ɌĂ΂��
    virtual void OnDamaged() {}

    // ���S�������ɌĂ΂��
    virtual void OnDead() {}

private:
    void UpdateScale() {
        scale.x = scale.y = scale.z = scaleFactor;
    }

    // �������͍X�V����
    void UpdataVerticalVelocity(float elapsedFrame);

    // �����ړ��X�V����
    void UpdateVerticalMove(float elapsedTime);

    // �������͍X�V����
    void UpdataHorizontalVelocity(float elapsedFrame);

    // �����ړ��X�V����
    void UpdateHorizontalMove(float elapsedTime);

protected:
    DirectX::XMFLOAT4X4 World;

    DirectX::XMFLOAT3 velocity          ={ 0, 0, 0 };  // ���x
    DirectX::XMFLOAT3 position          ={ 0, 0, 0 };  // �ʒu
    DirectX::XMFLOAT3 scale             ={ 3.0f, 3.0f, 3.0f }; // �X�P�[��
    DirectX::XMFLOAT3 rotation          ={ 0, 0, 0 }; //�@��]
    DirectX::XMFLOAT4 material_color    ={ 1, 1, 1, 1 }; // �F

    DirectX::XMFLOAT3 direction         = { 0,0,1 };// ����
    DirectX::XMFLOAT4X4 transform       = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };// �s��

    float   invincibleTimer             = 1.0f; // ���G����
    float   maxMoveSpeed                = 5.0f; // �ő呬�x
    float   moveVecX                    = 0.0f; // X�����ړ�
    float   moveVecZ                    = 0.0f; // Z�����ړ�
    float   friction                    = 0.5f; // ���C��
    float   acceleration                = 1.0f; // ������
    float   gravity                     = -1.0f; // �d��
    float   radius                      = 1.0f; // ���a
    float   height                      = 0.0f; // ���� 

    float   scaleFactor                 = 1.0f;

    float	airControl                  = 0.3f; // ��C��R

    bool    isGround                    = false; // ���n�t���O
    int     health                      = 5; // ���N���
    int     maxHealth                   = 5; // �ő匒�N���


    float animation_tick                = 0; // �A�j���[�V����
    std::shared_ptr<skinned_mesh> Model = nullptr;
    // �A�j���[�V�����p(�����o�����Ƃ��܂����B����ɑ�������ϐ��Ƃ������������炲�߂�)
    //���O��clip_index����ς��܂����B
    int animation_index = 0;
    int frame_index = 0;

    // �A�j���[�V�����I���t���O
    bool end_animation = false;

    Mouse* mouse;

    ID3D11PixelShader* pixelShader      = nullptr; // �V�F�[�_�[
private:
    InputComponent* input;
    PhysicsComponent* physics;
    GraphicsComponent* graphics;
};