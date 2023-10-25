#pragma once
#include "Lemur/Input/Input.h"
#include "..\Skill\BaseSkill.h"
#include "Game/Enemy/Enemy.h"
#include "Game/StateMachine/StateDerived.h"
#include "Lemur/Component/GameObject.h"
#include "Lemur/Effekseer/Effect.h"
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

        Counter_Anim,
        CounterAttack_Anim,
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
        SPAttack_State,

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

        // �v���C���[�̓X�s�[�h�̕␳��ς���
        walk_speed = 0.1f;

        attack_collision_range = 0.5f;
        max_health = 20;
        max_health = 1000;
        health = max_health;

        attack_power = 20;

        //speed_power = 30;
        speed_power = 100;// �f�o�b�O�p

        // �ŏ��ɏ����ł���X�L���͎O��
        //skills.resize(skill_capacity);
    }

    virtual void DebugImgui() override;
    virtual void DrawDebugPrimitive() override;

    void StateMachineInitialize();
    void StateMachineUpdate();
    Nero::Component::AI::StateMachine* GetStateMachine()const { return state_machine; }

    DirectX::XMFLOAT3 GetMoveVec(float input_x, float input_y);

    bool InputMove();

    /*-------------- ���͏�Ԃ̎擾���֐��� --------------*/

    bool GetButtonDownB_AND_MouseLeft()
    {
        GamePad& game_pad = Input::Instance().GetGamePad();
        if (game_pad.GetButtonDown() & GamePad::BTN_B)
            return true;

        Mouse& mouse = Input::Instance().GetMouse();
        if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
            return true;

        return false;
    }
    bool GetButtonDownY_AND_MouseRight()
    {
        GamePad& game_pad = Input::Instance().GetGamePad();
        if (game_pad.GetButtonDown() & GamePad::BTN_Y)
            return true;

        Mouse& mouse = Input::Instance().GetMouse();
        if (mouse.GetButtonDown() & Mouse::BTN_RIGHT)
            return true;

        return false;
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

    // ��炢�_���[�W�␳�ݒ�
    void SetDamageCorrection(const float damage_correction_) { damage_correction = damage_correction_; }
    // ��炢�_���[�W�␳�擾
    float GetDamageCorrection() const { return damage_correction ; }

    void CalcSPAttackTime();
    bool can_SP_attack = false;

    
    void SetIsCounter(const bool is_counter_) { is_counter = is_counter_; }
    bool GetIsCounter()const { return is_counter; }

    void SetCanCounter(const bool can_counter_) { can_counter = can_counter_; }
    // �J�E���^�[����
    bool GetCanCounter()const { return can_counter; }

    bool CounterJudge(DirectX::XMFLOAT3 hit_pos);

    void ResetSPAttackTimer() { SP_attack_cool_timer_ms = 0.0f; }
    // �U�����̊p�x���
    void AttackAngleInterpolation();

    // ����U���̃N�[���^�C���ݒ�
    void SetSPAttackCoolTime_ms(const float cool_time) { SP_attack_cool_time_ms = cool_time; }
    // ����U���̃N�[���^�C���擾
    float GetSPAttackCoolTime_ms() const { return SP_attack_cool_time_ms; }
private:
    
    //����U���̃N�[���^�C��(�b)
    // �^�C�}�[������𒴂��Ȃ��Ɠ���U���͎g���Ȃ�
    //float SP_attack_cool_time_ms = 15.0f;
    float SP_attack_cool_time_ms = 1.0f;// �f�o�b�O�p

    // ����U���p�̌v�����Ēl�������^�C�}�[(�b)
    float SP_attack_cool_timer_ms;

    // �G��|������
    int kill_count = 0;

    // �X�L���n�ł��낢��s������������^�����_���[�W��ϐ��ŕێ�
    int add_damage = 0;


    // ���G�t���[��
    int invincible_frame = 20;

    // ����̑���
    float turn_speed = 10.0f;

    // �J�E���^�[����������
    bool is_counter;

    // ����U���ŃJ�E���^�[����t���Ă邩
    bool can_counter;

    // �X�L���̉e���Ń_���[�W�𑽂���炤�␳(���v�_���[�W�ɂ�����)
    float damage_correction = 1.0f;

    /* --------�X�L���ɂ���ĉe�����󂯂�O�̃p�����[�^��ێ�����---------
     * --------------�X�L�������̎��ɑ���p�����[�^�ɑ��-----------------*/

    // �p�����[�^��ێ�����
    void RetentionParamSet();
    // �ێ������p�����[�^���Z�b�g����
    void RetentionParamGet();

    // �X�L�������U����
    float retention_basicAP;
    // �X�L�������X�s�[�h
    float retention_basicSP;
    // �X�L�������h���
    float retention_basicDP;
    // �X�L������MaxHP
    int retention_basicMHP;

    // �U�����̊p�x�␳���Ă���鋗��
    float can_attack_interpolation_length = 7.0f;

public:
    // ��񏉊��ݒ肵����true�B
    bool is_initialize = false;


    int down_count = 2;

    // todo �ق��̃p�����[�^�����
    // ������U���͂̃|�C���g��
    float bet_AP;
    // ������h��͂̃|�C���g��
    float bet_DP;
    // ������HP�̃|�C���g��
    float bet_MHP;
    // ������X�s�[�h�̃|�C���g��
    float bet_SP;

    // �x�b�g�����X�e�[�^�X�̍��v�l
    int total_point;

    // ���S�A�j���[�V�����I���t���O
    // �I��������V�[�����ŃV�[���؂�ւ��B
    bool death_anim_end;

public:/*---------- �����蔻��n ----------*/

    // �m�[�h�ƃG�l�~�[�̏Փˏ���
    void CollisionNodeVsEnemies(const char* mesh_name, const char* bone_name, float node_radius);

    // ���G���(��炢�����蔻�肷�邩)
    bool invincible = false;

    int GetInvincibleFrame()const { return invincible_frame; }

public:/*----------------- �X�L���֌W -----------------*/

    // �X�L����ݒ�
    template<class Skill>
    void SetSkill(const wchar_t* ui_spr_filename)
    {
        Skill* skill = new Skill(ui_spr_filename);
        all_skills.emplace_back(skill);
    }

    // �ŏ��ɃQ�[���̑S�X�L�����Z�b�g������true�ɂ���B
    bool is_set_allSkill = false;

    // ����̃X�L�����e�X�g�������Ƃ��Ɏg��
    void TestSkillSet(const char* set_skill_name)
    {
        BaseSkill* skill = nullptr;
        for (auto& s : all_skills)
        {
            if (s->GetName() == set_skill_name)
                skill = s.get();
        }
        _ASSERT_EXPR(skill, L"�e�X�g�������X�L�����S�X�L���z��ɐݒ肳��Ă܂���");

        SetSkill(skill);
    }

    // �v���C���[�ɃX�L�����Z�b�g
    void SetSkill(BaseSkill* skill)
    {
        skill->SetOwner(this);
        skills.emplace_back(skill);
    }
    // �v���C���[�ɃX�L�����Z�b�g(�z�񂲂ƃZ�b�g)
    void SetSkill(std::vector<BaseSkill*> skills_)
    {
        skills = skills_;
        for (auto& skill : skills)
        {
            skill->SetOwner(this);
        }
    }

    // �����X�L����Init���Ă�
    void SkillInit();
    // �����X�L����update���Ă�
    void SkillUpdate();
    // �X�L�������Z�b�g(���ŃX�L���ɂ���đ������l�����Z�b�g)
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

    // �Q�[���ɑ��݂���S�X�L��
    std::vector<std::unique_ptr<BaseSkill>> all_skills;

    // �����̃X�L�������Ă邩��������֐�
    bool HaveSkill(const char* search_skill_name);

    /*--------- �f�o�b�O�p ----------*/
    float skill_ui_offset_y = 120;

    float skill_ui_scale = 0.5f;
private:
    // �������Ă�X�L��
    std::vector<BaseSkill*> skills;


private:
    Nero::Component::AI::StateMachine* state_machine = nullptr;

public:/*----------- �G�t�F�N�g�֌W ------------*/

    // �a���G�t�F�N�g
    std::unique_ptr<Effect> slash;
    // �p���B�������̃G�t�F�N�g
    std::unique_ptr<Effect> parry_spark;
    // �p���B�������̎a���G�t�F�N�g
    std::unique_ptr<Effect> parry_slash;

public:/*--------------- UI�֌W ----------------*/
    // �_���[�W�\��
    std::unique_ptr<sprite>spr_damage;

    void UIRender();

private:
    // �����X�L����UI�`��
    void SkillUIRender();

    // �^�_���[�W�̕`��
    void DamageRender(DirectX::XMFLOAT3 hit_pos);
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
    void Render(GameObject* gameobj, float elapsedTime, ID3D11PixelShader* replaced_pixel_shader, bool shadow = false) override;
};