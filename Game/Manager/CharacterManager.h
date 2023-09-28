// ReSharper disable All
#pragma once
#include "Lemur/Component/GameObject.h"

// �S�L�����N�^�[���i�[�����V���O���g���N���X
//�L�����N�^�[�ǂ����Ńp�����[�^���~�����Ƃ��Ƃ��ɂ������o�R����
//�p�����[�^���擾����
class CharacterManager
{
public:

    static CharacterManager& Instance()
    {
        static CharacterManager character_manager;
        return character_manager;
    }
    GameObject* GetPlayer()const { return player; }

    void SetPlayer(GameObject* player_) { player = player_; }
    void AddEnemy(GameObject* enemy) { enemies.emplace_back(enemy); }
private:
    GameObject* player;
    std::vector<GameObject*>enemies;
};