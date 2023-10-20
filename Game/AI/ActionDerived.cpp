// ReSharper disable All
#include "ActionDerived.h"

#include <Game/Easing.h>

#include "../Enemy/Enemy.h"
#include "Game/MathHelper.h"
#include "Game/Manager/CharacterManager.h"

ActionBase::State WanderAction::Run(float elapsedTime)
{
	switch (step)
	{
	case 0:
		// todo �A�j���[�V�����ݒ�
		owner->SetAnimationIndex(owner->Walk_Anim);
		owner->SetRandomTargetPosition();

	    step++;
		break;
	case 1:
		// ���S���Ă��玀�S�A�N�V�����Ɉڍs
		if (owner->death)
			return ActionBase::State::Failed;

		// ���񂾂�I��
		if (owner->fear_frag) {
			step = 0;
			return ActionBase::State::Failed;
		}

		if (owner->DistanceJudge(owner->GetPosition(), owner->GetTargetPosition(), 1.0f))
		{
			step = 0;

			return State::Complete;
		}

	    owner->Move_to_Target(elapsedTime);

		// �G�𔭌�������I��
		if (owner->SearchPlayer(owner->GetVisionLength()))
		{
			step = 0;
			return ActionBase::State::Failed;
		}


	}
	    return ActionBase::State::Run;
}

ActionBase::State IdleAction::Run(float elapsedTime)
{
    switch (step)
    {
    case 0:
		// ���̓e�L�g�[
		idleing_time = Mathf::RandomRange(1.0f, 3.0f);

		// todo �A�j���[�V����

		step++;
		break;

    case 1:
		// ���S���Ă��玀�S�A�N�V�����Ɉڍs
		if (owner->death)
			return ActionBase::State::Failed;

		idleing_time -= elapsedTime;

		// ���񂾂�I��
		if (owner->fear_frag) {
			step = 0;
			return ActionBase::State::Failed;
		}

		// �ҋ@�I��������
		if (idleing_time < 0.0f)
		{
			// ���̜p�j�Ɍ������ʒu��ݒ�(WanderJudgement���ʂ�͂�)
			owner->SetRandomTargetPosition();

			// todo ����Y�ꂪ���₩�玩����������
			step = 0;
		    return State::Complete;
		}

		// �G�𔭌�������I��
		if (owner->SearchPlayer(owner->GetVisionLength()))
			return ActionBase::State::Failed;


    }
		return State::Run;
    
}

ActionBase::State TuraAction::Run(float elapsedTime)
{
	// ���S���Ă��玀�S�A�N�V�����Ɉڍs
	if (owner->death)
	{
		owner->SetAnimCalcRate(1.0f);
		return ActionBase::State::Failed;
	}

	// ���񂾂�I��
	if (owner->fear_frag) {
		owner->SetAnimCalcRate(1.0f);
		step = 0;
		return ActionBase::State::Failed;
	}

	switch (step)
	{
	case 0:
		// �A�j���[�V�����Z�b�g
		owner->SetAnimationIndex(owner->Walk_Anim);
		// �����킹�͕������[�V�����̂�����Ƒ�����
		owner->SetAnimCalcRate(2.0f);

		/*------------- �����킹�Ō����������v�Z ------------*/
		// �v���C���[�̈ʒu
		DirectX::XMFLOAT3 player_pos = CharacterManager::Instance().GetPlayer()->GetPosition();
		DirectX::XMVECTOR Player_Pos = DirectX::XMLoadFloat3(&player_pos);
		// �G(������)�̈ʒu
		DirectX::XMVECTOR Enemy_Pos = DirectX::XMLoadFloat3(&owner->GetPosition());
		//�v���C���[�܂ł̈ʒu
		DirectX::XMVECTOR Enemy_To_Player = DirectX::XMVectorSubtract(Player_Pos, Enemy_Pos);

		// ���K��
		Enemy_To_Player = DirectX::XMVector3Normalize(Enemy_To_Player);

		// ���o
		DirectX::XMFLOAT3 enemy_to_player;
		DirectX::XMStoreFloat3(&enemy_to_player, Enemy_To_Player);
		target_vec_x = enemy_to_player.x;
		target_vec_z = enemy_to_player.z;

	    step++;
		break;
	case 1:
		owner->Turn(target_vec_x, target_vec_z, 3.0f);
		if (owner->GetEndAnimation())
		{
			owner->SetAnimCalcRate(1.0f);
			step = 0;
			return ActionBase::State::Complete;
		}
	}
		return ActionBase::State::Run;

}

ActionBase::State PursueAction::Run(float elapsedTime)
{
	switch (step)
	{
	case 0:

		owner->SetAnimationIndex(owner->Walk_Anim);
		step++;
		break;

	case 1:

		// ���S���Ă��玀�S�A�N�V�����Ɉڍs
		if (owner->death)
			return ActionBase::State::Failed;

		// ���񂾂�I��
		if (owner->fear_frag) {
			step = 0;
			return ActionBase::State::Failed;
		}

		// �ǐՂ���^�[�Q�b�g�̈ʒu���Z�b�g
		owner->SetTargetPosition(CharacterManager::Instance().GetPlayer()->GetPosition());

		// �U���\�͈͂ɓ��B������R���v���[�g
		if(owner->DistanceJudge(owner->GetPosition(), owner->GetTargetPosition(), owner->GetNearAttackRange()))
		{
			step = 0;

			return ActionBase::State::Complete;
		}

		//�U���\�͈͂ɓ��B����܂Œǐ�
	    owner->Move_to_Target(elapsedTime);

	}
		return ActionBase::State::Run;
}

ActionBase::State NearAttackAction::Run(float elapsedTime)
{
	// ���S���Ă��玀�S�A�N�V�����Ɉڍs
	if (owner->death)
		return ActionBase::State::Failed;

	// ���񂾂�I��
	if (owner->fear_frag) {
		step = 0;
		return ActionBase::State::Failed;
	}

    switch (step)
    {
    case 0:
		owner->SetAnimationIndex(owner->NearAttack_Anim);
		step++;

		break;
    case 1:
		// ����t���[���̊Ԃ����U������\
		CollisionFragJudge();

		if (owner->GetEndAnimation())
		{
			step = 0;
			return  ActionBase::State::Complete;
		}

		// �����蔻��
		if(owner->attack_collision_flag)
		{
			Player* player = CharacterManager::Instance().GetPlayer();

			for (auto& attack_collision : owner->arm_attack_collisions)
			{
				owner->CollisionNodeVsPlayer(owner->meshName.c_str(), attack_collision->bone_name.c_str(), attack_collision->node_radius);
			}
		}


	}
		return ActionBase::State::Run;
}

ActionBase::State DeathAction::Run(float elapsedTime)
{
    switch (step)
    {
    case 0:
		// todo �A�j���[�V�����Đ�
		owner->SetAnimationIndex(owner->Death_Anim);
		step ++ ;
		break;
    case 1:

		if(owner->GetEndAnimation())
		{
			owner->Destroy();
			step = 0;
			return ActionBase::State::Complete;
		}
    }
		return ActionBase::State::Run;
}

ActionBase::State JumpAttackAction::Run(float elapsedTime)
{
	// ���񂾂�I��
	if (owner->fear_frag) {
		step = 0;
		return ActionBase::State::Failed;
	}
	// ���S���Ă��玀�S�A�N�V�����Ɉڍs
	if (owner->death)
		return ActionBase::State::Failed;


	switch (step)
	{
	case 0:
		// todo �A�j���[�V�����Đ�
		owner->SetAnimationIndex(owner->JumpAttack_Anim);
		step++;
		break;
	case 1:

		// ����t���[���̊Ԃ����U������\
		CollisionFragJudge();


		if (owner->GetEndAnimation())
		{
			step = 0;
			return ActionBase::State::Complete;
		}

		// �����蔻��
		if (owner->attack_collision_flag)
		{
			Player* player = CharacterManager::Instance().GetPlayer();

			for (auto& collision : owner->hit_collisions)
			{
				owner->CollisionNodeVsPlayer(owner->meshName.c_str(), collision->bone_name.c_str(), collision->node_radius);
			}

		}

	}
		return ActionBase::State::Run;
}

ActionBase::State DoubleAttackAction::Run(float elapsedTime)
{
	// ���񂾂�I��
	if (owner->fear_frag) {
		step = 0;
		return ActionBase::State::Failed;
	}
	// ���S���Ă��玀�S�A�N�V�����Ɉڍs
	if (owner->death)
		return ActionBase::State::Failed;


	switch (step)
	{
	case 0:
		// todo �A�j���[�V�����Đ�
		owner->SetAnimationIndex(owner->JumpAttack_Anim);
		step++;
		break;
	case 1:

		// ����t���[���̊Ԃ����U������\
		CollisionFragJudge();


		if (owner->GetEndAnimation())
		{
			step = 0;
			return ActionBase::State::Complete;
		}

		// �����蔻��
		if (owner->attack_collision_flag)
		{
			Player* player = CharacterManager::Instance().GetPlayer();

			for (auto& attack_collision : owner->arm_attack_collisions)
			{
				owner->CollisionNodeVsPlayer(owner->meshName.c_str(), attack_collision->bone_name.c_str(), attack_collision->node_radius);
			}
		}

	}
		return ActionBase::State::Run;

}

ActionBase::State TwinArmsAttackAction::Run(float elapsedTime)
{
	// ���񂾂�I��
	if (owner->fear_frag) {

		if (owner->enemy_type == "E_Spider")
		{
			owner->SetAnimCalcRate(1.0f);
		}
		step = 0;
		return ActionBase::State::Failed;
	}
	// ���S���Ă��玀�S�A�N�V�����Ɉڍs
	if (owner->death)
	{
		if (owner->enemy_type == "E_Spider")
		{
			owner->SetAnimCalcRate(1.0f);
		}
		return ActionBase::State::Failed;
	}


	switch (step)
	{
	case 0:
		// todo �A�j���[�V�����Đ�
		owner->SetAnimationIndex(owner->JumpAttack_Anim);

		// E�N���͈ꌂ���x������A�j�����x�x������B
        if (owner->enemy_type=="E_Spider")
        {
			owner->SetAnimCalcRate(0.7f);
        }
		step++;
		break;
	case 1:

		// ����t���[���̊Ԃ����U������\
		CollisionFragJudge();

		if (owner->GetEndAnimation())
		{
		    if (owner->enemy_type == "E_Spider")
			{
				owner->SetAnimCalcRate(1.0f);
			}
			step = 0;
			return ActionBase::State::Complete;
		}

		// �����蔻��
		if (owner->attack_collision_flag)
		{
			Player* player = CharacterManager::Instance().GetPlayer();

			for (auto& attack_collision : owner->arm_attack_collisions)
			{
				owner->CollisionNodeVsPlayer(owner->meshName.c_str(), attack_collision->bone_name.c_str(), attack_collision->node_radius);
			}
		}
	}

	return ActionBase::State::Run;
}

ActionBase::State PoisonAttackAction::Run(float elapsedTime)
{
	// ���񂾂�I��
	if (owner->fear_frag) {
		step = 0;
		return ActionBase::State::Failed;
	}
	// ���S���Ă��玀�S�A�N�V�����Ɉڍs
	if (owner->death)
		return ActionBase::State::Failed;


	switch (step)
	{
	case 0:
		// todo �A�j���[�V�����Đ�
		owner->SetAnimationIndex(owner->ShotAttack_Anim);
		step++;
		break;
	case 1:

		// ����t���[���̊Ԃ����U������\
		CollisionFragJudge();

		if (owner->GetEndAnimation())
		{
			step = 0;
			return ActionBase::State::Complete;
		}

		// �����蔻��
		if (owner->attack_collision_flag)
		{
			Player* player = CharacterManager::Instance().GetPlayer();

			owner->CollisionNodeVsPlayer(owner->meshName.c_str(), "J_root", owner->GetAttackCollisionRange());
		}
	}

	return ActionBase::State::Run;

}

ActionBase::State RushAttackAction::Run(float elapsedTime)
{
	// ���񂾂�I��
	if (owner->fear_frag) {
		step = 0;
		owner->SetAnimCalcRate(1.0f);

		return ActionBase::State::Failed;
	}
	// ���S���Ă��玀�S�A�N�V�����Ɉڍs
	if (owner->death)
	{
		owner->SetAnimCalcRate(1.0f);
		return ActionBase::State::Failed;
	}


	switch (step)
	{
	case 0:
		// todo �A�j���[�V�����Đ�
		owner->SetAnimationIndex(owner->Walk_Anim);

	    owner->SetAnimCalcRate(3.0f);

		// �^�[�Q�b�g�ʒu�ݒ肷��
		CalcTargetPos();

		step++;
		break;
	case 1:

		// ����t���[���̊Ԃ����U������\
		CollisionFragJudge();

		if (owner->GetEndAnimation())
		{
			if (owner->DistanceJudge(owner->GetPosition(), owner->GetTargetPosition(), owner->GetNearAttackRange()))
			{
				step = 0;
				owner->SetAnimCalcRate(1.0f);

				return ActionBase::State::Complete;
			}
			else owner->SetAnimationIndex(owner->Walk_Anim);

		}

		owner->Move_to_Target(elapsedTime, 2.0f);
		// �����蔻��
		if (owner->attack_collision_flag)
		{
			Player* player = CharacterManager::Instance().GetPlayer();

			owner->CollisionNodeVsPlayer(owner->meshName.c_str(), "J_root", owner->GetAttackCollisionRange());
		}
	}

	return ActionBase::State::Run;


}

void RushAttackAction::CalcTargetPos()
{
	// �^�[�Q�b�g�ʒu�ݒ肷��

    // �v���C���[�̈ʒu
	DirectX::XMFLOAT3 player_pos = CharacterManager::Instance().GetPlayer()->GetPosition();
	DirectX::XMVECTOR Player_pos = DirectX::XMLoadFloat3(&player_pos);

	// �G(������)�̈ʒu
	DirectX::XMFLOAT3 enemy_pos = owner->GetPosition();
	DirectX::XMVECTOR Enemy_pos = DirectX::XMLoadFloat3(&enemy_pos);

	// �v���C���[����G�܂ł̕����x�N�g��(���K��)
	DirectX::XMVECTOR Vec = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(Player_pos, Enemy_pos));

	// �����x�N�g���ɉ߂����鋗���������āA�n���^�[�̈ʒu�𑫂�����^�[�Q�b�g�ʒu�̏o���オ��
	DirectX::XMVECTOR Target_pos = DirectX::XMVectorScale(Vec, run_over_length);
	Target_pos = DirectX::XMVectorAdd(Target_pos, Player_pos);
	DirectX::XMFLOAT3 target_pos;
	DirectX::XMStoreFloat3(&target_pos, Target_pos);
	owner->SetTargetPosition(target_pos);

}

ActionBase::State BackStepAction::Run(float elapsedTime)
{
	// ���S���Ă��玀�S�A�N�V�����Ɉڍs
	if (owner->death)
		return ActionBase::State::Failed;
	// ���񂾂�I��
	if (owner->fear_frag) {
		step = 0;
		return ActionBase::State::Failed;
	}

	switch (step)
	{
	case 0:
		// todo �A�j���[�V�����Đ�
		owner->SetAnimationIndex(owner->BackStep_Anim);

		// ��]�s��̍쐬
		DirectX::XMMATRIX rotation_matrix = DirectX::XMMatrixRotationRollPitchYaw(owner->GetAngle().x, owner->GetAngle().y, owner->GetAngle().z);
		// �O�����x�N�g���擾
		DirectX::XMVECTOR Forward = rotation_matrix.r[2];

		Forward = DirectX::XMVector3Normalize(Forward);

		DirectX::XMFLOAT3 front;
		DirectX::XMStoreFloat3(&front, Forward);
		start_pos = owner->GetPosition();
		end_pos = { start_pos.x - front.x * length, start_pos.y,start_pos.z - front.z * length };

		easing_timer = 0;

		step++;
		break;
	case 1:

		// todo �ق��̃C�[�W���O����
		owner->SetPosition(
			// �C�[�W���O�ɂ��X���W�̍X�V
			Easing::OutCubic(easing_timer, easing_time, end_pos.x, start_pos.x),
			owner->GetPosition().y,
			Easing::OutCubic(easing_timer, easing_time, end_pos.z, start_pos.z)
			);
		// todo �����ŕǃ��C�L���X�g


		easing_timer += high_resolution_timer::Instance().time_interval();
		if (owner->GetEndAnimation())
		{
			step = 0;
			return ActionBase::State::Complete;
		}
	}
		return ActionBase::State::Run;

}

ActionBase::State FearAction::Run(float elapsedTime)
{
	switch (step)
	{
	case 0:
		// todo �A�j���[�V�����Đ�
		owner->SetAnimationIndex(owner->Fear_Anim);
		step++;
		break;
	case 1:

		if (owner->GetEndAnimation())
		{
			owner->fear_frag = false;
			step = 0;
			return ActionBase::State::Complete;
		}
	}
		return ActionBase::State::Run;

}

