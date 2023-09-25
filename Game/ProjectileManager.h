//#pragma once
//
//#include <vector>
//#include "Projectile.h"
//
//////�e�}�l�[�W���[
////class ProjectileManager
////{
//////    ProjectileManager();
//////    ~ProjectileManager();
//////
//////    //�X�V����
//////    void Update(float elapsedTime);
//////
//////    //�`�揈��
//////    void Render(float elapsedTime);
//////
//////    //�f�o�b�N�v���~�e�B�u�`��
//////    void DrawDebugPrimitive();
//////
//////    //�e�ۓo�^
//////    void Register(Projectile* projectile);
//////
//////    //�e�ۑS�폜
//////    void Clear();
//////
//////    //�e�ې��擾
//////    int GetProjectileCount() const { return static_cast<int>(P_projectiles.size()); }
//////
//////    //�e�ێ擾
//////    ProjectilePhysicsComponent* GetProjectile(int index) { return P_projectiles.at(index); }
//////
//////private:
//////    std::vector<ProjectilePhysicsComponent*> P_projectiles;
//////    std::vector<ProjectileGraphicsComponent*> G_projectiles;
//////    std::vector<Projectile*> projectiles;
////};