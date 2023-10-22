#pragma once
#include<vector>
#include<set>
#include "../Enemy/Enemy.h"
#include "../Manager/CharacterManager.h"

// エネミーマネージャー
class EnemyManager:public CharacterManager
{
private:
    EnemyManager() {}
    ~EnemyManager() {}
public:
    // 唯一のインスタンス取得
    static EnemyManager& Instance()
    {
        static EnemyManager instance;
        return instance;
    }

    // 更新処理
    void Update(float elapsedTime);

    // 描画処理
    void Render(float elapsedTime);

    // ダメージ描画処理
    void DamageRender();
    // エネミー登録
    void Register(Enemy* enemy);


    // エネミー全削除
    void Clear();

    //デバッグプリミティブ描画
    void DrawDebugPrimitive();

    // エネミー数取得
    int GetEnemyCount() const { return static_cast<int>(enemies.size()); }

    // エネミー取得
    Enemy* GetEnemy(int index) { return enemies.at(index); }

    std::vector<Enemy*> GetEnemies()const { return enemies; }

    // エネミー同士の衝突判定
    void CollisionEnemyVsEnemis();

    // エネミー削除
    void Remove(Enemy* enemy);

    // エネミー当たり履歴消去
    void HitClear();
private:
    std::set<Enemy*>    removes;
    std::vector<Enemy*>enemies;

};
