// ReSharper disable All
#include "ActionDerived.h"
#include "../Enemy/Enemy.h"
#include "Game/MathHelper.h"
#include "Game/Manager/CharacterManager.h"


ActionBase::State WanderAction::Run(float elapsedTime)
{
	switch (step)
	{
	case 0:
		// todo �A�j���[�V�����ݒ�

		owner->SetRandomTargetPosition();

	    step++;
		break;
	case 1:
		

		if (owner->ReachTargetJudge(owner->GetPosition(), owner->GetTargetPosition(), 1.0f))
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


	    return ActionBase::State::Run;
	}
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

		idleing_time -= elapsedTime;


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


		return State::Run;
    }
    
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

		// �ǐՂ���^�[�Q�b�g�̈ʒu���Z�b�g
		owner->SetTargetPosition(CharacterManager::Instance().GetPlayer()->GetPosition());

		// �U���\�͈͂ɓ��B������R���v���[�g
		if(owner->ReachTargetJudge(owner->GetPosition(), owner->GetTargetPosition(), owner->GetNearAttackRange()))
		{
			step = 0;

			return ActionBase::State::Complete;
		}

		//�U���\�͈͂ɓ��B����܂Œǐ�
	    owner->Move_to_Target(elapsedTime);

		return ActionBase::State::Run;
	}
}

ActionBase::State NearAttackAction::Run(float elapsedTime)
{
    switch (step)
    {
    case 0:
		owner->SetAnimationIndex(owner->NearAttack_Anim);
		step++;

		break;
    case 1:

		// ����t���[���̊Ԃ����U������\
		if (owner->GetFrameIndex() >= start_collision__frame &&
			owner->GetFrameIndex() <= end_collision__frame)
			owner->attack_collision_flag = true;

		else owner->attack_collision_flag = false;


		if (owner->GetEndAnimation())
		{
			step = 0;
			return  ActionBase::State::Complete;
		}

		// �����蔻��
		if(owner->attack_collision_flag)
		{
			Player* player = CharacterManager::Instance().GetPlayer();

			owner->CollisionNodeVsPlayer("polySurface1", "J_leg_A_03_L", owner->GetAttackCollisionRange());
		}


		break;
    }
}

ActionBase::State DeathAction::Run(float elapsedTime)
{
    switch (step)
    {
    case 0:
		// todo �A�j���[�V�����Đ�
		//owner->SetAnimationIndex()

		break;
    case 1:

		if(owner->GetEndAnimation())
		{
			owner->Destroy();
			return ActionBase::State::Complete;
		}
		return ActionBase::State::Run;
    }
}

ActionBase::State ClawAttackAction::Run(float elapsedTime)
{
	//owner->GetOwner()->
	return ActionBase::State::Run;
}

