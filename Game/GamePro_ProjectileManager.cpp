#include "GamePro_ProjectileManager.h"

GamePro_ProjectileManager::GamePro_ProjectileManager()
{
}

GamePro_ProjectileManager::~GamePro_ProjectileManager()
{
    Clear();
}

void GamePro_ProjectileManager::Update(float elapsedTime)
{
    for (GamePro_Projectile* gamepro_projectile : gamepro_projectiles)
    {
        gamepro_projectile->Update(elapsedTime);
    }
}

void GamePro_ProjectileManager::Render(ID3D11DeviceContext* dc, ID3D11PixelShader* replaced_pixel_shader)
{
    for (GamePro_Projectile* gamepro_projectile : gamepro_projectiles)
    {
        gamepro_projectile->Render(dc,replaced_pixel_shader);
    }
}

//void GamePro_ProjectileManager::Register()

void GamePro_ProjectileManager::Register(GamePro_Projectile* gamepro_projectile)
{
    gamepro_projectiles.emplace_back(gamepro_projectile);
}

void GamePro_ProjectileManager::Clear()
{
    for (GamePro_Projectile* gamepro_projectile : gamepro_projectiles)
    {
        delete gamepro_projectile;
    }
    gamepro_projectiles.clear();
}
