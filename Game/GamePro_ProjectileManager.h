#pragma once

#include <vector>
#include "GamePro_Projectile.h"

class GamePro_ProjectileManager
{
public:
    GamePro_ProjectileManager();
    ~GamePro_ProjectileManager();
    //�X�V����
    void Update(float elapsedTime);
    //�`�揈��
    void Render(ID3D11DeviceContext* dc, ID3D11PixelShader* replaced_pixel_shader);
    //�f�o�b�O�v���~�e�B�u�`��
    //void Register();
    //�e�ۓo�^
    void Register(GamePro_Projectile* gamepro_projectile);
    //�e�ۑS�폜
    void Clear();
    //�e�ې��擾
    int GetProjectileCount() const { return static_cast<int>(gamepro_projectiles.size()); }
    //�e�ێ擾
    GamePro_Projectile* GetProjectile(int index) { return gamepro_projectiles.at(index); }

private:
    std::vector<GamePro_Projectile*> gamepro_projectiles;
};