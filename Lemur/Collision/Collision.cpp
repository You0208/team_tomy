#include "Collision.h"


void CollisionPhysicsComponent::Initialize(GameObject* gameobj)
{

}

void CollisionPhysicsComponent::Update(GameObject* gameobj, float elapsedTime)
{

}



//�@���Ƌ���������
bool Collision::IntersectSphereVsSphereOut(
    const DirectX::XMFLOAT3& positionA,
    float radiusA,
    const DirectX::XMFLOAT3& positionB,
    float radiusB,
    DirectX::XMFLOAT3& outPositionB
)
{
    //// A��B�̒P�ʃx�N�g�����Z�o
    //DirectX::XMVECTOR PositionA = { positionA.x,positionA.y,positionA.z };
    //DirectX::XMVECTOR PositionB = { positionA.x,positionA.y,positionA.z };
    //DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(PositionB, PositionA);
    //DirectX::XMVECTOR LengthSq = DirectX::XMVector3Length(Vec);
    //float lengthSq;
    //DirectX::XMStoreFloat(&lengthSq, LengthSq);
    //
    //// ��������
    //float range = radiusA + radiusB;
    //if (range < lengthSq)
    //{
    //    return false;
    //}
    //
    //// A��B�������o������
    //DirectX::XMVECTOR OutVec = DirectX::XMVector3Normalize(Vec);
    //DirectX::XMVECTOR OutVecLeng = DirectX::XMVectorScale(OutVec,(range - lengthSq));
    //DirectX::XMVECTOR newPositionB = DirectX::XMVectorAdd(PositionB, OutVec);
    //DirectX::XMVECTOR NewPositionB = DirectX::XMVectorMultiplyAdd(OutVecLeng, OutVec, PositionB);
    //
    //// ���ʂ��o��
    //DirectX::XMStoreFloat3(&outPositionB, newPositionB);
    //
    //return true;

    // A��B�̒P�ʃx�N�g�����Z�o
    DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat3(&positionA);
    DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat3(&positionB);
    DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(PositionB, PositionA);
    DirectX::XMVECTOR LengthSq = DirectX::XMVector3LengthSq(Vec);//���[�g����ĂȂ��iC�̓��̏�ԁj
    float lengthSq;
    DirectX::XMStoreFloat(&lengthSq, LengthSq);

    // ��������
    float range = radiusA + radiusB;
    if (lengthSq > range * range)
    {
        return false;
    }

    // A��B�������o��
    Vec = DirectX::XMVector3Normalize(Vec);// �P�ʃx�N�g��
    Vec = DirectX::XMVectorScale(Vec, range);// �����𔼌a�̑������킹�����ɂ�
    PositionB = DirectX::XMVectorAdd(PositionA, Vec);//�`�̍��W�Ƀx�N�g���𑫂�
    DirectX::XMStoreFloat3(&outPositionB, PositionB);//�|�W�V�����a�����߂�

    return true;
}

//�@���Ƌ���������
bool Collision::IntersectSphereVsSphere(
    const DirectX::XMFLOAT3& positionA,
    float radiusA,
    const DirectX::XMFLOAT3& positionB,
    float radiusB
)
{
    // A��B�̒P�ʃx�N�g�����Z�o
    DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat3(&positionA);
    DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat3(&positionB);
    DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(PositionB, PositionA);
    DirectX::XMVECTOR LengthSq = DirectX::XMVector3LengthSq(Vec);//���[�g����ĂȂ��iC�̓��̏�ԁj
    float lengthSq;
    DirectX::XMStoreFloat(&lengthSq, LengthSq);

    // ��������
    float range = radiusA + radiusB;
    if (lengthSq > range * range)
    {
        return false;
    }

    return true;
}

bool Collision::IntersectCylinderVsCylinder(
    const DirectX::XMFLOAT3& positionA,
    float radiusA,
    float heightA,
    const DirectX::XMFLOAT3& positionB,
    float radiusB,
    float heightB,
    DirectX::XMFLOAT3& outPositionB)
{
    // A�̑�����B�̓�����Ȃ瓖�����Ă��Ȃ�
    if (positionA.y > positionB.y + heightB)
    {
        return false;
    }
    // A�̓���B�̑�����艺�Ȃ瓖�����Ă��Ȃ�
    if (positionA.y + heightA < positionB.y)
    {
        return false;
    }

    // A��B�̒P�ʃx�N�g�����Z�o
    float vx = positionB.x - positionA.x;//�x�N�g�������߂�
    float vz = positionB.z - positionA.z;//�x�N�g�������߂�
    float distXZ = sqrtf(vx * vx + vz * vz);//���������߂�
    float radius = radiusA + radiusB;//���a�̍��v�����߂�
    if (distXZ > radius)//��ׂ�
    {
        return false;
    }

    vx = vx / distXZ;//�P�ʃx�N�g����
    vz = vz / distXZ;//�P�ʃx�N�g����
    outPositionB.x = positionA.x + (vx * radius);// A����P�ʃx�N�g�������ɔ��a�̍��v�������ړ�����
    outPositionB.y = positionB.y;
    outPositionB.z = positionA.z + (vz * radius);// A����P�ʃx�N�g�������ɔ��a�̍��v�������ړ�����

    return true;
}

bool Collision::IntersectSphereVsCylinderOut(
    const DirectX::XMFLOAT3& spherePosition,
    float sphereRadius,
    const DirectX::XMFLOAT3& cylinderPosition,
    float cylinderRadius, float cylinderHeight,
    DirectX::XMFLOAT3& outCylinderPosition)
{
    // y�������蔻��
    if (spherePosition.y - sphereRadius > cylinderPosition.y + cylinderHeight)return false;
    if (spherePosition.y + sphereRadius < cylinderPosition.y)return false;

    // xz���ʓ����蔻��
    float vx = cylinderPosition.x - spherePosition.x;//�x�N�g�������߂�
    float vz = cylinderPosition.z - spherePosition.z;//�x�N�g�������߂�
    float radius = cylinderRadius + sphereRadius;//���a�̍��v�����߂�
    float distXZ = sqrtf(vx * vx + vz * vz);//���������߂�
    if (distXZ > radius)//��ׂ�
    {
        return false;
    }

    // �����~���������o��
    vx = vx / distXZ;//�P�ʃx�N�g����
    vz = vz / distXZ;//�P�ʃx�N�g����
    outCylinderPosition.x = spherePosition.x + (vx * radius);// A����P�ʃx�N�g�������ɔ��a�̍��v�������ړ�����
    outCylinderPosition.y = cylinderPosition.y;
    outCylinderPosition.z = spherePosition.z + (vz * radius);// A����P�ʃx�N�g�������ɔ��a�̍��v�������ړ�����

    return true;
}

bool Collision::IntersectSphereVsCylinder(const DirectX::XMFLOAT3& spherePosition, float sphereRadius,
    const DirectX::XMFLOAT3& cylinderPosition, float cylinderRadius, float cylinderHeight)
{
    // y�������蔻��
    if (spherePosition.y - sphereRadius > cylinderPosition.y + cylinderHeight)return false;
    if (spherePosition.y + sphereRadius < cylinderPosition.y)return false;

    // xz���ʓ����蔻��
    float vx = cylinderPosition.x - spherePosition.x;//�x�N�g�������߂�
    float vz = cylinderPosition.z - spherePosition.z;//�x�N�g�������߂�
    float radius = cylinderRadius + sphereRadius;//���a�̍��v�����߂�
    float distXZ = sqrtf(vx * vx + vz * vz);//���������߂�
    if (distXZ > radius)//��ׂ�
    {
        return false;
    }

    return true;
}
