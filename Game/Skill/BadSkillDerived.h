// ReSharper disable All
#pragma once
#include "BaseSkill.h"

// 豆腐(一撃でもダメージを受けてしまうと死んでしまう(バットスキル))
class Tofu :
    public BaseSkill
{
public:
    Tofu() :BaseSkill("Tofu"){}
    ~Tofu() = default;

    // 名前検索で使うからここには処理書かない

};

