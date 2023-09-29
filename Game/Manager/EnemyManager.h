#pragma once
#include<vector>
#include<set>
#include "../Enemy/Enemy.h"
#include "../Manager/CharacterManager.h"

// �G�l�~�[�}�l�[�W���[
class EnemyManager:public CharacterManager
{
private:
    EnemyManager() {}
    ~EnemyManager() {}
public:
    // �B��̃C���X�^���X�擾
    static EnemyManager& Instance()
    {
        static EnemyManager instance;
        return instance;
    }

    // �X�V����
    void Update(float elapsedTime);

    // �`�揈��
    void Render(float elapsedTime);

    // �G�l�~�[�o�^
    void Register(Enemy* enemy);

    // �G�l�~�[�S�폜
    void Clear();

    //�f�o�b�O�v���~�e�B�u�`��
    void DrawDebugPrimitive();

    // �G�l�~�[���擾
    int GetEnemyCount() const { return static_cast<int>(enemies.size()); }

    // �G�l�~�[�擾
    Enemy* GetEnemy(int index) { return enemies.at(index); }

    // �G�l�~�[���m�̏Փ˔���
    void CollisionEnemyVsEnemis();

    // �G�l�~�[�폜
    void Remove(Enemy* enemy);

private:
    std::set<Enemy*>    removes;
};
