#include "EnemyManager.h"

#include "Lemur/Graphics/Camera.h"

void EnemyManager::Update(float elapsedTime)
{
    for (Enemy* enemy : enemies)
    {
        enemy->Update(elapsedTime);
    }

    //// �����O�ɉ��̂��邩�ێ�
    //int enemy_count = enemies.size();

    // �j������
    for (Enemy* enemy : removes)
    {
        std::vector<Enemy*>::iterator it = std::find(enemies.begin(), enemies.end(), enemy);
        if (it != enemies.end())
        {
            enemies.erase(it);

            //// �������������J�E���g�����炷
            //enemy_count--;
        }
        if (Camera::Instance().GetLockONEnemy() == enemy)
            Camera::Instance().SetLockONEnemy(nullptr);
        // �폜
        delete enemy;

    }
    // �j�����X�g���N���A
    removes.clear();

    
    //enemies.resize(enemy_count);

}

void EnemyManager::Render(float elapsedTime,bool shadow)
{
    for (Enemy* enemy : enemies)
    {
        enemy->Render(elapsedTime,shadow);
    }
}

void EnemyManager::DamageRender()
{
    for (Enemy* enemy : enemies)
    {
        enemy->DamageRender();
    }

}

void EnemyManager::Register(Enemy* enemy)
{
    enemies.emplace_back(enemy);
}

void EnemyManager::Clear()
{
    for (Enemy* enemy : enemies)
    {
        enemy->BehaviorTreeClear();
        enemy->Delete();
        delete enemy;
    }
    enemies.clear();
}

void EnemyManager::DrawDebugPrimitive()
{
    for(Enemy* enemy:enemies)
    {
       // enemy->DrawDebugPrimitive();
    }
}


void EnemyManager::CollisionEnemyVsEnemis()
{
}

void EnemyManager::Remove(Enemy* enemy)
{
    // �j�����X�g�ɒǉ�
    removes.insert(enemy);
}

void EnemyManager::HitClear()
{
    for (Enemy* enemy : enemies)
    {
        enemy->is_hit = false;
    }

}
