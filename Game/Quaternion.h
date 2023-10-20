#pragma once
#include"DirectXMath.h"

namespace Nero::Math
{
    class Quaternion :public DirectX::XMFLOAT4
    {
    public:
        Quaternion();
        ~Quaternion();

        // �s��̉��Ԗڂ��Ƃ�Ƃ������Ȃ��Ă����B
        // ��]�̐��ʂ��Ƃ�B
        DirectX::XMFLOAT3 GetForward()const;

        // ��]�̉E���Ƃ�B
        DirectX::XMFLOAT3 GetRight()const;

        // ��]�̏���Ƃ�B
        DirectX::XMFLOAT3 GetUp()const;

        // �x�N�g������]������B
        DirectX::XMFLOAT3 RotateVector3(const DirectX::XMFLOAT3& v)const;
        Quaternion operator*(const Quaternion& q)const;


        static Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, float t);
        static float Angle(const Quaternion& q1, const Quaternion& q2);

        // �N�H�[�^�j�I���쐬�n
        static Quaternion AngleAxis(const DirectX::XMFLOAT3& axis, float angle);
        static Quaternion RollPitchYaw(float pitch, float yaw, float roll);
        static Quaternion LookTo(const DirectX::XMFLOAT3& vec, const DirectX::XMFLOAT3& up = { 0,1,0 });


    };
}
