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
#if 0 徘徊処理
			// ターゲット位置までの距離判定

			// ターゲット位置
			DirectX::XMFLOAT3 target_position = owner->GetTargetPosition();
			// モンスター位置
			DirectX::XMFLOAT3 position = owner->GetOwner()->GetTransform()->GetPosition();

			float vx = target_position.x - position.x;
			float vz = target_position.z - position.z;

			// ターゲット位置までの距離(二乗)
			float dist_sq = ((vx * vx) + (vz * vz));

			// 到達した判定になる範囲
			float range = owner->GetRadius() * owner->GetRadius();
			if (dist_sq < range)
			{
				// 目的地にたどり着いた
				step = 0;

				if (owner->GetMigrationAwaking())
				{
					//カメラ引く
					owner->target_hunter->GetCameraController()->ChangeZoom(10.0f);
				}
				// 止まる
				//owner->DisableVelocity();
				return ActionBase::State::Complete;
			}

			// ターゲット位置に移動
			owner->Move_to_Target(elapsedTime, wander_speed_facter);

			// プレイヤーを発見してかつ、覚醒移行行動中じゃなかったら徘徊終了。
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

