#include "DemoEnemyManager.h"


void DemoEnemyManager::Update(float elapsedTime)
{
    for (DemoEnemy* enemy : enemies)
    {
        enemy->Update(elapsedTime);
    }

    // �j������
    for (DemoEnemy* enemy : removes)
    {
        std::vector<DemoEnemy*>::iterator it = std::find(enemies.begin(), enemies.end(), enemy);
        if (it != enemies.end())
        {
            enemies.erase(it);
        }

        // �폜
        delete enemy;
    }
    // �j�����X�g���N���A
    removes.clear();
}

void DemoEnemyManager::Render(float elapsedTime)
{
    for (DemoEnemy* enemy : enemies)
    {
        enemy->Render(elapsedTime);
    }
}

void DemoEnemyManager::Register(DemoEnemy* enemy)
{
    enemies.emplace_back(enemy);
}

void DemoEnemyManager::Clear()
{
    for (DemoEnemy* enemy : enemies)
    {
        delete enemy;
    }
    enemies.clear();
}

void DemoEnemyManager::DrawDebugPrimitive()
{
}

void DemoEnemyManager::CollisionEnemyVsEnemis()
{
}

void DemoEnemyManager::Remove(DemoEnemy* enemy)
{
    // �j�����X�g�ɒǉ�
    removes.insert(enemy);
}
