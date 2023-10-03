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

private:
    std::string skill_name;

    Player* owner;
};