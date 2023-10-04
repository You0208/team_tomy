// ReSharper disable All
#pragma once
#include <string>

class Player;

// �擾�X�L���̊��N���X
class BaseSkill
{
public:
    BaseSkill(Player* player,std::string skill_name_) :owner(player),skill_name(skill_name_){}
    ~BaseSkill() = default;

    // �퓬�J�n���Ɉ�񂾂��Ă΂��B
    virtual void Init() = 0;
    // �퓬���Ăё�����B
    virtual void Update() = 0;
    // �퓬�I�����Ɉ�񂾂��Ă΂��B
    virtual void Fin() = 0;

private:
    std::string skill_name;

    Player* owner;
};