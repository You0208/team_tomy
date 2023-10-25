// ReSharper disable All
#include "ActionDerived.h"

#include <Game/Easing.h>

#include "../Enemy/Enemy.h"
#include "Game/MathHelper.h"
#include "Game/Manager/CharacterManager.h"

ActionBase::State WanderAction::Run(float elapsedTime)
{
	// 死亡してたら死亡アクションに移行
	if (owner->death)
		return ActionBase::State::Failed;

	// 怯んだら終了
	if (owner->fear_frag) {
		step = 0;
		return ActionBase::State::Failed;
	}
	// 戦闘中になったら終了
	if(owner->belligerency) {
		step = 0;
		return ActionBase::State::Failed;
	}
	switch (step)
	{
	case 0:
		// todo アニメーション設定
		owner->SetAnimationIndex(owner->Walk_Anim,true);
		owner->SetRandomTargetPosition();

	    step++;
		break;
	case 1:
		// 死亡してたら死亡アクションに移行
		if (owner->death)
			return ActionBase::State::Failed;


		if (owner->DistanceJudge(owner->GetPosition(), owner->GetTargetPosition(), 1.0f))
		{
			step = 0;

			return State::Complete;
		}

	    owner->Move_to_Target(elapsedTime);

		// 敵を発見したら終了
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
	// 死亡してたら死亡アクションに移行
	if (owner->death)
		return ActionBase::State::Failed;

	idleing_time -= elapsedTime;

	// 怯んだら終了
	if (owner->fear_frag) {
		step = 0;
		return ActionBase::State::Failed;
	}
	// 戦闘中になったら終了
	if (owner->belligerency) {
		step = 0;
		return ActionBase::State::Failed;
	}


    switch (step)
    {
    case 0:
		// 数はテキトー
		idleing_time = Mathf::RandomRange(1.0f, 3.0f);

		owner->SetAnimationIndex(owner->Idle_Anim, true);
		step++;
		break;

    case 1:
		// 待機終了したら
		if (idleing_time < 0.0f)
		{
			// 次の徘徊に向かう位置を設定(WanderJudgementが通るはず)
			owner->SetRandomTargetPosition();

			// todo これ忘れがちやから自動化したい
			step = 0;
		    return State::Complete;
		}

		// 敵を発見したら終了
		if (owner->SearchPlayer(owner->GetVisionLength()))
			return ActionBase::State::Failed;


    }
		return State::Run;
    
}

ActionBase::State TuraAction::Run(float elapsedTime)
{
	// 死亡してたら死亡アクションに移行
	if (owner->death)
	{
		owner->SetAnimCalcRate(1.0f);
		return ActionBase::State::Failed;
	}

	// 怯んだら終了
	if (owner->fear_frag) {
		owner->SetAnimCalcRate(1.0f);
		step = 0;
		return ActionBase::State::Failed;
	}

	switch (step)
	{
	case 0:
		// アニメーションセット
		owner->SetAnimationIndex(owner->Walk_Anim, true);
		// 軸合わせは歩きモーションのちょっと早送り
		owner->SetAnimCalcRate(2.0f);

		/*------------- 軸合わせで向く方向を計算 ------------*/
		// プレイヤーの位置
		DirectX::XMFLOAT3 player_pos = CharacterManager::Instance().GetPlayer()->GetPosition();
		DirectX::XMVECTOR Player_Pos = DirectX::XMLoadFloat3(&player_pos);
		// 敵(こいつの)の位置
		DirectX::XMVECTOR Enemy_Pos = DirectX::XMLoadFloat3(&owner->GetPosition());
		//プレイヤーまでの位置
		DirectX::XMVECTOR Enemy_To_Player = DirectX::XMVectorSubtract(Player_Pos, Enemy_Pos);

		// 正規化
		Enemy_To_Player = DirectX::XMVector3Normalize(Enemy_To_Player);

		// 抽出
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

		owner->SetAnimationIndex(owner->Walk_Anim, true);
		step++;
		break;

	case 1:

		// 死亡してたら死亡アクションに移行
		if (owner->death)
			return ActionBase::State::Failed;

		// 怯んだら終了
		if (owner->fear_frag) {
			step = 0;
			return ActionBase::State::Failed;
		}

		// 追跡するターゲットの位置をセット
		owner->SetTargetPosition(CharacterManager::Instance().GetPlayer()->GetPosition());

		// 攻撃可能範囲に到達したらコンプリート
		if(owner->DistanceJudge(owner->GetPosition(), owner->GetTargetPosition(), owner->GetNearAttackRange()))
		{
			step = 0;

			return ActionBase::State::Complete;
		}

		//攻撃可能範囲に到達するまで追跡
	    owner->Move_to_Target(elapsedTime);

	}
		return ActionBase::State::Run;
}

ActionBase::State NearAttackAction::Run(float elapsedTime)
{
	// 死亡してたら死亡アクションに移行
	if (owner->death)
		return ActionBase::State::Failed;

	// 怯んだら終了
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
		// 限定フレームの間だけ攻撃判定可能
		CollisionFragJudge();

		if (owner->GetEndAnimation())
		{
			step = 0;
			return  ActionBase::State::Complete;
		}

		// 当たり判定
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
		// todo アニメーション再生
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
	// 怯んだら終了
	if (owner->fear_frag) {
		step = 0;
		return ActionBase::State::Failed;
	}
	// 死亡してたら死亡アクションに移行
	if (owner->death)
		return ActionBase::State::Failed;


	switch (step)
	{
	case 0:
		// todo アニメーション再生
		owner->SetAnimationIndex(owner->JumpAttack_Anim);
		step++;
		break;
	case 1:

		// 限定フレームの間だけ攻撃判定可能
		CollisionFragJudge();


		if (owner->GetEndAnimation())
		{
			step = 0;
			return ActionBase::State::Complete;
		}

		// 当たり判定
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
	// 怯んだら終了
	if (owner->fear_frag) {
		step = 0;
		return ActionBase::State::Failed;
	}
	// 死亡してたら死亡アクションに移行
	if (owner->death)
		return ActionBase::State::Failed;


	switch (step)
	{
	case 0:
		// todo アニメーション再生
		owner->SetAnimationIndex(owner->JumpAttack_Anim);
		step++;
		break;
	case 1:

		// 限定フレームの間だけ攻撃判定可能
		CollisionFragJudge();


		if (owner->GetEndAnimation())
		{
			step = 0;
			return ActionBase::State::Complete;
		}

		// 当たり判定
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
	// 怯んだら終了
	if (owner->fear_frag) {

		if (owner->enemy_type == "E_Spider")
		{
			owner->SetAnimCalcRate(1.0f);
		}
		step = 0;
		return ActionBase::State::Failed;
	}
	// 死亡してたら死亡アクションに移行
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
		// todo アニメーション再生
		owner->SetAnimationIndex(owner->JumpAttack_Anim);

		// Eクモは一撃が遅いからアニメ速度遅くする。
        if (owner->enemy_type=="E_Spider")
        {
			owner->SetAnimCalcRate(0.7f);
        }
		step++;
		break;
	case 1:

		// 限定フレームの間だけ攻撃判定可能
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

		// 当たり判定
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
	// 怯んだら終了
	if (owner->fear_frag) {
		step = 0;
		return ActionBase::State::Failed;
	}
	// 死亡してたら死亡アクションに移行
	if (owner->death)
		return ActionBase::State::Failed;


	switch (step)
	{
	case 0:
		// todo アニメーション再生
		owner->SetAnimationIndex(owner->ShotAttack_Anim);
		step++;
		break;

	case 1:
		if(owner->GetFrameIndex()>=140)
		{
			poison_pos = owner->GetModel()->joint_position(owner->meshName.c_str(), "J_lowbody_end", &owner->keyframe, owner->world);
			poison->Play(poison_pos);

			step++;
		}

		break;
	case 2:

		if (owner->GetEndAnimation())
		{
			step = 0;
			return ActionBase::State::Complete;
		}

		//// 当たり判定
		//if (owner->attack_collision_flag)
		//{
		//	Player* player = CharacterManager::Instance().GetPlayer();

		//	owner->CollisionNodeVsPlayer(owner->meshName.c_str(), "J_root", owner->GetAttackCollisionRange());
		//}
	}

	return ActionBase::State::Run;

}

ActionBase::State RushAttackAction::Run(float elapsedTime)
{
	// 怯んだら終了
	if (owner->fear_frag) {
		step = 0;
		owner->SetAnimCalcRate(1.0f);

		return ActionBase::State::Failed;
	}
	// 死亡してたら死亡アクションに移行
	if (owner->death)
	{
		owner->SetAnimCalcRate(1.0f);
		return ActionBase::State::Failed;
	}


	switch (step)
	{
	case 0:
		// todo アニメーション再生
		owner->SetAnimationIndex(owner->Walk_Anim, true);

	    owner->SetAnimCalcRate(3.0f);

		// ターゲット位置設定する
		CalcTargetPos();

		step++;
		break;
	case 1:

		// 限定フレームの間だけ攻撃判定可能
		CollisionFragJudge();

		if (owner->GetEndAnimation())
		{
			if (owner->DistanceJudge(owner->GetPosition(), owner->GetTargetPosition(), 1.0f))
			{
				step = 0;
				owner->SetAnimCalcRate(1.0f);

				return ActionBase::State::Complete;
			}
			else owner->SetAnimationIndex(owner->Walk_Anim, true);

		}

		owner->Move_to_Target(elapsedTime, 2.0f);
		// 当たり判定
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
	// ターゲット位置設定する

    // プレイヤーの位置
	DirectX::XMFLOAT3 player_pos = CharacterManager::Instance().GetPlayer()->GetPosition();
	DirectX::XMVECTOR Player_pos = DirectX::XMLoadFloat3(&player_pos);

	// 敵(こいつ)の位置
	DirectX::XMFLOAT3 enemy_pos = owner->GetPosition();
	DirectX::XMVECTOR Enemy_pos = DirectX::XMLoadFloat3(&enemy_pos);

	// プレイヤーから敵までの方向ベクトル(正規化)
	DirectX::XMVECTOR Vec = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(Player_pos, Enemy_pos));

	// 方向ベクトルに過ぎ去る距離をかけて、ハンターの位置を足したらターゲット位置の出来上がり
	DirectX::XMVECTOR Target_pos = DirectX::XMVectorScale(Vec, run_over_length);
	Target_pos = DirectX::XMVectorAdd(Target_pos, Player_pos);
	DirectX::XMFLOAT3 target_pos;
	DirectX::XMStoreFloat3(&target_pos, Target_pos);

    if (target_pos.x < -20)
		target_pos.x = -20;
	else if (target_pos.x > 22)
		target_pos.x = 22;

	if (target_pos.z < 3)
		target_pos.z = 3;
	else if (target_pos.z > 50)
		target_pos.z = 50;

	owner->SetTargetPosition(target_pos);

}

ActionBase::State BackStepAction::Run(float elapsedTime)
{
	// 死亡してたら死亡アクションに移行
	if (owner->death)
		return ActionBase::State::Failed;
	// 怯んだら終了
	if (owner->fear_frag) {
		step = 0;
		return ActionBase::State::Failed;
	}

	switch (step)
	{
	case 0:
		// todo アニメーション再生
		owner->SetAnimationIndex(owner->BackStep_Anim);

		// 回転行列の作成
		DirectX::XMMATRIX rotation_matrix = DirectX::XMMatrixRotationRollPitchYaw(owner->GetAngle().x, owner->GetAngle().y, owner->GetAngle().z);
		// 前方向ベクトル取得
		DirectX::XMVECTOR Forward = rotation_matrix.r[2];

		Forward = DirectX::XMVector3Normalize(Forward);

		DirectX::XMFLOAT3 front;
		DirectX::XMStoreFloat3(&front, Forward);

	    start_pos = owner->GetPosition();
		// 現在の位置から前方向の逆ベクトルにlength分下がる。
		end_pos = { start_pos.x - front.x * length, start_pos.y,start_pos.z - front.z * length };

		easing_timer_ms = 0;

		step++;
		break;
	case 1:

		if (owner->GetFrameIndex() >= jump_anim_start_frame)
		{
			// todo ほかのイージング試す
			owner->SetPosition(
				// イージングによるX座標の更新
				Easing::OutSine(easing_timer_ms, easing_time_ms, end_pos.x, start_pos.x),
				owner->GetPosition().y,
				Easing::OutSine(easing_timer_ms, easing_time_ms, end_pos.z, start_pos.z)
			);

			easing_timer_ms += high_resolution_timer::Instance().time_interval();

		}
		// todo ここで壁レイキャスト


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
		// todo アニメーション再生
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

