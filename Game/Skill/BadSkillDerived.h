// ReSharper disable All
#pragma once
#include "BaseSkill.h"

// 1.豆腐(一撃でもダメージを受けてしまうと死んでしまう(バットスキル))
class Tofu :
    public BaseSkill
{
public:
    Tofu() :BaseSkill("Tofu"){}
    ~Tofu() = default;

    // 名前検索で使うからここには処理書かない
};

// 2.呪詛(自動でHPが減っていくが攻撃力が時間経過で上がる(バットスキル))
class Curse :public BaseSkill
{
public:
    Curse():BaseSkill("Curse"){}
    
    void Update() override;
};