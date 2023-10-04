// ReSharper disable All
#pragma once
#include <string>

class Player;

// 取得スキルの基底クラス
class BaseSkill
{
public:
    BaseSkill(Player* player,std::string skill_name_) :owner(player),skill_name(skill_name_){}
    ~BaseSkill() = default;

    // 戦闘開始時に一回だけ呼ばれる。
    virtual void Init() = 0;
    // 戦闘中呼び続ける。
    virtual void Update() = 0;
    // 戦闘終了時に一回だけ呼ばれる。
    virtual void Fin() = 0;

private:
    std::string skill_name;

    Player* owner;
};