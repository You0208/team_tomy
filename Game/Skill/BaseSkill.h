// ReSharper disable All
#pragma once
#include <string>

class Player;

// �擾�X�L���̊��N���X
class BaseSkill
{
public:
    // �v���C���[���V�[���̑S�X�L���z�񂩂�X�L�����擾����^�C�~���O�ŁA�I�[�i�[���Z�b�g����B
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

    Player* GetOwner()const { return owner; }

    // ���O�擾
    std::string GetName()const { return skill_name; }

    bool is_select = false;
protected:
    std::string skill_name;

    Player* owner;
};

// 1.���r(�U���͂�1.25�{�オ��)
// 2.�S��(�G��|���ƍU���͂̃X�e�[�^�X��50�オ��)
// 3.����(�A���̃_���[�W���オ��)
// 4.�c��(�A������x�ɍU���͂��オ��)
// 5.���Q(HP�����Ȃ����ɍU���͂��オ��)
// 6.����(�^����_���[�W���オ�邪�󂯂�_���[�W���オ��)
// 7.����U���̃_���[�W���オ�邪�A�N�[���^�C�����L�т�
// 8.����(�ʏ�U���̃_���[�W���オ��)
// 9.�z��(�G�Ƀ_���[�W��^�����HP��)
// 10.�A��(�_���[�W���󂯂��ɓG���U�����邱�ƂŃR���{���q����_���[�W���オ��)
// 12.����(���Ԃ��o���тɃX�s�[�h���オ��)
// 15.�x�e(�퓬�I����R�O % �񕜂���)
// 16.�t�](�G��|�����R�O�b�Ԃ����X�e�[�^�X���S�̓I�ɏオ��)
// 17.��m(�����Ă��q�����X�e�[�^�X������Ȃ�)
// 18.�_��(�o�b�g�X�L���̌��ʂ𖳌��ɂ���)
// 19.�D��(�o�b�g�X�L���ȊO�̃X�L���̌��ʂ��Q�{�ɂȂ�)
// 20.���^(�킢�ɏ��������ꍇ1.5�{��V���オ��)
// 21.���l(�퓬���X�e�[�^�X��2�{�ɂȂ�)

// 2.���f(������HP�������Ă������U���͂����Ԍo�߂ŏオ��(�o�b�g�X�L��))
// 3.�s�K(�����Ă�1.1�{���������Ȃ�(�o�b�g�X�L��))
// 4.�Ӗ�(�I������G�̔{���������Ȃ�(�o�b�g�X�L��))
// 5.�ݑ�(�ړ��X�s�[�h���x���Ȃ�(�o�b�g�X�L��))
// 6.����(���Ԍo�߂ɂ��U���͂�������)
// 7.�ӑ�(�X�e�[�^�X���Ⴂ��Ԃ���n�܂�A���Ԍo�߂ŃX�e�[�^�X���߂�)
// 8.���i(HP���U���͂�荂���ꍇHP���U���͂Ɠ����X�e�[�^�X�ɉ�����B
// 9.�،�(�U���͂�������)
// 10.�s��(�o�b�g�X�L���̌��ʂ��Q�{�ɂȂ�)

// 1.��(�����N����Ȃ�)
// 2.�Ԕ�(�퓬BGM�������ȂɂȂ�)
// 3.����(�����S�Ė����Ȃ�)
// 4.����(��l������������)
// 5.����(�a���̉������̐��ɂȂ�)
// 6.�L��(���ʂƍL���������)
