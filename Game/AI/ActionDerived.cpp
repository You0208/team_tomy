// ReSharper disable All
#include "ActionDerived.h"
#include "../Enemy/Enemy.h"
#include "Game/Manager/CharacterManager.h"

// 二間のベクトルの長さを計算
inline float Length(const DirectX::XMFLOAT3& f1, const DirectX::XMFLOAT3& f2)
{
	float ret{};
	DirectX::XMStoreFloat(
		&ret,
		DirectX::XMVector3Length(
			DirectX::XMVectorSubtract(
				DirectX::XMLoadFloat3(&f1),
				DirectX::XMLoadFloat3(&f2)
			)
		)
	);
	return ret;
}

ActionBase::State WanderAction::Run(float elapsedTime)
{
	switch (step)
	{
	case 0:
		// todo アニメーション設定

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

ActionBase::State PursueAction::Run(float elapsedTime)
{
	switch (step)
	{
	case 0:

		// todo アニメーション設定
		//owner->GetModel()->animation_clips
		step++;
		break;

	case 1:

		DirectX::XMFLOAT3 player_pos = CharacterManager::Instance().GetPlayer()->GetPosition();
		DirectX::XMFLOAT3 enemy_pos = owner->GetPosition();

		float vec_x = player_pos.x - enemy_pos.x;
		float vec_z = player_pos.x - enemy_pos.x;
		float length = Length(player_pos, enemy_pos);
		vec_x = vec_x / length;
		vec_z = vec_z / length;
		if (length < owner->GetAttackRange())
			return ActionBase::State::Complete;

	    owner->Move(vec_x, vec_z, owner->GetWalkSpeed());
		
		return ActionBase::State::Run;
	}
}

ActionBase::State ClawAttackAction::Run(float elapsedTime)
{
	//owner->GetOwner()->
	return ActionBase::State::Run;
}

