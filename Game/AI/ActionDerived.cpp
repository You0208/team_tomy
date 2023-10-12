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
		// todo アニメーション設定

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

		// 敵を発見したら終了
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
		// 数はテキトー
		idleing_time = Mathf::RandomRange(1.0f, 3.0f);

		// todo アニメーション

		step++;
		break;

    case 1:

		idleing_time -= elapsedTime;


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

		// 追跡するターゲットの位置をセット
		owner->SetTargetPosition(CharacterManager::Instance().GetPlayer()->GetPosition());

		// 攻撃可能範囲に到達したらコンプリート
		if(owner->ReachTargetJudge(owner->GetPosition(), owner->GetTargetPosition(), owner->GetNearAttackRange()))
		{
			step = 0;

			return ActionBase::State::Complete;
		}

		//攻撃可能範囲に到達するまで追跡
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

		// 限定フレームの間だけ攻撃判定可能
		if (owner->GetFrameIndex() >= start_collision__frame &&
			owner->GetFrameIndex() <= end_collision__frame)
			owner->attack_collision_flag = true;

		else owner->attack_collision_flag = false;


		if (owner->GetEndAnimation())
		{
			step = 0;
			return  ActionBase::State::Complete;
		}

		// 当たり判定
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
		// todo アニメーション再生
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

