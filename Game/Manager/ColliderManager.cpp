#include "ColliderManager.h"

#include "Lemur/Collision/Collision.h"

void ColliderManager::Update()
{
    for (size_t i = 0; i < colliders.size(); i++)
    {
        // todo こいつが当たり判定しない場合はコンティニューって処理を作る
        for (size_t j = i + 1; j < colliders.size(); j++)
        {
            // todo こいつが当たり判定しない場合はコンティニューって処理を作る

            // todo ここで優先度の比較をする


            DirectX::XMFLOAT3 pos_a = colliders.at(i)->GetPosition();
            DirectX::XMFLOAT3 pos_b = colliders.at(j)->GetPosition();

            Collision::IntersectSphereVsSphereOut(pos_a, colliders.at(i)->GetRadius(),
                pos_b, colliders.at(j)->GetRadius(), pos_b);

            colliders.at(i)->SetPosition(pos_a);
            colliders.at(j)->SetPosition(pos_b);
        }
    }
}
