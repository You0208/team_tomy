#include "GambleScene.h"

#include "Game/Manager/CharacterManager.h"
#include "Game/Skill/AttackSkillDerived.h"
#include "Game/Skill/BadSkillDerived.h"
#include "Game/Skill/SkillDerived.h"
#include "Lemur/Input/Input.h"
#include "Lemur/Scene/SceneManager.h"
#include "SceneGame.h"

void GambleScene::Initialize()
{
    // アセットロード



    // シーンにスキルの設定
	SetSkill<StrongArm>();
	SetSkill<DemonPower>();
	SetSkill<MagicSword>();
	SetSkill<Cruel>();
	SetSkill<Revenge>();
	SetSkill<BloodSucking>();
	SetSkill<Sprint>();
	SetSkill<Acceleration>();
	SetSkill<Patience>();
	SetSkill<Regeneration>();
	SetSkill<SuperMan>();
	SetSkill<SwordSaint>();
	SetSkill<Gale>();
	SetSkill<Obesity>();

	SetSkill<Tofu>();

	// プレイヤーの生成
	player = CreatePlayer();

	// プレイヤーにスキルを取得させる
	SetPlayerSkills();

	// 優先順位でスキルを並び替え(Initとかupdateを呼ぶ順番を変えるために)
	player->SkillSort();

	// プレイヤー初期処理
	player->Initialize();

	// プレイヤーをキャラクターマネージャにセット
	CharacterManager::Instance().SetPlayer(player);

}

void GambleScene::Finalize()
{
	// ここではまだプレイヤーを消去しない
	//ゲームが完全に終了するタイミングで消す

}

void GambleScene::Update(HWND hwnd, float elapsedTime)
{
    GamePad& game_pad = Input::Instance().GetGamePad();
    if (game_pad.GetButtonDown() & GamePad::BTN_START)
    {
		Lemur::Scene::SceneManager::Instance().ChangeScene(new GameScene);
    }
	Mouse& mouse = Input::Instance().GetMouse();
	if(mouse.GetButtonDown()& Mouse::BTN_LEFT)
	{
		Lemur::Scene::SceneManager::Instance().ChangeScene(new GameScene);

	}
}

void GambleScene::Render(float elapsedTime)
{
}

void GambleScene::SetPlayerSkills()
{
	int all_skill_count = all_skills.size();

	_ASSERT_EXPR(player->skill_capacity <= all_skill_count, L"取得可能スキル超過");

	// 所持できる分だけ繰り返す
	for (int i = 0; i < player->skill_capacity;)
	{
		BaseSkill* skill = all_skills.at(rand() % all_skill_count).get();

		// もうすでに取得してたらもう一回
		if (skill->GetOwner())
		{
			continue;
		}
		player->SetSkill(skill);
		i++;
	}
}


