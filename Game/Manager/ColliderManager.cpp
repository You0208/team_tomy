#include "ColliderManager.h"

#include "Lemur/Collision/Collision.h"

void ColliderManager::Update()
{
    for (size_t i = 0; i < colliders.size(); i++)
    {
        // todo �����������蔻�肵�Ȃ��ꍇ�̓R���e�B�j���[���ď��������
        for (size_t j = i + 1; j < colliders.size(); j++)
        {
            // todo �����������蔻�肵�Ȃ��ꍇ�̓R���e�B�j���[���ď��������

            // todo �����ŗD��x�̔�r������


            DirectX::XMFLOAT3 pos_a = colliders.at(i)->GetPosition();
            DirectX::XMFLOAT3 pos_b = colliders.at(j)->GetPosition();

            Collision::IntersectSphereVsSphereOut(pos_a, colliders.at(i)->GetRadius(),
                pos_b, colliders.at(j)->GetRadius(), pos_b);

            colliders.at(i)->SetPosition(pos_a);
            colliders.at(j)->SetPosition(pos_b);
        }
    }
}
