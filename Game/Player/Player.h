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

        // プレイヤーはスピードの補正を変える
        walk_speed = 0.1f;

        attack_collision_range = 0.5f;
        max_health = 30;

        health = max_health;

        attack_power = 30;

        //speed_power = 30;
        speed_power = 30;// デバッグ用

        // 最初に所持できるスキルは三つ
        //skills.resize(skill_capacity);

        position={ 0.0f,0.0f,5.0f };
    }

    virtual void DebugImgui() override;
    virtual void DrawDebugPrimitive() override;

    void StateMachineInitialize();
    void StateMachineUpdate();
    Nero::Component::AI::StateMachine* GetStateMachine()const { return state_machine; }

    DirectX::XMFLOAT3 GetMoveVec(float input_x, float input_y);

    bool InputMove();

    /*-------------- 入力状態の取得を関数化 --------------*/

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

    // 敵撃破数取得
    int GetKillCount()const { return kill_count; }

    // 与えたダメージ取得
    int GetAddDamage()const { return add_damage; }
    // 与えたダメージリセット
    void ResetAddDamage() { add_damage = 0; }

    // HPが最大HPをオーバーしたら最大HPにする
    void MaxHealthCheck()
    {
        if (health >= max_health)
            health = max_health;
    }

    // 喰らいダメージ補正設定
    void SetDamageCorrection(const float damage_correction_) { damage_correction = damage_correction_; }
    // 喰らいダメージ補正取得
    float GetDamageCorrection() const { return damage_correction ; }

    void CalcSPAttackTime();
    bool can_SP_attack = false;

    
    void SetIsCounter(const bool is_counter_) { is_counter = is_counter_; }
    bool GetIsCounter()const { return is_counter; }

    void SetCanCounter(const bool can_counter_) { can_counter = can_counter_; }
    // カウンター成功
    bool GetCanCounter()const { return can_counter; }

    bool CounterJudge(DirectX::XMFLOAT3 hit_pos);

    void ResetSPAttackTimer() { SP_attack_cool_timer_ms = 0.0f; }
    // 攻撃時の角度補間
    void AttackAngleInterpolation();

    // 特殊攻撃のクールタイム設定
    void SetSPAttackCoolTime_ms(const float cool_time) { base_SP_attack_cool_time_ms = cool_time; }
    // 特殊攻撃のクールタイム取得
    float GetSPAttackCoolTime_ms() const { return base_SP_attack_cool_time_ms; }
private:
    
    //基本の特殊攻撃のクールタイム(秒)
    float base_SP_attack_cool_time_ms = 17.0f;
    // speed_powerの影響を受けた実際のクールタイム(秒)
    // タイマーがこれを超えないと特殊攻撃は使えない
    float SP_attack_cool_time_ms;
    //float base_SP_attack_cool_time_ms = 1.0f;// デバッグ用

    // 特殊攻撃用の計測して値が動くタイマー(秒)
    float SP_attack_cool_timer_ms;

    // 敵を倒した数
    int kill_count = 0;

    // スキル系でいろいろ都合がいいから与えたダメージを変数で保持
    int add_damage = 0;


    // 無敵フレーム
    int invincible_frame = 20;

    // 旋回の速さ
    float turn_speed = 10.0f;

    // カウンター成功したか
    bool is_counter;

    // 特殊攻撃でカウンターを受付してるか
    bool can_counter;

    // スキルの影響でダメージを多く喰らう補正(合計ダメージにかける)
    float damage_correction = 1.0f;

    /* --------スキルによって影響を受ける前のパラメータを保持する---------
     * --------------スキル解除の時に代入パラメータに代入-----------------*/

    // パラメータを保持する
    void RetentionParamSet();
    // 保持したパラメータをセットする
    void RetentionParamGet();

    // スキル無し攻撃力
    float retention_basicAP;
    // スキル無しスピード
    float retention_basicSP;
    // スキル無し防御力
    float retention_basicDP;
    // スキル無しMaxHP
    int retention_basicMHP;

    // 攻撃時の角度補正してくれる距離
    float can_attack_interpolation_length = 7.0f;

public:
    // 一回初期設定したらtrue。
    bool is_initialize = false;


    int down_count = 2;

    // todo ほかのパラメータも作る
    // かける攻撃力のポイント量
    float bet_AP;
    // かける防御力のポイント量
    float bet_DP;
    // かけるHPのポイント量
    float bet_MHP;
    // かけるスピードのポイント量
    float bet_SP;

    // ベットしたステータスの合計値
    int total_point;

    // 死亡アニメーション終了フラグ
    // 終了したらシーン側でシーン切り替え。
    bool death_anim_end;

public:/*---------- 当たり判定系 ----------*/

    // ノードとエネミーの衝突処理
    void CollisionNodeVsEnemies(const char* mesh_name, const char* bone_name, float node_radius);

    // 無敵状態(喰らい当たり判定するか)
    bool invincible = false;

    int GetInvincibleFrame()const { return invincible_frame; }

public:/*----------------- スキル関係 -----------------*/

    // スキルを設定
    template<class Skill>
    void SetSkill(const wchar_t* ui_spr_filename)
    {
        Skill* skill = new Skill(ui_spr_filename);
        all_skills.emplace_back(skill);
    }

    // 最初にゲームの全スキルをセットしたらtrueにする。
    bool is_set_allSkill = false;

    // 特定のスキルをテストしたいときに使う
    void TestSkillSet(const char* set_skill_name)
    {
        BaseSkill* skill = nullptr;
        for (auto& s : all_skills)
        {
            if (s->GetName() == set_skill_name)
                skill = s.get();
        }
        _ASSERT_EXPR(skill, L"テストしたいスキルが全スキル配列に設定されてません");

        SetSkill(skill);
    }

    // プレイヤーにスキルをセット
    void SetSkill(BaseSkill* skill)
    {
        skill->SetOwner(this);
        skills.emplace_back(skill);
    }
    // プレイヤーにスキルをセット(配列ごとセット)
    void SetSkill(std::vector<BaseSkill*> skills_)
    {
        skills = skills_;
        for (auto& skill : skills)
        {
            skill->SetOwner(this);
        }
    }

    // 所持スキルのInitを呼ぶ
    void SkillInit();
    // 所持スキルのupdateを呼ぶ
    void SkillUpdate();
    // スキルをリセット(中でスキルによって増えた値もリセット)
    void SkillFin();

    // 所持できるスキル数
    int skill_capacity = 3;

    // スキルの配列の並びを入れ替える(スキルの関数を呼ぶ順番を変えるために)
    void SkillSort()
    {
        // こういう書き方で関数を使ってソートできる。
        //参照 https://qiita.com/threecups/items/aa1923a9922dc0a7abfe
        auto fcomp = [](BaseSkill*& a, BaseSkill*& b) {return a->priorty < b->priorty; };
        std::sort(skills.begin(), skills.end(), fcomp);
    }

    // ゲームに存在する全スキル
    std::vector<std::unique_ptr<BaseSkill>> all_skills;

    // 引数のスキル持ってるか検索する関数
    bool HaveSkill(const char* search_skill_name);

    /*--------- デバッグ用 ----------*/
    float skill_ui_offset_y = 120;

    float skill_ui_scale = 0.5f;
private:
    // 所持してるスキル
    std::vector<BaseSkill*> skills;


private:
    Nero::Component::AI::StateMachine* state_machine = nullptr;

public:/*----------- エフェクト関係 ------------*/

    // 斬撃エフェクト
    std::unique_ptr<Effect> slash;
    // パリィ成功時のエフェクト
    std::unique_ptr<Effect> parry_spark;
    // パリィ成功時の斬撃エフェクト
    std::unique_ptr<Effect> parry_slash;

public:/*--------------- UI関係 ----------------*/
    // ダメージ表示
    std::unique_ptr<sprite>spr_damage;

    void UIRender();

private:
    // 所持スキルのUI描画
    void SkillUIRender();

    // 与ダメージの描画
    void DamageRender(DirectX::XMFLOAT3 hit_pos);
};

//こいつらは実体にはならない。コンポーネントとして実体になるやつに搭載される。
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