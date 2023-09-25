#pragma once

#include <vector>
#include "GamePro_Projectile.h"

class GamePro_ProjectileManager
{
public:
    GamePro_ProjectileManager();
    ~GamePro_ProjectileManager();
    //更新処理
    void Update(float elapsedTime);
    //描画処理
    void Render(ID3D11DeviceContext* dc, ID3D11PixelShader* replaced_pixel_shader);
    //デバッグプリミティブ描画
    //void Register();
    //弾丸登録
    void Register(GamePro_Projectile* gamepro_projectile);
    //弾丸全削除
    void Clear();
    //弾丸数取得
    int GetProjectileCount() const { return static_cast<int>(gamepro_projectiles.size()); }
    //弾丸取得
    GamePro_Projectile* GetProjectile(int index) { return gamepro_projectiles.at(index); }

private:
    std::vector<GamePro_Projectile*> gamepro_projectiles;
};