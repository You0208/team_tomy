#pragma once
#include "Game/Player/Player.h"
#include "Game/Skill/BaseSkill.h"
#include "Lemur/Scene/BaseScene.h"

// スキル配布、敵クエスト択、ステータスかける
//をするシーン
class GambleScene :
    public Lemur::Scene::BaseScene
{
public:
    GambleScene() {}
    ~GambleScene() override {}

    // 初期化
    void Initialize()override;

    // 終了化
    void Finalize()override;

    // 更新処理
    void Update(HWND hwnd, float elapsedTime)override;

    // 描画処理
    void Render(float elapsedTime)override;


    enum Phase
    {
        Skill,
        Quest,
        Gamble,
    };
    int step;


private:
    Player* CreatePlayer()
    {
        return new Player(
            new PlayerInputComponent(),
            new PlayerPhysicsComponent(),
            new PlayerGraphicsComponent()
        );
    }

private:/*---------------- スキル関係 -----------------*/

    // ゲームに存在する全スキル
    std::vector<std::unique_ptr<BaseSkill>> all_skills;

    // スキルを設定
    template<class Skill>
    void SetSkill()
    {
        Skill* skill = new Skill();
        all_skills.emplace_back(skill);
    }

    // 全スキルからランダムでスキルを取得
    void SetPlayerSkills();

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

        player->SetSkill(skill);
    }

    Player* player;
};

