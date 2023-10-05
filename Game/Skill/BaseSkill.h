// ReSharper disable All
#pragma once
#include <string>

class Player;

// �擾�X�L���̊��N���X
class BaseSkill
{
public:
    // �v���C���[���V�[���̑S�X�L���z�񂩂�擾����^�C�~���O�ŃI�[�i�[���Z�b�g����B
    BaseSkill(std::string skill_name_) :skill_name(skill_name_){}

    //BaseSkill(Player* player,std::string skill_name_) :owner(player),skill_name(skill_name_){}

    ~BaseSkill() = default;

    // �퓬�J�n���Ɉ�񂾂��Ă΂��B
    virtual void Init(){}
    // �퓬���Ăё�����B
    virtual void Update(){}
    // �퓬�I�����Ɉ�񂾂��Ă΂��B
    virtual void Fin(){}

    // �v���C���[���X�L�����擾����^�C�~���O�ŃI�[�i�[���Z�b�g
    void SetOwner(Player* player) { owner = player; }

    // ���O�擾
    std::string GetName()const { return skill_name; }

protected:
    std::string skill_name;

    Player* owner;
};