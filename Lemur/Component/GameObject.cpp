#include "GameObject.h"

#include "Game/Easing.h"
#include "Game/MathHelper.h"
#include "Lemur/Collision/Collision.h"


void GameObject::AnimationUpdate(float elapsedTime)
{
    // todo ��񎀂񂾂�A�j���[�V�����o�O��
    //todo �q�b�g�X�g�b�v�ǂ����ɂ��邩���߂�
    // �q�b�g�X�g�b�v���Ȃ�X�V���Ȃ�
    //if (is_hit_stop) return;
    if (Model->animation_clips.size() > 0&&!animStop)
    {
        animation = { Model->animation_clips.at(animation_index) };
        frame_index = static_cast<int>(animation_tick * animation.sampling_rate);
        if (frame_index > animation.sequence.size() - 1)
        {
            end_animation = true;
            frame_index = 0;
            animation_tick = 0;

        }
        else
        {
            end_animation = false;
            animation_tick += elapsedTime * hit_stop_rate * anim_calc_rate;
            keyframe = { animation.sequence.at(frame_index) };
        }
    }
}

bool GameObject::ApplyDamage(int damage)
{
    //if (damage <= defense_power)
    //    damage = 1;
    //else
    //    damage -= static_cast<int>(defense_power);

    //// �_���[�W���O�̏ꍇ�͌��N��Ԃ�ύX����K�v���Ȃ�
    //if (damage <= 0)return false;

    // ���S���Ă���Ԃ͌��N��Ԃ�ύX���Ȃ�
    if (health <= 0)return false;


    // �_���[�W����
    health -= damage;

    // ���S�ʒm
    if (health <= 0)
    {
        health = 0;
        death = true;
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
    DirectX::XMStoreFloat4x4(&world, C * S * R * T);


    if (Model->animation_clips.size() > 0)
    {
#if 1

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

void GameObject::Render(float elapsedTime, ID3D11PixelShader** replaced_pixel_shader)
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
    DirectX::XMMATRIX S{ DirectX::XMMatrixScaling(scale.x, scale.y, scale.z) };
    DirectX::XMMATRIX R{ DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z) };
    DirectX::XMMATRIX T{ DirectX::XMMatrixTranslation(position.x, position.y, position.z) };
    // ���[���h�ϊ��s����쐬
    DirectX::XMStoreFloat4x4(&world, C * S * R * T);


    if (Model->animation_clips.size() > 0)
    {
#if 1

#else
        animation::keyframe keyframe;
        const animation::keyframe* keyframes[2]{
            &skinned_meshes[0]->animation_clips.at(0).sequence.at(40),
            &skinned_meshes[0]->animation_clips.at(0).sequence.at(80)
        };
        skinned_meshes[0]->blend_animations(keyframes, factors[2], keyframe);
        skinned_meshes[0]->update_animation(keyframe);

# endif
        Model->render(immediate_context, world, material_color, &keyframe, replaced_pixel_shader);
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

void GameObject::Turn(float vx, float vz, float speed)
{
    speed *= high_resolution_timer::Instance().time_interval();

    // �x�N�g���̑傫�����擾
    float Length = sqrtf(vx * vx + vz * vz);

    // �x�N�g���̑傫����0�Ȃ�(�[���x�N�g���Ȃ�)
    if (Length <= 0.01)
    {
        return;
    }

    // �i�s�x�N�g���̐��K��
    vx = vx / Length;
    vz = vz / Length;

    // ���g�̉�]�l����O���������߂�B
    float frontX = sinf(rotation.y);
    float frontZ = cosf(rotation.y);

    // ��]�p�����߂邽�߂ɁA2�̒P�ʃx�N�g���̓��ς��v�Z����
    float dot = (vx * frontX) + (vz * frontZ);

    // dot �� -1.0f �` 1.0f �ɂȂ�B�Ȃ̂� rot �� 0.0f �` 2.0f �ɂȂ�B
    float rot = 1.0f - dot;

    // ���ς��������Ȃ�����
    if (rot < speed) speed = rot; // ���̕�������ς���p�x������������

    // ���E������s�����߂�2�̒P�ʃx�N�g���̊O�ς��v�Z����
    float cross = (vx * frontZ) - (vz * frontX);

    // 2D�̊O�ϒl�����̏ꍇ�����̏ꍇ�ɂ���č��E���]���s����
    // ���E������s�����Ƃɂ���č��E��]��I������
    if (cross < 0.0f)
    {
        rotation.y -= speed;
    }
    else
    {
        rotation.y += speed;
    }

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

void GameObject::HitStopON(float hit_stop_time_)
{
    hit_stop_time = hit_stop_time_;
    hit_stop_timer = 0;
    is_hit_stop = true;
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

    if (position.x < -20)
        position.x = -20;
    else if (position.x > 22)
        position.x = 22;

    if (position.z < 3)
        position.z = 3;
    else if (position.z > 50)
        position.z = 50;

#if 0
    // �ړ��ʎ擾
    float VecX = position.x + velocity.x - position.x;
    float VecZ = position.z + velocity.z - position.z;

    // �������͗ʌv�Z
    float velocityLengthXZ = sqrtf((VecX * VecX) + (VecZ * VecZ));

    if (velocityLengthXZ > 0.0f)
    {
        // �����ړ��l
        float mx = velocity.x * elapsedTime;
        float mz = velocity.z * elapsedTime;

        // ���C�̊J�n�ʒu�ƏI�_�ʒu�ݒ�
        DirectX::XMFLOAT4 start = { position.x,position.y,position.z,1 };
        DirectX::XMFLOAT4 end = {
            position.x + mx,
            position.y ,
            position.z + mz,
            1
        };
        start.y += 0.1;
        end.y += 0.1;
        // ���C�n�_
        DirectX::XMVECTOR Start = DirectX::XMLoadFloat4(&start);
        // ���C�I�_
        DirectX::XMVECTOR End = DirectX::XMLoadFloat4(&end);

        // ���C�̕���
        DirectX::XMVECTOR RayVec = DirectX::XMVector4Normalize(DirectX::XMVectorSubtract(End, Start));
        DirectX::XMFLOAT4 ray_vec;
        DirectX::XMStoreFloat4(&ray_vec, RayVec);

        // ���C�L���X�g�ɂ��ǔ���

        // ��_
        DirectX::XMFLOAT4 hit_pos;

        std::string intersected_mesh;
        std::string intersected_material;
        DirectX::XMFLOAT3 hit_normal;

        if(Model->raycast(start, ray_vec, world, hit_pos, hit_normal, intersected_mesh, intersected_material))
        {
            DirectX::XMVECTOR Hit = DirectX::XMLoadFloat4(&hit_pos);
            DirectX::XMVECTOR Start_to_Hit = DirectX::XMVectorSubtract(Hit, Start);
            float ray_length = DirectX::XMVectorGetX(DirectX::XMVector4Length(Start_to_Hit));


            // �q�b�g����
            if (ray_length < velocityLengthXZ)
            {
                // �ړ��ʃx�N�g���I�_
                DirectX::XMVECTOR MoveVecEnd = DirectX::XMVectorScale(DirectX::XMVector3Normalize(Start), velocityLengthXZ);

                // �q�b�g�����|���S���̖@��
                DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit_normal);

                DirectX::XMVECTOR Hit_To_MoveVecEnd = DirectX::XMVectorSubtract(MoveVecEnd, Hit);
                // �ˉe�x�N�g�����Z�o
                DirectX::XMVECTOR Projection = DirectX::XMVectorScale(Normal,
                    DirectX::XMVectorGetX(DirectX::XMVector3Dot(DirectX::XMVectorNegate(Hit_To_MoveVecEnd), Normal)));

                // ���߂ɕ␳���邽�߂ɂ�����Ƒ傫������
                Projection = DirectX::XMVectorScale(Projection, 1.01f);

                // �i�s�x�N�g���̏I�_�Ǝˉe�x�N�g���𑫂��ĕ␳��̈ʒu�x�N�g���Z�o�B
                DirectX::XMVECTOR CollectPosition = DirectX::XMVectorMultiplyAdd(Normal, Projection, MoveVecEnd);

                // �␳��̈ʒu
                DirectX::XMFLOAT4 new_pos;
                DirectX::XMStoreFloat4(&new_pos, CollectPosition);

                /*------------ ��_����␳��̈ʒu�̊Ԃł����������C�L���X�g ----------*/

                // ��_����␳��̈ʒu�܂ł̃x�N�g��
                DirectX::XMVECTOR Hit_To_NewPos = DirectX::XMVectorSubtract(CollectPosition, Hit);
                Hit_To_NewPos = DirectX::XMVector3Normalize(Hit_To_NewPos);

                DirectX::XMStoreFloat4(&ray_vec, Hit_To_NewPos);
                if (Model->raycast(new_pos, ray_vec, world, hit_pos, hit_normal, intersected_mesh, intersected_material))
                {
                    position.x = hit_pos.x;
                    position.y = hit_pos.y;
                    position.z = hit_pos.z;
                }
                else
                {
                    position.x = new_pos.x;
                    position.y = new_pos.y;
                    position.z = new_pos.z;
                }

            }
        }
        else
        {
            position.x += mx;
            position.z += mz;
        }
    }
#endif


}

void GameObject::HitStopCalc()
{
    //todo �q�b�g�X�g�b�v�ǂ����ɂ��邩���߂�

#if 0 �q�b�g�X�g�b�v���u�[���ł��Ċ��S�Ɏ~�܂�p�^�[��
    if(hit_stop_timer>hit_stop_time)
    {
        is_hit_stop = false;
    }

    hit_stop_timer += high_resolution_timer::Instance().time_interval();
#else �q�b�g�X�g�b�v���C�[�W���O�ł��Ă�����Ƃ������o���p�^�[��

    if (hit_stop_timer > hit_stop_time)
    {
        hit_stop_rate = 1.0f;
        is_hit_stop = false;

        return;
    }

    hit_stop_rate = Easing::InSine(hit_stop_timer, hit_stop_time, 1.0f, 0.0f);

    hit_stop_timer += high_resolution_timer::Instance().time_interval();
#endif

}
