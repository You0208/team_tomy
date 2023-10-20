// ReSharper disable All
#pragma once
#include "BaseSkill.h"

// 1.豆腐(一撃でもダメージを受けてしまうと死んでしまう(バットスキル))
class Tofu :
    public BaseSkill
{
public:
    Tofu(const wchar_t* ui_spr_filename) :BaseSkill("Tofu",ui_spr_filename){}
    ~Tofu() = default;

    // 名前検索で使うからここには処理書かない
};

// 2.呪詛(自動でHPが減っていくが攻撃力が時間経過で上がる(バットスキル))
class Curse :public BaseSkill
{
public:
    Curse(const wchar_t* ui_spr_filename):BaseSkill("Curse",ui_spr_filename){}
    
    void Update() override;
    float damage_time = 5.0f;
    float damage_timer = 0.0f;
    int damage_value = 50;

};

// 6.傲慢(時間経過により攻撃力が下がる)
class Arrogance :public BaseSkill
{
public:
    Arrogance(const wchar_t* ui_spr_filename):BaseSkill("Arrogance",ui_spr_filename){}
    
    void Update() override;
    float time_ms = 5.0f;
    float timer_ms = 0.0f;
    float value = 5;

};