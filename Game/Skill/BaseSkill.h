// ReSharper disable All
#pragma once
#include <memory>
#include <string>

#include "Lemur/Graphics/Graphics.h"
#include "Lemur/Graphics/sprite.h"

class Player;

// �擾�X�L���̊��N���X
class BaseSkill
{
public:
    // �v���C���[���V�[���̑S�X�L���z�񂩂�X�L�����擾����^�C�~���O�ŁA�I�[�i�[���Z�b�g����B
    BaseSkill(std::string skill_name_, const wchar_t* ui_spr_filename,int priorty_ = 0) : skill_name(skill_name_),priorty(priorty_)
    {
        UI_spr = std::make_unique<sprite>(Lemur::Graphics::Graphics::Instance().GetDevice(), ui_spr_filename);
    }

    //BaseSkill(Player* player,std::string skill_name_) :owner(player),skill_name(skill_name_){}

    ~BaseSkill() = default;

    virtual void DrawImGui(){}

    // �퓬�J�n���Ɉ�񂾂��Ă΂��B
    virtual void Init(){}
    // �퓬���Ăё�����B
    virtual void Update(){}
    // �퓬�I�����Ɉ�񂾂��Ă΂��B
    virtual void Fin(){}

    // �v���C���[���X�L�����擾����^�C�~���O�ŃI�[�i�[���Z�b�g
    void SetOwner(Player* player) { owner = player; }

    Player* GetOwner()const { return owner; }

    // ���O�擾
    std::string GetName()const { return skill_name; }

    // �X�L�����I����Ă�t���O
    bool is_select = false;

    //auto GetIT()
    //{
    //    std::iterator<int>
    //}

    // �X�L���̏��Ԃ����ւ��邽�߂Ɏg���B
    //���ԂŐ��\�Ȃǂ����E�����X�L���̂ݐݒ肷��B
    int priorty = 0;

    // �X�L����UI�A�Z�b�g
    std::unique_ptr<sprite> UI_spr;
protected:
    std::string skill_name;

    Player* owner;


};

// 10.�A��(�_���[�W���󂯂��ɓG���U�����邱�ƂŃR���{���q����_���[�W���オ��)
// 17.��m(�����Ă��q�����X�e�[�^�X������Ȃ�)
// 18.�_��(�o�b�g�X�L���̌��ʂ𖳌��ɂ���)
// 19.�D��(�o�b�g�X�L���ȊO�̃X�L���̌��ʂ��Q�{�ɂȂ�)
// 20.���^(�킢�ɏ��������ꍇ1.5�{��V���オ��)


// 3.�s�K(�����Ă�1.1�{���������Ȃ�(�o�b�g�X�L��))
// 4.�Ӗ�(�I������G�̔{���������Ȃ�(�o�b�g�X�L��))
// 5.�ݑ�(�ړ��X�s�[�h���x���Ȃ�(�o�b�g�X�L��))
// 7.�ӑ�(�X�e�[�^�X���Ⴂ��Ԃ���n�܂�A���Ԍo�߂ŃX�e�[�^�X���߂�)
// 8.���i(HP���U���͂�荂���ꍇHP���U���͂Ɠ����X�e�[�^�X�ɉ�����B
// 9.�،�(�U���͂�������)
// 10.�s��(�o�b�g�X�L���̌��ʂ��Q�{�ɂȂ�)

// 2.�Ԕ�(�퓬BGM�������ȂɂȂ�)
// 3.����(�����S�Ė����Ȃ�)
// 4.����(��l������������)
// 5.����(�a���̉������̐��ɂȂ�)
// 6.�L��(���ʂƍL���������)
