#include "GamePro_ProjectileStraight.h"
#include <imgui.h>
#include "./Lemur/Collision/Collision.h"
#include"./Lemur/Effekseer/EffekseerManager.h"
#include"./Lemur/Effekseer/Effect.h"


//void GamePro_ProjectileStraight::Launch(GameObject* gameobj, const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position)
//{
//    GamePro_ProjectileStraight* project = dynamic_cast<GamePro_ProjectileStraight*> (gameobj);
//    project->direction = direction;
//    project->position = position;
//}

void GamePro_ProjectileStraightGraphicsComponent::Initialize(GameObject* gameobj)
{
    Lemur::Graphics::Graphics& graphics = Lemur::Graphics::Graphics::Instance();
    ID3D11DeviceContext* immediate_context = graphics.GetDeviceContext();
	GamePro_ProjectileStraight* project = dynamic_cast<GamePro_ProjectileStraight*> (gameobj);

    BulletModel = ResourceManager::Instance().LoadModelResource(graphics.GetDevice(), ".\\resources\\Model\\jank\\jank_low_v001.fbx");
	project->projectEffect = new Effect("resources/Effects/shot_stylize.efk");
	project->handle = project->projectEffect->Play(project->position, 0.5f);
}

void GamePro_ProjectileStraightGraphicsComponent::Update(GameObject* gameobj)
{

}

void GamePro_ProjectileStraightGraphicsComponent::Render(GameObject* gameobj, float elapsedTime, ID3D11PixelShader* replaced_pixel_shader)
{
    GamePro_ProjectileStraight* project = dynamic_cast<GamePro_ProjectileStraight*> (gameobj);
    Lemur::Graphics::Graphics& graphics = Lemur::Graphics::Graphics::Instance();
    ID3D11DeviceContext* immediate_context = graphics.GetDeviceContext();


	// 左手系・Y 軸アップへ変換
	const DirectX::XMFLOAT4X4 coordinate_system_transforms[]{
		{ -1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 },	// 0:RHS Y-UP
		{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 },		// 1:LHS Y-UP
		{ -1, 0, 0, 0, 0, 0, -1, 0, 0, 1, 0, 0, 0, 0, 0, 1 },	// 2:RHS Z-UP
		{ 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1 },		// 3:LHS Z-UP
	};
#if 0
	// 単位をセンチメートルからメートルに変更するには、「scale_factor」を 0.01 に設定
	const float scale_factor = 1.0f; // To change the units from centimeters to meters, set 'scale_factor' to 0.01.
#else
	const float scale_factor = 0.01f; // To change the units from centimeters to meters, set 'scale_factor' to 0.01.
#endif
	// 変換用
	DirectX::XMMATRIX C{ DirectX::XMLoadFloat4x4(&coordinate_system_transforms[0])* DirectX::XMMatrixScaling(scale_factor, scale_factor, scale_factor) };
	DirectX::XMMATRIX S{ DirectX::XMMatrixScaling(project->scale.x, project->scale.y, project->scale.z) };
	DirectX::XMMATRIX R{ DirectX::XMMatrixRotationRollPitchYaw(project->rotation.x, project->rotation.y, project->rotation.z) };
	DirectX::XMMATRIX T{ DirectX::XMMatrixTranslation(project->position.x, project->position.y, project->position.z) };
	// ワールド変換行列を作成
	DirectX::XMFLOAT4X4 world;
	DirectX::XMStoreFloat4x4(&world, C * S * R * T);


    BulletModel->render(immediate_context, world, project->material_color, nullptr, replaced_pixel_shader);

	//DebugRenderer* debugRenderer = Lemur::Graphics::Graphics::Instance().GetDebugRenderer();
	//
	////衝突判定用のデバッグ円柱を描画
	//
	//debugRenderer->DrawSphere(project->position, project->radius, DirectX::XMFLOAT4(0, 0, 0, 1));

}

void GamePro_ProjectileStraightInputComponent::Update(GameObject* gameobj, float elapsedTime)
{
	GamePro_ProjectileStraight* project = dynamic_cast<GamePro_ProjectileStraight*> (gameobj);
	Lemur::Graphics::Graphics& graphics = Lemur::Graphics::Graphics::Instance();
	ID3D11DeviceContext* immediate_context = graphics.GetDeviceContext();

	//if (ImGui::TreeNode("pro"))
	//{
	//	ImGui::DragFloat3("pos", &project->position.x);
	//	ImGui::DragFloat3("rotate", &project->rotation.x);
	//	ImGui::DragFloat3("scale", &project->scale.x);
	//	ImGui::DragFloat("HP", &project->HP);
	//	ImGui::TreePop();
	//}
}

void GamePro_ProjectileStraightPhysicsComponent::Initialize(GameObject* gameobj)
{
	GamePro_ProjectileStraight* project = dynamic_cast<GamePro_ProjectileStraight*> (gameobj);


	project->HP = project->NumDelivery[4];
	project->scale = { 5.0f,5.0f,5.0f };
	project->position.y = 0.0f;
	project->attack = 1.0f;
	project->speed = project->NumFloatDelivery[2];

	project->damage = project->NumDelivery[3];
	project->radius = 1.0f;

	project->GiftAngle.x = project->NumFloatDelivery[0];
	project->GiftAngle.z = project->NumFloatDelivery[1];

	
}

void GamePro_ProjectileStraightPhysicsComponent::Update(GameObject* gameobj, float elapsedTime)
{
	GamePro_ProjectileStraight* project = dynamic_cast<GamePro_ProjectileStraight*> (gameobj);

	/*float px = project->GiftAngle.x;
	float pz = project->GiftAngle.z;
	DirectX::XMVECTOR vec_x = DirectX::XMLoadFloat(&px);
	DirectX::XMVECTOR vec_z = DirectX::XMLoadFloat(&pz);
	vec_x = DirectX::XMVector3Normalize(vec_x);
	vec_z = DirectX::XMVector3Normalize(vec_z);
	float floatX = DirectX::XMVectorGetX(vec_x);
	float floatZ = DirectX::XMVectorGetX(vec_z);
	project->position.x += floatX * project->speed;
	project->position.z += floatZ * project->speed;*/

	

	float cos = project->GiftAngle.x;
	float sin = project->GiftAngle.z;

	float angle = atan2f(sin, cos);

	project->position.x += cosf(angle) * project->speed;
	project->position.z += sinf(angle) * project->speed;
	
	project->projectEffect->SetPosition(project->handle, project->position);

	DirectX::XMFLOAT3 playerPos = project->player_->position; // プレイヤーの位置
	DirectX::XMFLOAT3 projPos = project->position; // 弾の位置

	// 位置差を計算
	float distance = sqrt(
		(playerPos.x - projPos.x) * (playerPos.x - projPos.x) +
		(playerPos.y - projPos.y) * (playerPos.y - projPos.y) +
		(playerPos.z - projPos.z) * (playerPos.z - projPos.z)
	);

	if (distance >= 40.0f)
	{
		project->projectEffect->Stop(project->handle);
		project->Death = true; // 弾を破棄

	}

	///当たり判定
	//DirectX::XMFLOAT3 p_p = project->position;
	//float p_r = project->radius;

	//for (auto& it : project->enemyList_)
	//{
	//	DirectX::XMFLOAT3 e_p = it->position;
	//	float e_r = it->radius;
	//	if (Collision::IntersectSphereVsSphere(p_p, p_r, e_p, e_r))
	//	{
	//		it->NumDelivery[10] = project->damage;
	//		project->HP -= 1;
	//	}
	//}

//	if (project->HP <= 0)
//	{
//<<<<<<< HEAD
//		DirectX::XMFLOAT3 e_p = it->position;
//		float e_r = it->radius;
//		if (Collision::IntersectSphereVsSphere(p_p, p_r, e_p, e_r))
//		{
//			if(it->NumDelivery[9] == 0)
//			{
//				it->NumDelivery[9] = project->damage;
//				project->HP -= 1;
//			}
//		}
//	}


	if (project->HP <= 0)
	{
		project->projectEffect->Stop(project->handle);
		project->Death = true;
	}
}
