#pragma once
#include "Game/Player/Player.h"
#include "Game/Skill/BaseSkill.h"
#include "Lemur/Scene/BaseScene.h"
#include "Lemur/Graphics/sprite_d.h"
#include "Lemur/Font/font.h"



// �X�L���z�z�A�G�N�G�X�g���A�X�e�[�^�X������
//������V�[��
class GambleScene :
	public Lemur::Scene::BaseScene
{
public:
	GambleScene() {}
	~GambleScene() override {}

	// ������
	void Initialize()override;

	// �I����
	void Finalize()override;

	// �X�V����
	void Update(HWND hwnd, float elapsedTime)override;

	// �`�揈��
	void Render(float elapsedTime)override;

	// �t�F�[�Y
	enum Phase
	{
		Skill_Lottery,// �X�L�����I
		Quest_Select,// �N�G�X�g�I��
		Gamble_Status,// �X�e�[�^�X�M�����u��
		Loading_Transition,// ���[�h�V�[���ɑJ��
	};
	int step;

	void DebugImGui();

	// �f�[�^
	struct data
	{
		WCHAR title[256];
		WCHAR contents[256];
		int pattern;
		float min_magnification;
		float max_magnification;
	};

	// �N�G�X�g���̓{�X�܂߂�17��
	data quest_data[30];
	data skill_data[30];

	void set_skill_data();
	void set_quest_data();
	//void AllIsSelectReset()
	//{
	//    
	//}

	int skill_num_max;

	int select_num;
	int select_down_num;
	bool select;

	struct Card
	{
		std::wstring wcText = L"Dummy";
		//WCHAR wcText[256];
		int category;
		DirectX::XMFLOAT2 position;
		DirectX::XMFLOAT2 size;
		DirectX::XMFLOAT2 font_position;
	};
	Card questCard[3];
	Card skillCard[3];
	DirectX::XMFLOAT2 Poo = {};

	DirectX::XMFLOAT2 bet_boxpos[3];
	DirectX::XMFLOAT2 bet_boxsize = { 400,100 };


	float font_d[3];

	int selection_card = 1;
	DirectX::XMFLOAT2 arrow_position[2]{};
	DirectX::XMFLOAT2 arrow_size = { 95, 145 };

	DirectX::XMFLOAT2 select_pos[2] = {
		{448,915},
		{1065,915}
	};

	DirectX::XMFLOAT2 select_decision_pos;

	DirectX::XMFLOAT2 small_arrow_up_pos[3];
	DirectX::XMFLOAT2 small_arrow_down_pos[3];
	DirectX::XMFLOAT2 char_bet_pos[3];

	DirectX::XMFLOAT2 num_bet_pos[3];

	DirectX::XMFLOAT2 coin_bet_pos[3];

	int bet_num[3];



private:
	Player* CreatePlayer()
	{
		return new Player(
			new PlayerInputComponent(),
			new PlayerPhysicsComponent(),
			new PlayerGraphicsComponent()
		);
	}

	// ���I�X�L���z��ɃX�L���𒊑I���ăZ�b�g����
	void SetLotterySkills();

	// ���I����Ă�X�L��
	std::vector<BaseSkill*> lottery_skills;

	// ���I�\��
	int can_lottery_count = 3;

	bool IsDirection;
	bool SelectCard[3] = {};
	float plusPos[3] = {};
	int last_num;

	// �|�C���g�{��
	float min_magnification;
	float max_magnification;

	float player_status[3];
	float player_status_bet[3];// �v���C���[���������X�e�[�^�X�|�C���g
	float player_status_max[3];

	Player* player;
private:/*---------------- �X�v���C�g�֌W -----------------*/

	// �`���[�g���A���̃y�[�W

	std::unique_ptr<sprite> spr_tutorial_01;
	std::unique_ptr<sprite> spr_tutorial_02;
	std::unique_ptr<sprite> spr_tutorial_03;
	std::unique_ptr<sprite> spr_tutorial_04;
	std::unique_ptr<sprite> spr_tutorial_05;
	std::unique_ptr<sprite> spr_tutorial_06;
	std::unique_ptr<sprite> spr_tutorial_07;
	std::unique_ptr<sprite> spr_tutorial_08;
	std::unique_ptr<sprite> spr_tutorial_09;

	// �`���[�g���A���y�[�W�̈ʒu(�S�����ʂ̎g���āA�C�[�W���O����Ƃ��ɏ�����)
	DirectX::XMFLOAT2 spr_tutorial_pos = { 1920.0f,0.0f };

	void TutorialRender();

	std::shared_ptr<sprite>  spr_back;
	std::shared_ptr<sprite>  spr_card;
	std::shared_ptr<sprite>  spr_arrow;
	std::shared_ptr<sprite>  spr_select;

	std::shared_ptr<sprite> spr_bet_icon[3];

	std::shared_ptr<sprite> spr_betbox;
	std::shared_ptr<sprite> spr_coin[3];
	std::shared_ptr<sprite> spr_OK;
	std::shared_ptr<sprite> spr_betback;
	std::shared_ptr<sprite> spr_small_arrow;
	std::shared_ptr<sprite> spr_number;

	std::shared_ptr<sprite> spr_skill_back;

	std::shared_ptr<sprite> spr_skill[22];
	std::shared_ptr<sprite> spr_quest[22];

	std::shared_ptr<sprite> spr_skill_ok;
	std::shared_ptr<sprite> spr_skill_change;


	/*------- �C�[�W���O�֌W --------*/

	// �C�[�W���O���鎞��
	float easing_time_ms = 1.0f;
	// �C�[�W���O���Ă鎞��
	float easing_timer_ms = 0.0f;

	// �`���[�g���A����ʂ��^�񒆂łƂǂ܂鎞��
	float stop_time_ms = 3.0f;
	// �`���[�g���A����ʂ��E�łƂǂ܂鎞��
	float hide_stop_time_ms = 1.0f;
	// �`���[�g���A����ʂ��Ƃǂ܂鏈���̒l�������^�C�}�[
	float stop_timer_ms = 0.0f;

	// �`���[�g���A����ʂ��C�[�W���O���鏈��
	//�߂�l�ɂ̓C�[�W���O���I�����������Ԃ�
	//�������A�C�[�W���O�n�߂���W
	//�������A�C�[�W���O�Ŏ~�܂���W
	//��O�����A�C�[�W���O�n�܂�܂ł̎���
	//��l�����A�C�[�W���O�ɂ����鎞��
	bool EasingTutorial(int start_pos, int end_pos,float stop_time_ms, float easing_time_ms);

	void ResetEasingTime()
	{
		easing_timer_ms = 0.0f;
		stop_timer_ms = 0.0f;
	}
	int easing_step;
};

