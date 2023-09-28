// ReSharper disable All
#include "ActionDerived.h"
#include <Windows.h>
#include "../Enemy/Enemy.h"

    ActionBase::State WanderAction::Run(float elapsedTime)
    {
		switch (step)
		{
		case 0:

			//owner->anim_operator->SetAnimationIndex(owner->WanderWalkAnimIndex);
		//		wander_speed_facter = 1.5f;
		//		owner->anime_controller->SetAnimationState(owner->anime_controller->WanderWalkAnimIndex);
		//	
			step++;
			break;
		case 1:
#if 0 �p�j����
			// �^�[�Q�b�g�ʒu�܂ł̋�������

			// �^�[�Q�b�g�ʒu
			DirectX::XMFLOAT3 target_position = owner->GetTargetPosition();
			// �����X�^�[�ʒu
			DirectX::XMFLOAT3 position = owner->GetOwner()->GetTransform()->GetPosition();

			float vx = target_position.x - position.x;
			float vz = target_position.z - position.z;

			// �^�[�Q�b�g�ʒu�܂ł̋���(���)
			float dist_sq = ((vx * vx) + (vz * vz));

			// ���B��������ɂȂ�͈�
			float range = owner->GetRadius() * owner->GetRadius();
			if (dist_sq < range)
			{
				// �ړI�n�ɂ��ǂ蒅����
				step = 0;

				if (owner->GetMigrationAwaking())
				{
					//�J��������
					owner->target_hunter->GetCameraController()->ChangeZoom(10.0f);
				}
				// �~�܂�
				//owner->DisableVelocity();
				return ActionBase::State::Complete;
			}

			// �^�[�Q�b�g�ʒu�Ɉړ�
			owner->Move_to_Target(elapsedTime, wander_speed_facter);

			// �v���C���[�𔭌����Ă��A�o���ڍs�s��������Ȃ�������p�j�I���B
			if (owner->SearchPlayer(owner->GetVisionLength()) &&
				!owner->GetMigrationAwaking())
			{
				return ActionBase::State::Failed;
			}

#endif

		    return ActionBase::State::Run;
		}

    }

	ActionBase::State ClawAttackAction::Run(float elapsedTime)
	{
		//owner->GetOwner()->
		return ActionBase::State::Run;
	}

