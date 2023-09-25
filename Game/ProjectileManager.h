//#pragma once
//
//#include <vector>
//#include "Projectile.h"
//
//////弾マネージャー
////class ProjectileManager
////{
//////    ProjectileManager();
//////    ~ProjectileManager();
//////
//////    //更新処理
//////    void Update(float elapsedTime);
//////
//////    //描画処理
//////    void Render(float elapsedTime);
//////
//////    //デバックプリミティブ描画
//////    void DrawDebugPrimitive();
//////
//////    //弾丸登録
//////    void Register(Projectile* projectile);
//////
//////    //弾丸全削除
//////    void Clear();
//////
//////    //弾丸数取得
//////    int GetProjectileCount() const { return static_cast<int>(P_projectiles.size()); }
//////
//////    //弾丸取得
//////    ProjectilePhysicsComponent* GetProjectile(int index) { return P_projectiles.at(index); }
//////
//////private:
//////    std::vector<ProjectilePhysicsComponent*> P_projectiles;
//////    std::vector<ProjectileGraphicsComponent*> G_projectiles;
//////    std::vector<Projectile*> projectiles;
////};