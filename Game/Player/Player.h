#pragma once
#include "Lemur/Input/Input.h"
#include "..\Skill\BaseSkill.h"
#include "Game/StateMachine/StateDerived.h"
#include "Lemur/Component/GameObject.h"
//#include "Game/StateMachine/StateMachine.h"

namespace Nero::Component::AI
{
    class StateMachine;
}

class Player:public GameObject
{
public:
    enum AnimIndex
    {
        FirstAttack_Anim,
        SecondAttack_Anim,
        ThirdAttack_Anim,

        Run_Anim,
        Idle_Anim,
        Avoid_Anim,
        Death_Anim,
        Fear_Anim,

        Max_Anim
    };

    enum StateIndex
    {
        Idle_State,
        Run_State,
        Avoid_State,
        Attack_State,
        Death_State,
        Fear_State,

        Max_State,
    };
public:
    Player(){}
    Player(InputComponent* input_,
        PhysicsComponent* physics_,
        GraphicsComponent* graphics_) :GameObject(input_, physics_, graphics_)
    {
        height = 1.7f;
        radius = 0.4f;

        // �����̑���(�X�s�[�h�p�����[�^�����邩��x�[�X�̑��x�͌�����)
        walk_speed = 0.1f;

        attack_power = 50;


        // �ŏ��ɏ����ł���X�L���͎O��
        //skills.resize(skill_capacity);
    }

    virtual void DebugImgui() override;
    virtual void DrawDebugPrimitive() override;

    void StateMachineInitialize();
    void StateMachineUpdate();
    Nero::Component::AI::StateMachine* GetStateMachine()const { return state_machine; }

    bool InputMove();

    // �����̃X�L�������Ă邩��������֐�
    bool HaveSkill(const char* search_skill_name);
    /*-------------- ���͏�Ԃ̎擾���֐��� --------------*/

    bool GetButtonDownB_AND_MouseLeft()
    {
        GamePad& game_pad = Input::Instance().GetGamePad();
        if (game_pad.GetButtonDown() & GamePad::BTN_B)
            return true;

        Mouse& mouse = Input::Instance().GetMouse();
        if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
            return true;
    }

    // �G���j���擾
    int GetKillCount()const { return kill_count; }

    // �^�����_���[�W�擾
    int GetAddDamage()const { return add_damage; }
    // �^�����_���[�W���Z�b�g
    void ResetAddDamage() { add_damage = 0; }

    // HP���ő�HP���I�[�o�[������ő�HP�ɂ���
    void MaxHealthCheck()
    {
        if (health >= max_health)
            health = max_health;
    }

private:

    // �G��|������
    int kill_count = 0;

    // �X�L���n�ł��낢��s������������^�����_���[�W��ϐ��ŕێ�
    int add_damage = 0;

public:/*---------- �����蔻��n ----------*/

    // �m�[�h�ƃG�l�~�[�̏Փˏ���
    void CollisionNodeVsEnemies(const char* mesh_name,const char* bone_name, float node_radius);

    // ���G���(��炢�����蔻�肷�邩)
    bool invincible = false;

    int GetInvincibleFrame()const { return invincible_frame; }
private:

    // ���G�t���[��
    int invincible_frame = 20;

private:/*---------- �ړ��A���x�֌W -----------*/

    DirectX::XMFLOAT3 GetMoveVec(float input_x, float input_y);

private:

    // ����̑���
    float turn_speed = 10.0f;
public:
    // �X�s�[�h�p�����[�^
    float speed_power = 10.0f;

private:
    Nero::Component::AI::StateMachine* state_machine = nullptr;

public:/*----------------- �X�L���֌W -----------------*/
    // �v���C���[�ɃX�L�����Z�b�g
    void SetSkill(BaseSkill* skill)
    {
        skill->SetOwner(this);
        skills.emplace_back(skill);
    }
    // �����X�L����Init���Ă�
    void SkillInit();
    // �����X�L����update���Ă�
    void SkillUpdate();
    // �X�L�������Z�b�g
    void SkillFin();

    // �����ł���X�L����
    int skill_capacity = 3;

    // �X�L���̔z��̕��т����ւ���(�X�L���̊֐����Ăԏ��Ԃ�ς��邽�߂�)
    void SkillSort()
    {
        // ���������������Ŋ֐����g���ă\�[�g�ł���B
        //�Q�� https://qiita.com/threecups/items/aa1923a9922dc0a7abfe
        auto fcomp = [](BaseSkill*& a, BaseSkill*& b) {return a->priorty < b->priorty; };
        std::sort(skills.begin(), skills.end(), fcomp);
    }
private:
    // �������Ă�X�L��
    std::vector<BaseSkill*> skills;


};

//������͎��̂ɂ͂Ȃ�Ȃ��B�R���|�[�l���g�Ƃ��Ď��̂ɂȂ��ɓ��ڂ����B
class PlayerInputComponent :public InputComponent
{
    void Initialize(GameObject* gameobj) override {}
    void Update(GameObject* gameobj, float elapsedTime) override;

};

class PlayerPhysicsComponent :public PhysicsComponent
{
    void Initialize(GameObject* gameobj) override;
    void Update(GameObject* gameobj, float elapsedTime) override;
};

class PlayerGraphicsComponent :public GraphicsComponent
{
    void Initialize(GameObject* gameobj) override;
    void Update(GameObject* gameobj) override;
    void Render(GameObject* gameobj, float elapsedTime, ID3D11PixelShader* replaced_pixel_shader) override;
};