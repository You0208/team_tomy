#include "GambleScene.h"

#include "imgui.h"
#include "Game/Manager/CharacterManager.h"
#include "Game/Skill/AttackSkillDerived.h"
#include "Game/Skill/BadSkillDerived.h"
#include "Game/Skill/SkillDerived.h"
#include "Lemur/Input/Input.h"
#include "Lemur/Scene/SceneManager.h"
#include "SceneGame.h"
#include "Quest.h"

// todo 牟田さん こいつにQuestPattern型のどのクエストにするのか選択して値を入れる処理を作ってください
QuestPattern quest_pattern = QuestPattern::B;

// 一番初めのプレイヤーを生成したか(二週目移行か)
bool is_first_set_player = false;
void GambleScene::Initialize()
{
    // アセットロード


	if(!is_first_set_player)
	{
		// プレイヤーの生成
	    player = CreatePlayer();

        // ゲームの全スキルの設定
		player->SetSkill<StrongArm>();
		player->SetSkill<DemonPower>();
		player->SetSkill<MagicSword>();
		player->SetSkill<Cruel>();
		player->SetSkill<Revenge>();
		player->SetSkill<BloodSucking>();
		player->SetSkill<Sprint>();
		player->SetSkill<Acceleration>();
		player->SetSkill<Patience>();
		player->SetSkill<Regeneration>();
		player->SetSkill<SuperMan>();
		player->SetSkill<SwordSaint>();
		player->SetSkill<Gale>();
		player->SetSkill<Obesity>();

		player->SetSkill<Tofu>();

		is_first_set_player = true;
	}
	else
	{
		// もうすでに生成してたらシングルトンクラスから持ってくるだけ
		player = CharacterManager::Instance().GetPlayer();
	}

	player->TestSkillSet("StrongArm");
	player->TestSkillSet("SuperMan");
	// プレイヤーにスキルを取得させる
	player->SetPlayerSkills();

	// 優先順位でスキルを並び替え(Initとかupdateを呼ぶ順番を変えるために)
	player->SkillSort();

	// ここでシングルトンクラスにセットしてこいつをゲームシーンで渡す
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
	if(mouse.GetButtonDown()& Mouse::BTN_RIGHT)
	{
		Lemur::Scene::SceneManager::Instance().ChangeScene(new GameScene);

	}
}

void GambleScene::Render(float elapsedTime)
{
	Lemur::Graphics::Graphics& graphics = Lemur::Graphics::Graphics::Instance();
	ID3D11DeviceContext* immediate_context = graphics.GetDeviceContext();

	SetUpRendering();

	player->DebugImgui();
	DebugImGui();
}

void GambleScene::DebugImGui()
{
	ImGui::Begin("Scene");
	ImGui::Checkbox("is_first_set_player", &is_first_set_player);
	ImGui::End();

}



