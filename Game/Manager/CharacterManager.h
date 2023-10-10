// ReSharper disable All
#pragma once
#include "Lemur/Component/GameObject.h"
#include "./Game/Player/Player.h"
#include "./Game/Enemy/Enemy.h"

// 全キャラクターを格納したシングルトンクラス
//キャラクターどうしでパラメータが欲しいときとかにこいつを経由して
//パラメータを取得する
class CharacterManager
{
public:
    static CharacterManager& Instance()
    {
        static CharacterManager character_manager;
        return character_manager;
    }
    Player* GetPlayer()const { return player; }

    std::vector<Enemy*> GetEnemies()const { return enemies; }


    void SetPlayer(Player* player_) { player = player_; }
    void AddEnemy(Enemy* enemy) { enemies.emplace_back(enemy); }

    void CollisionAllCharacter();
protected:
    Player* player;
    std::vector<Enemy*>enemies;
};
