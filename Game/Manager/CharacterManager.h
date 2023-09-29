// ReSharper disable All
#pragma once
#include "Lemur/Component/GameObject.h"
#include "./Game/Player/Player.h"
#include "./Game/Enemy/Enemy.h"

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
    Player* GetPlayer()const { return player; }

    void SetPlayer(Player* player_) { player = player_; }
    void AddEnemy(Enemy* enemy) { enemies.emplace_back(enemy); }
private:
    Player* player;
    std::vector<Enemy*>enemies;
};