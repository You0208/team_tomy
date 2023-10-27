#include "EnemyManager.h"

#include "Lemur/Graphics/Camera.h"

void EnemyManager::Update(float elapsedTime)
{
    for (Enemy* enemy : enemies)
    {
        enemy->Update(elapsedTime);
    }

    //// 消す前に何体いるか保持
    //int enemy_count = enemies.size();

    // 破棄処理
    for (Enemy* enemy : removes)
    {
        std::vector<Enemy*>::iterator it = std::find(enemies.begin(), enemies.end(), enemy);
        if (it != enemies.end())
        {
            enemies.erase(it);

            //// 消した分だけカウントを減らす
            //enemy_count--;
        }
        if (Camera::Instance().GetLockONEnemy() == enemy)
            Camera::Instance().SetLockONEnemy(nullptr);
        // 削除
        delete enemy;

    }
    // 破棄リストをクリア
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
        enemy->DrawDebugPrimitive();
    }
}


void EnemyManager::CollisionEnemyVsEnemis()
{
}

void EnemyManager::Remove(Enemy* enemy)
{
    // 破棄リストに追加
    removes.insert(enemy);
}

void EnemyManager::HitClear()
{
    for (Enemy* enemy : enemies)
    {
        enemy->is_hit = false;
    }

}
