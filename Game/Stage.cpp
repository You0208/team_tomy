#include "Stage.h"
#include "Lemur/Input/Mouse.h"
#include "Lemur/Graphics/Camera.h"

void StagePhysicsComponent::Initialize(GameObject* gameobj)
{
	Stage* stage = dynamic_cast<Stage*> (gameobj);
	stage->scale.x = stage->scale.y = stage->scale.z = 10.0f;
	stage->position.y = -0.75f;
	stage->position.x = 0.35f;

}

void StagePhysicsComponent::Update(GameObject* gameobj, float elapsedTime)
{

}

void StageGraphicsComponent::Initialize(GameObject* gameobj)
{
	Stage* stage = dynamic_cast<Stage*> (gameobj);
	Lemur::Graphics::Graphics& graphics = Lemur::Graphics::Graphics::Instance();

		stage->stageModel = ResourceManager::Instance().LoadModelResource(graphics.GetDevice(), ".\\resources\\Model\\stage\\foor_v003.fbx");

}

void StageGraphicsComponent::Update(GameObject* gameobj)
{

}

void StageGraphicsComponent::Render(GameObject* gameobj, float elapsedTime, ID3D11PixelShader* replaced_pixel_shader)
{
	Stage* stage = dynamic_cast<Stage*> (gameobj);

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
	DirectX::XMMATRIX S{ DirectX::XMMatrixScaling(stage->scale.x, stage->scale.y, stage->scale.z) };
	DirectX::XMMATRIX R{ DirectX::XMMatrixRotationRollPitchYaw(stage->rotation.x, stage->rotation.y, stage->rotation.z) };
	DirectX::XMMATRIX T{ DirectX::XMMatrixTranslation(stage->position.x, stage->position.y, stage->position.z) };
	// ワールド変換行列を作成
	DirectX::XMFLOAT4X4 world;
	DirectX::XMStoreFloat4x4(&world, C * S * R * T);

	stage->transform = world;

	if (stage->stageModel->animation_clips.size() > 0)
	{
		// アニメーション用
		int clip_index = 0;
		int frame_index = 0;
		static float animation_tick = 0;
#if 1
		animation& animation{ stage->stageModel->animation_clips.at(clip_index) };
		frame_index = static_cast<int>(animation_tick * animation.sampling_rate);
		if (frame_index > animation.sequence.size() - 1)
		{
			frame_index = 0;
			animation_tick = 0;
		}
		else
		{
			animation_tick += elapsedTime;
		}
		animation::keyframe& keyframe{ animation.sequence.at(frame_index) };
#else
		animation::keyframe keyframe;
		const animation::keyframe* keyframes[2]{
			&skinned_meshes[0]->animation_clips.at(0).sequence.at(40),
			&skinned_meshes[0]->animation_clips.at(0).sequence.at(80)
		};
		skinned_meshes[0]->blend_animations(keyframes, factors[2], keyframe);
		skinned_meshes[0]->update_animation(keyframe);

# endif
		stage->stageModel->render(immediate_context, world, stage->material_color, &keyframe, replaced_pixel_shader);
	}
	else
	{
		stage->stageModel->render(immediate_context, world, stage->material_color, nullptr, replaced_pixel_shader);
	}
}

// 入力処理
void StageInputComponent::Update(GameObject* gameobj, float elapsedTime)
{
	//GamePad& gamePad = Input::Instance().GetGamePad();
	//float ax = gamePad.GetAxisRX();
	//float ay = gamePad.GetAxisRY();

	//float lx = gamePad.GetAxisLX();
	//float ly = gamePad.GetAxisLY();

	///*gameobj.rotation.y += lx * elapsedTime;
	//gameobj.rotation.y += ly * elapsedTime;*/

	//Mouse& mouse = Input::Instance().GetMouse();

	//gameobj.rotation.y = mouse.GetPositionX() + SCREEN_WIDTH / 2;
	//gameobj.rotation.y = mouse.GetPositionY() + SCREEN_HEIGHT / 2;

	////ビューポート
	//D3D11_VIEWPORT viewport;
	//UINT numViewports = 1;
	////dc->RSGetViewports(&numViewports, &viewport);

	////変換行列
	//DirectX::XMMATRIX View = Camera::Instance().GetViewMatrix();
	//DirectX::XMMATRIX Projection = Camera::Instance().GetProjectionMatrix();
	//DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();


	//if (mouse.GetButtonDown() == mouse.BTN_LEFT)
	//{
	//	//マウスカーソル座標
	////DirectX::XMFLOAT3 screenPosition;
	////screenPosition.x = static_cast<float>(mouse.GetOldPositionX());
	////screenPosition.y = static_cast<float>(mouse.GetOldPositionY());
	////screenPosition.z = 0.0f;
	////
	////DirectX::XMVECTOR WorldPosition0 = DirectX::XMVector3Unproject(
	////	DirectX::XMLoadFloat3(&screenPosition),
	////	viewport.TopLeftX, viewport.TopLeftY,
	////	viewport.Width, viewport.Height,
	////	viewport.MinDepth, viewport.MaxDepth,
	////	Projection, View, World
	////);
	////
	////DirectX::XMVECTOR WorldPosition1 = DirectX::XMVector3Unproject(
	////	DirectX::XMLoadFloat3(&screenPosition),
	////	viewport.TopLeftX, viewport.TopLeftY,
	////	viewport.Width, viewport.Height,
	////	viewport.MinDepth, viewport.MaxDepth,
	////	Projection, View, World
	////);
	////
	////DirectX::XMFLOAT3 worldPosition0;
	////DirectX::XMFLOAT3 worldPosition1;
	////DirectX::XMStoreFloat3(&worldPosition0, WorldPosition0);
	////DirectX::XMStoreFloat3(&worldPosition1, WorldPosition1);
	////
	////DirectX::XMFLOAT3 start = worldPosition0;
	////DirectX::XMFLOAT3 end = worldPosition1;
	////HitResult hit;

	////if (Stage::Instance().RayCast(start, end, hit))
	//	{

	//	}

	//}


}
