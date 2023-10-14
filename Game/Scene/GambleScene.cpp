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

// todo ���c���� ������QuestPattern�^�̂ǂ̃N�G�X�g�ɂ���̂��I�����Ēl�����鏈��������Ă�������
QuestPattern quest_pattern = QuestPattern::B;

// ��ԏ��߂̃v���C���[�𐶐�������(��T�ڈڍs��)
bool is_first_set_player = false;
void GambleScene::Initialize()
{
    // �A�Z�b�g���[�h


	if(!is_first_set_player)
	{
		// �v���C���[�̐���
	    player = CreatePlayer();

        // �Q�[���̑S�X�L���̐ݒ�
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
		// �������łɐ������Ă���V���O���g���N���X���玝���Ă��邾��
		player = CharacterManager::Instance().GetPlayer();
	}

	player->TestSkillSet("StrongArm");
	player->TestSkillSet("SuperMan");
	// �v���C���[�ɃX�L�����擾������
	player->SetPlayerSkills();

	// �D�揇�ʂŃX�L������ёւ�(Init�Ƃ�update���Ăԏ��Ԃ�ς��邽�߂�)
	player->SkillSort();

	// �����ŃV���O���g���N���X�ɃZ�b�g���Ă������Q�[���V�[���œn��
	CharacterManager::Instance().SetPlayer(player);


}

void GambleScene::Finalize()
{
	// �����ł͂܂��v���C���[���������Ȃ�
	//�Q�[�������S�ɏI������^�C�~���O�ŏ���

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



