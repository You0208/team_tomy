#include "GameObject.h"


bool GameObject::ApplyDamage(int damage, float invincibleTime)
{
    // �_���[�W���O�̏ꍇ�͌��N��Ԃ�ύX����K�v���Ȃ�
    if (damage == 0)return false;

    // ���S���Ă���Ԃ͌��N��Ԃ�ύX���Ȃ�
    if (health <= 0)return false;

    if (invincibleTimer > 0.0f)return false;

    invincibleTimer = invincibleTime;

    // �_���[�W����
    health -= damage;

    // ���S�ʒm
    if (health <= 0)
    {
        OnDead();
    }
    else
    {
        OnDamaged();
    }

    // ���N��Ԃ��ύX�����ꍇ��true��Ԃ�
    return true;
}

void GameObject::Render(float elapsedTime, ID3D11PixelShader* replaced_pixel_shader)
{
    Lemur::Graphics::Graphics& graphics = Lemur::Graphics::Graphics::Instance();

    ID3D11DeviceContext* immediate_context = graphics.GetDeviceContext();

    // ����n�EY ���A�b�v�֕ϊ�
    const DirectX::XMFLOAT4X4 coordinate_system_transforms[]{
        { -1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 },	// 0:RHS Y-UP
        { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 },		// 1:LHS Y-UP
        { -1, 0, 0, 0, 0, 0, -1, 0, 0, 1, 0, 0, 0, 0, 0, 1 },	// 2:RHS Z-UP
        { 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1 },		// 3:LHS Z-UP
    };
#if 0
    // �P�ʂ��Z���`���[�g�����烁�[�g���ɕύX����ɂ́A�uscale_factor�v�� 0.01 �ɐݒ�
    const float scale_factor = 1.0f; // To change the units from centimeters to meters, set 'scale_factor' to 0.01.
#else
    const float scale_factor = 0.01f; // To change the units from centimeters to meters, set 'scale_factor' to 0.01.
#endif
    // �ϊ��p
    DirectX::XMMATRIX C{ DirectX::XMLoadFloat4x4(&coordinate_system_transforms[0])* DirectX::XMMatrixScaling(scale_factor, scale_factor, scale_factor) };
    DirectX::XMMATRIX S{ DirectX::XMMatrixScaling(scale.x,scale.y,scale.z) };
    DirectX::XMMATRIX R{ DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y,rotation.z) };
    DirectX::XMMATRIX T{ DirectX::XMMatrixTranslation(position.x, position.y, position.z) };
    // ���[���h�ϊ��s����쐬
    DirectX::XMFLOAT4X4 world;
    DirectX::XMStoreFloat4x4(&world, C * S * R * T);


    if (Model->animation_clips.size() > 0)
    {
#if 1
        animation& animation{ Model->animation_clips.at(clip_index) };
        frame_index = static_cast<int>(animation_tick * animation.sampling_rate);
        if (frame_index > animation.sequence.size() - 1)
        {
            frame_index = 0;
            animation_tick = 0;
        }
        else
        {
            animation_tick += elapsedTime;
        }
        animation::keyframe& keyframe{ animation.sequence.at(frame_index) };
#else
        animation::keyframe keyframe;
        const animation::keyframe* keyframes[2]{
            &skinned_meshes[0]->animation_clips.at(0).sequence.at(40),
            &skinned_meshes[0]->animation_clips.at(0).sequence.at(80)
        };
        skinned_meshes[0]->blend_animations(keyframes, factors[2], keyframe);
        skinned_meshes[0]->update_animation(keyframe);

# endif
        Model->render(immediate_context, world,material_color, &keyframe, replaced_pixel_shader);
    }
    else
    {
        Model->render(immediate_context, world, material_color, nullptr, replaced_pixel_shader);
    }
}

void GameObject::Move(float vx, float vz, float speed)
{
    // �ړ������x�N�g����ݒ�
    moveVecX = vx;
    moveVecZ = vz;

    // �ő呬�x�ݒ�
    maxMoveSpeed = speed;
}

void GameObject::Jump(float speed)
{
    // ������̗͂�ݒ�
    velocity.y = speed;
}

void GameObject::UpdateVelocity(float elapsedTime)
{
    // �o�߃t���[��
    float elapsedFrame = 60.0f * elapsedTime;

    // �������͍X�V����
    UpdataVerticalVelocity(elapsedFrame);

    // �������͍X�V����
    UpdataHorizontalVelocity(elapsedFrame);

    // �����ړ��X�V����
    UpdateVerticalMove(elapsedTime);

    // �����ړ��X�V����
    UpdateHorizontalMove(elapsedTime);
}

void GameObject::UpdataVerticalVelocity(float elapsedFrame)
{
    // �d�͏���
    velocity.y += gravity * elapsedFrame;//�O���r�e�B�ƒP�ʂ����킹�邽�߂�elapsedFrame���g���Ă���
}

void GameObject::UpdateVerticalMove(float elapsedTime)
{
    // �ړ�����
    position.y += velocity.y * elapsedTime;

    // �n�ʔ���
    if (position.y < 0.0f)
    {
        position.y = 0.0f;
        velocity.y = 0.0f;

        // ���n����
        if (!isGround)
        {
            OnLanding();
        }
        isGround = true;
    }
    else
    {
        isGround = false;
    }
}

void GameObject::UpdataHorizontalVelocity(float elapsedFrame)
{   // XZ���ʂ̑��͂���������
    float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
    if (length > 0.0f)
    {
        // ���C��
        float friction = this->friction * elapsedFrame;

        // �󒆂ɂ���Ƃ��͖��C�͂����炷
        if (!isGround)friction *= airControl;

        // ���C�ɂ�鉡�����̌�������
        if (length > friction)
        {
            float vx = velocity.x / length;
            float vz = velocity.z / length;

            velocity.x -= vx * friction;
            velocity.z -= vz * friction;
        }
        // �������̑��͂����C�͈ȉ��ɂȂ����̂ő��͂𖳌���
        else
        {
            velocity.x = 0;
            velocity.z = 0;
        }
    }

    // XZ���ʂ̑��͂���������
    if (length <= maxMoveSpeed)
    {
        // �ړ��x�N�g�����[���x�N�g���o�Ȃ��Ȃ��������
        float moveVecLength = sqrtf(moveVecX * moveVecX + moveVecZ * moveVecZ);
        if (moveVecLength > 0.0f)
        {
            // ������
            float acceleration = this->acceleration * elapsedFrame;

            // �󒆂ɂ���Ƃ��͖��C�͂����炷
            if (!isGround)acceleration *= airControl;

            // �ړ��x�N�g���ɂ���������
            velocity.x += moveVecX * acceleration;
            velocity.z += moveVecZ * acceleration;

            // �ő呬�x����
            float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
            if (length > maxMoveSpeed)
            {
                float vx = velocity.x / length;
                float vz = velocity.z / length;

                velocity.x = vx * maxMoveSpeed;
                velocity.z = vz * maxMoveSpeed;
            }
        }
    }
    // �ړ��x�N�g�������Z�b�g
    moveVecX = 0.0f;
    moveVecZ = 0.0f;
}

void GameObject::UpdateHorizontalMove(float elapsedTime)
{
    // �ړ�����
    position.x += velocity.x * elapsedTime;
    position.z += velocity.z * elapsedTime;
}