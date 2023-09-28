#include "EnemyManager.h"

void EnemyManager::Update(float elapsedTime)
{
    for (Enemy* enemy : enemies)
    {
        enemy->Update(elapsedTime);
    }

    // 破棄処理
    for (Enemy* enemy : removes)
    {
        std::vector<Enemy*>::iterator it = std::find(enemies.begin(), enemies.end(), enemy);
        if (it != enemies.end())
        {
            enemies.erase(it);
        }

        // 削除
        delete enemy;
    }
    // 破棄リストをクリア
    removes.clear();
}

void EnemyManager::Render(float elapsedTime)
{
    for (Enemy* enemy : enemies)
    {
        enemy->Render(elapsedTime);
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
        delete enemy;
    }
    enemies.clear();
}

void EnemyManager::DrawDebugPrimitive()
{
}

void EnemyManager::CollisionEnemyVsEnemis()
{
}

void EnemyManager::Remove(Enemy* enemy)
{
    // 破棄リストに追加
    removes.insert(enemy);
}
