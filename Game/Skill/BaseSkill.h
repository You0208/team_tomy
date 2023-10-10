// ReSharper disable All
#pragma once
#include <string>

class Player;

// 取得スキルの基底クラス
class BaseSkill
{
public:
    // プレイヤーがシーンの全スキル配列からスキルを取得するタイミングで、オーナーをセットする。
    BaseSkill(std::string skill_name_) :skill_name(skill_name_){}

    //BaseSkill(Player* player,std::string skill_name_) :owner(player),skill_name(skill_name_){}

    ~BaseSkill() = default;

    // 戦闘開始時に一回だけ呼ばれる。
    virtual void Init(){}
    // 戦闘中呼び続ける。
    virtual void Update(){}
    // 戦闘終了時に一回だけ呼ばれる。
    virtual void Fin(){}

    // プレイヤーがスキルを取得するタイミングでオーナーをセット
    void SetOwner(Player* player) { owner = player; }

    Player* GetOwner()const { return owner; }

    // 名前取得
    std::string GetName()const { return skill_name; }

    bool is_select = false;
protected:
    std::string skill_name;

    Player* owner;
};