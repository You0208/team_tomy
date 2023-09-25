#pragma once

#include<vector>
#include<set>
#include "DemoEnemy.h"

// �G�l�~�[�}�l�[�W���[
class DemoEnemyManager
{
private:
    DemoEnemyManager() {}
    ~DemoEnemyManager() {}
public:
    // �B��̃C���X�^���X�擾
    static DemoEnemyManager& Instance()
    {
        static DemoEnemyManager instance;
        return instance;
    }

    // �X�V����
    void Update(float elapsedTime);

    // �`�揈��
    void Render(float elapsedTime);

    // �G�l�~�[�o�^
    void Register(DemoEnemy* enemy);

    // �G�l�~�[�S�폜
    void Clear();

    //�f�o�b�O�v���~�e�B�u�`��
    void DrawDebugPrimitive();

    // �G�l�~�[���擾
    int GetEnemyCount() const { return static_cast<int>(enemies.size()); }

    // �G�l�~�[�擾
    DemoEnemy* GetEnemy(int index) { return enemies.at(index); }

    // �G�l�~�[���m�̏Փ˔���
    void CollisionEnemyVsEnemis();

    // �G�l�~�[�폜
    void Remove(DemoEnemy* enemy);

private:
    std::vector<DemoEnemy*> enemies;
    std::set<DemoEnemy*>    removes;
};
