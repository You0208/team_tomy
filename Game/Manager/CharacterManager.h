// ReSharper disable All
#pragma once
#include "Lemur/Component/GameObject.h"

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
    GameObject* GetPlayer()const { return player; }

    void SetPlayer(GameObject* player_) { player = player_; }
    void AddEnemy(GameObject* enemy) { enemies.emplace_back(enemy); }
private:
    GameObject* player;
    std::vector<GameObject*>enemies;
};