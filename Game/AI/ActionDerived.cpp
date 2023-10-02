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
		
		DirectX::XMFLOAT3 target_pos = CharacterManager::Instance().GetPlayer()->GetPosition();

		if (owner->ReachTargetJudge(owner->GetPosition(), target_pos, 1.0f))
		{
			step = 0;

			return State::Complete;
		}

	    owner->Move_to_Target(elapsedTime);

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

		return State::Run;
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
		{
			step = 0;

			return ActionBase::State::Complete;
		}
	    owner->Move(vec_x, vec_z, owner->GetWalkSpeed());
		
		return ActionBase::State::Run;
	}
}

ActionBase::State ClawAttackAction::Run(float elapsedTime)
{
	//owner->GetOwner()->
	return ActionBase::State::Run;
}

