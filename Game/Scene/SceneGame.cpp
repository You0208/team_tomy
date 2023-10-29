#include "SceneGame.h"

#include <Game/Easing.h>

#include "GambleScene.h"
#include "imgui.h"
#include "Game/Manager/CharacterManager.h"
#include "Game/Manager/ColliderManager.h"
#include "Game/Manager/EnemyManager.h"
#include "Game/Skill/AttackSkillDerived.h"
#include "Game/Skill/BadSkillDerived.h"
#include "Game/Skill/SkillDerived.h"
#include "Lemur/Effekseer/EffekseerManager.h"
#include "Lemur/Graphics/Camera.h"
#include "Quest.h"
#include "ResultScene.h"
#include "TitleScene.h"
#include "Game/Enemy/BossSpider.h"
#include "Game/Enemy/SmallSpider.h"
#include "..\Enemy\Spider_ABC.h"
#include "Game/Enemy/Spider_DEF.h"
#include "Game/Enemy/Spider_GH.h"
#include "Game/StateMachine/StateMachine.h"
#include "Lemur/Scene/SceneManager.h"

#include "Lemur/Audio/AudioManager.h"
extern QuestPattern quest_pattern;
extern int wave_count;

extern bool tutorial;
// todo ポーズ画面
void GameScene::Initialize()
{
	Lemur::Graphics::Graphics& graphics = Lemur::Graphics::Graphics::Instance();

	// シェーダー関連
	{
		SetState();

		HRESULT hr{ S_OK };
		// シーン定数バッファオブジェクトを生成
		{
			D3D11_BUFFER_DESC buffer_desc{};
			buffer_desc.Usage = D3D11_USAGE_DEFAULT;// 読み取りおよび書き込みがどのように行われるか
			buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;// バッファがパイプラインにどのようにバインド(結びつけ)されるかを特定
			buffer_desc.CPUAccessFlags = 0;// CPU アクセス フラグ（CPU アクセスが必要ない場合は 0）
			buffer_desc.MiscFlags = 0;// その他のフラグ（未使用に場合は0）
			buffer_desc.StructureByteStride = 0;//バッファが構造化バッファを表す場合の、バッファ構造内の各要素のサイズ
			{
				buffer_desc.ByteWidth = sizeof(scene_constants);// バッファのサイズ 
				hr = graphics.GetDevice()->CreateBuffer(&buffer_desc, nullptr, constant_buffers[static_cast<size_t>(CONSTANT_BUFFER::SCENE)].GetAddressOf());
				_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
			}
			{ // フォグ
				buffer_desc.ByteWidth = sizeof(fog_constants);
				hr = graphics.GetDevice()->CreateBuffer(&buffer_desc, nullptr, constant_buffers[static_cast<size_t>(CONSTANT_BUFFER::FOG)].GetAddressOf());
				_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
			}
			{ // 距離フォグ
				buffer_desc.ByteWidth = sizeof(dis_fog_constants);
				hr = graphics.GetDevice()->CreateBuffer(&buffer_desc, nullptr, constant_buffers[static_cast<size_t>(CONSTANT_BUFFER::D_FOG)].GetAddressOf());
				_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
			}
			{ // ディゾルブ
				buffer_desc.ByteWidth = sizeof(dissolve_constants);
				hr = graphics.GetDevice()->CreateBuffer(&buffer_desc, nullptr, constant_buffers[static_cast<size_t>(CONSTANT_BUFFER::MASK)].GetAddressOf());
				_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
			}
			{ // PBR
				buffer_desc.ByteWidth = sizeof(adjust_constants);
				hr = graphics.GetDevice()->CreateBuffer(&buffer_desc, nullptr, constant_buffers[static_cast<size_t>(CONSTANT_BUFFER::PBR)].GetAddressOf());
				_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
			}
			{ // ライト
				buffer_desc.ByteWidth = sizeof(light_constants);
				hr = graphics.GetDevice()->CreateBuffer(&buffer_desc, nullptr, light_constant_buffer.GetAddressOf());
				_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
			}
		}


		// FOG
		bit_block_transfer[static_cast<size_t>(BIT_BLOCK::FOG)] = std::make_unique<fullscreen_quad>(graphics.GetDevice());
		framebuffers[static_cast<size_t>(FRAME_BUFFER::FOG_1)] = std::make_unique<framebuffer>(graphics.GetDevice(), SCREEN_WIDTH, SCREEN_HEIGHT, true);
		framebuffers[static_cast<size_t>(FRAME_BUFFER::FOG_2)] = std::make_unique<framebuffer>(graphics.GetDevice(), SCREEN_WIDTH / 1, SCREEN_HEIGHT / 1, false);
		create_ps_from_cso(graphics.GetDevice(), "Shader/fog_ps.cso", pixel_shaders[static_cast<size_t>(PS::FOG)].GetAddressOf());
		create_ps_from_cso(graphics.GetDevice(), "Shader/final_pass_ps.cso", pixel_shaders[static_cast<size_t>(PS::FINAL)].GetAddressOf());

		// SHADOW
		double_speed_z = std::make_unique<shadow_map>(graphics.GetDevice(), shadowmap_width, shadowmap_height);

		// dissolve
		load_texture_from_file(graphics.GetDevice(), L".\\resources\\Image\\dissolve_animation.png", noise.GetAddressOf(), graphics.GetTexture2D());//TODO

		//TODO 実験用
		create_ps_from_cso(graphics.GetDevice(), "./Shader/player_ps.cso", Try.GetAddressOf());
		
		// シェーダーの読み込み
		{
			// sprite用デフォルト描画シェーダー（ディゾルブ）
			{
				D3D11_INPUT_ELEMENT_DESC input_element_desc[]
				{
					{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				};
				create_vs_from_cso(graphics.GetDevice(),
					"./Shader/sprite_dissolve_vs.cso",
					sprite_vertex_shader.GetAddressOf(),
					sprite_input_layout.GetAddressOf(),
					input_element_desc,
					ARRAYSIZE(input_element_desc));
				create_ps_from_cso(graphics.GetDevice(),
					"./Shader/sprite_dissolve_ps.cso",
					sprite_pixel_shader.GetAddressOf());
			}
		}
	}

	// ゲーム部分
	{

		// プレイヤーをキャラクターマネージャから持ってくる
		player = CharacterManager::Instance().GetPlayer();
		// プレイヤー初期処理
		if (!player->is_initialize)
			player->Initialize();

		player->SkillInit();
		// プレイヤーにシェーダーをセット
		player->SetPixelShader(Try.Get());
		// プレイヤーをキャラクターマネージャにセット
		CharacterManager::Instance().SetPlayer(player);

		ColliderManager::Instance().SetCollider(player);


		CreateEnemy_KARI();

		stage = std::make_unique<Stage>();
		stage->Init();

		/*------------------ UI関係 --------------------*/
		player_hp_gauge = std::make_unique<sprite>(graphics.GetDevice(), L".\\resources\\Image\\HPui_02.png");
		player_hp_gauge_zabuton = std::make_unique<sprite>(graphics.GetDevice(), L".\\resources\\Image\\HPui.png");
		enemy_hp_gauge = std::make_unique<sprite>(graphics.GetDevice(), L".\\resources\\Image\\enemy_HPui_02.png");
		enemy_hp_gauge_zabuton = std::make_unique<sprite>(graphics.GetDevice(), L".\\resources\\Image\\enemy_HPui.png");

		pause_back = std::make_unique<sprite>(graphics.GetDevice(), L".\\resources\\Image\\pose_back.png");
		pause_title = std::make_unique<sprite>(graphics.GetDevice(), L".\\resources\\Image\\pose_title.png");
		pause_return = std::make_unique<sprite>(graphics.GetDevice(), L".\\resources\\Image\\pose_return.png");
		pause_select = std::make_unique<sprite>(graphics.GetDevice(), L".\\resources\\Image\\pose_selping.png");

		Method_A_Button = std::make_unique<sprite>(graphics.GetDevice(), L".\\resources\\Image\\A.png");
		Method_B_Button = std::make_unique<sprite>(graphics.GetDevice(), L".\\resources\\Image\\B.png");
		Method_Y_Button = std::make_unique<sprite>(graphics.GetDevice(), L".\\resources\\Image\\Y.png");
		Method_LBRB_Button = std::make_unique<sprite>(graphics.GetDevice(), L".\\resources\\Image\\LBRB.png");

		tutorial_01 = std::make_unique<sprite>(graphics.GetDevice(), L".\\resources\\Image\\tutorial\\fight_page1.png");
		tutorial_02 = std::make_unique<sprite>(graphics.GetDevice(), L".\\resources\\Image\\tutorial\\fight_page2.png");
		tutorial_03 = std::make_unique<sprite>(graphics.GetDevice(), L".\\resources\\Image\\tutorial\\fight_page3.png");
	}

	// ライト
	{	// ディレクションライトのデータを設定する
	// ライトは右側から当たっている
		light.dirDirection.x = 1.0f;
		light.dirDirection.y = -1.0f;
		light.dirDirection.z = -1.0f;
		DirectX::XMFLOAT3 normalizedDirection{};
		DirectX::XMVECTOR directionVector = DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(normalizedDirection.x, normalizedDirection.y, normalizedDirection.z));
		directionVector = DirectX::XMVector3Normalize(directionVector);
		DirectX::XMStoreFloat3(&normalizedDirection, directionVector);

		light.dirDirection = { normalizedDirection.x,normalizedDirection.y,normalizedDirection.z,0.0f };

		// ライトのカラーは白
		light.dirColor.x = 0.5f;
		light.dirColor.y = 0.5f;
		light.dirColor.z = 0.5f;

		Camera& camera = Camera::Instance();

		// 視点の位置を設定する
		DirectX::XMFLOAT3 eyePos;
		DirectX::XMStoreFloat3(&eyePos, camera.GetEye());
		light.eyePos.x = eyePos.x;
		light.eyePos.y = eyePos.y;
		light.eyePos.z = eyePos.z;

		// 環境光
		light.ambientLight.x = 0.3f;
		light.ambientLight.y = 0.3f;
		light.ambientLight.z = 0.3f;

		// step-2 ポイントライトの初期座標を設定する
		light.ptPosition.x = -17.3f;
		light.ptPosition.y = 18.8f;
		light.ptPosition.z = 41.4f;

		// step-3 ポイントライトの初期カラーを設定する
		light.ptColor.x = 1.0f;
		light.ptColor.y = 1.0f;
		light.ptColor.z = 1.0f;

		// step-4 ポイントライトの影響範囲を設定する
		light.ptRange.x = 100.0f;

	}

	Lemur::Audio::AudioManager::Instance().play_bgm(Lemur::Audio::BGM::TITLE,true);
}

void GameScene::Finalize()
{
	//player->Delete();
	//delete player;
	//エネミー終了
	Lemur::Audio::AudioManager::Instance().stop_BGM(Lemur::Audio::BGM::TITLE);
	EnemyManager::Instance().Clear();
}

void GameScene::Update(HWND hwnd, float elapsedTime)
{
	Pause();

	Camera& camera = Camera::Instance();
	camera.Update(elapsedTime);

	if (is_pause)return;

	if (wave_count == 1) {

		switch (easing_step)
		{

		case 0: // 1回目のイージング
			if (!tutorial) break;

			if (!EasingTutorial(SCREEN_WIDTH, 0.0f, hide_stop_time_ms, easing_time_ms))
				return;


			// 一回目やったら
			if (tutorial_i == 0)
			{
				// 一回目のイージング終了したらタイマーをリセットしてステップを進める。
				ResetEasingTime();

				easing_step++;
				return;

			}
			// ボタン押されん限り進めん
			else if (Input::Instance().GetGamePad().GetButtonDown() & GamePad::BTN_A ||
				Input::Instance().GetMouse().GetButtonDown() & Mouse::BTN_LEFT)
			{
				// 一回目のイージング終了したらタイマーをリセットしてステップを進める。
				ResetEasingTime();
				stop_time_ms = 0.0f;
				easing_step++;
				return;
			}
			return;
			break;

		case 1: // 2回目のイージング
			if (!EasingTutorial(0.0f, -SCREEN_WIDTH, stop_time_ms, easing_time_ms))
				return;
			ResetEasingTime();

			tutorial_end[tutorial_i] = true;
			tutorial_i++;
			if (tutorial_i > 2)
			{
				easing_step++;
				stop_time_ms = 3.0f;
				return;
			}
			else
			{
				easing_step = 0;
				return;
			}
			break;
		}
	}


	timer += elapsedTime;

	// エフェクト更新処理
	EffectManager::Instance().Update(elapsedTime);

	// 敵の更新処理
	EnemyManager::Instance().Update(elapsedTime);


	player->Update(elapsedTime);


	//ColliderManager::Instance().Update();

	QuestClear();
	QuestFailed();


}

void GameScene::Render(float elapsedTime)
{
	Camera& camera = Camera::Instance();
	Lemur::Graphics::Graphics& graphics = Lemur::Graphics::Graphics::Instance();
	ID3D11DeviceContext* immediate_context = graphics.GetDeviceContext();
	ID3D11RenderTargetView* render_target_view = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* depth_stencil_view = graphics.GetDepthStencilView();

	camera.SetPerspectiveFov(immediate_context);
	SetUpRendering();

	immediate_context->OMSetBlendState(blend_states[static_cast<size_t>(BLEND_STATE::ALPHA)].Get(), nullptr, 0xFFFFFFFF);
	// 深度ステンシルステートの設定
	immediate_context->OMSetDepthStencilState(depth_stencil_states[static_cast<size_t>(DEPTH_STATE::ZT_ON_ZW_ON)].Get(), 0);
	immediate_context->RSSetState(rasterizer_states[static_cast<size_t>(RASTER_STATE::SOLID)].Get());

	immediate_context->OMSetBlendState(blend_states[static_cast<size_t>(BLEND_STATE::ALPHA)].Get(), nullptr, 0xFFFFFFFF);
	immediate_context->OMSetDepthStencilState(depth_stencil_states[static_cast<size_t>(DEPTH_STATE::ZT_ON_ZW_ON)].Get(), 0);
	immediate_context->RSSetState(rasterizer_states[static_cast<size_t>(RASTER_STATE::SOLID)].Get());
	
	// SHADOW : make shadow map
	if (enableShadow)
	{
		using namespace DirectX;

		const float aspect_ratio = double_speed_z->viewport.Width / double_speed_z->viewport.Height;
		XMVECTOR F{ camera.GetFocus() };
		XMVECTOR E{ F - XMVector3Normalize(XMLoadFloat4(&light_direction)) * light_view_distance };
		XMVECTOR U{ XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f) };
		XMMATRIX V{ XMMatrixLookAtLH(E, F, U) };
		XMMATRIX P{ XMMatrixOrthographicLH(light_view_size * aspect_ratio, light_view_size, light_view_near_z, light_view_far_z) };

		DirectX::XMStoreFloat4x4(&scene_constants.view_projection, V * P);
		scene_constants.light_view_projection = scene_constants.view_projection;
		immediate_context->UpdateSubresource(constant_buffers[static_cast<size_t>(CONSTANT_BUFFER::SCENE)].Get(), 0, 0, &scene_constants, 0, 0);
		immediate_context->VSSetConstantBuffers(static_cast<size_t>(CONSTANT_BUFFER_R::SCENE), 1, constant_buffers[static_cast<size_t>(CONSTANT_BUFFER::SCENE)].GetAddressOf());

		double_speed_z->clear(immediate_context, 1.0f);
		double_speed_z->activate(immediate_context);

		ID3D11PixelShader* null_pixel_shader{ NULL };
		player->Render(elapsedTime, &null_pixel_shader);
		EnemyManager::Instance().Render(elapsedTime, true);
		double_speed_z->deactivate(immediate_context);
	}

	//TODO 要修正
#if 1
	{
		D3D11_VIEWPORT viewport;
		UINT num_viewports{ 1 };
		immediate_context->RSGetViewports(&num_viewports, &viewport);

		float aspect_ratio{ viewport.Width / viewport.Height };
		DirectX::XMMATRIX P{ DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(30), aspect_ratio, 0.1f, 1000.0f) };

		DirectX::XMVECTOR eye = camera.GetEye();
		DirectX::XMVECTOR focus = camera.GetFocus();
		DirectX::XMVECTOR up{ DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f) };
		DirectX::XMMATRIX V{ DirectX::XMMatrixLookAtLH(eye, focus, up) };

		DirectX::XMStoreFloat4x4(&scene_constants.view_projection, V * P);
		scene_constants.light_direction = light_direction;
		
		DirectX::XMStoreFloat4(&camera_position, eye);

		scene_constants.camera_position = camera_position;
		if (enableFog)
		{
			// FOG
			DirectX::XMStoreFloat4x4(&scene_constants.inverse_projection, DirectX::XMMatrixInverse(NULL, P));
			DirectX::XMStoreFloat4x4(&scene_constants.inv_view_projection, DirectX::XMMatrixInverse(NULL, V * P));
			scene_constants.time = high_resolution_timer::Instance().time_stamp();
		}
	}
#else
	// 基本バッファー更新
	DirectX::XMStoreFloat4x4(&scene_constants.view_projection, camera.GetViewMatrix() * camera.GetProjectionMatrix());
	scene_constants.light_direction = light_direction;
	DirectX::XMStoreFloat4(&scene_constants.camera_position, camera.GetEye());

	// FOG
	DirectX::XMStoreFloat4x4(&scene_constants.inverse_projection, DirectX::XMMatrixInverse(NULL, camera.GetProjectionMatrix()));
	DirectX::XMStoreFloat4x4(&scene_constants.inverse_view_projection, DirectX::XMMatrixInverse(NULL, camera.GetProjectionMatrix() * camera.GetViewMatrix()));
	scene_constants.time = high_resolution_timer::Instance().time_stamp();
#endif

	// 定数バッファの更新
	{
		immediate_context->UpdateSubresource(constant_buffers[static_cast<size_t>(CONSTANT_BUFFER::SCENE)].Get(), 0, 0, &scene_constants, 0, 0);
		immediate_context->VSSetConstantBuffers(static_cast<size_t>(CONSTANT_BUFFER_R::SCENE), 1, constant_buffers[static_cast<size_t>(CONSTANT_BUFFER::SCENE)].GetAddressOf());
		immediate_context->PSSetConstantBuffers(static_cast<size_t>(CONSTANT_BUFFER_R::SCENE), 1, constant_buffers[static_cast<size_t>(CONSTANT_BUFFER::SCENE)].GetAddressOf());

		// SKYMAP
		if (enableSkyMap)
		{
			DirectX::XMStoreFloat4x4(&scene_constants.inv_view_projection, DirectX::XMMatrixInverse(NULL, camera.GetViewMatrix() * camera.GetProjectionMatrix()));
		}
		// FOG
		immediate_context->UpdateSubresource(constant_buffers[static_cast<size_t>(CONSTANT_BUFFER::FOG)].Get(), 0, 0, &fog_constants, 0, 0);
		immediate_context->PSSetConstantBuffers(static_cast<size_t>(CONSTANT_BUFFER_R::FOG), 1, constant_buffers[static_cast<size_t>(CONSTANT_BUFFER::FOG)].GetAddressOf());

		// ポイントライト
		immediate_context->UpdateSubresource(light_constant_buffer.Get(), 0, 0, &light, 0, 0);
		immediate_context->VSSetConstantBuffers(static_cast<size_t>(CONSTANT_BUFFER_R::LIGHT), 1, light_constant_buffer.GetAddressOf());
		immediate_context->PSSetConstantBuffers(static_cast<size_t>(CONSTANT_BUFFER_R::LIGHT), 1, light_constant_buffer.GetAddressOf());

		// PBR
		immediate_context->UpdateSubresource(constant_buffers[static_cast<size_t>(CONSTANT_BUFFER::PBR)].Get(), 0, 0, &pbr, 0, 0);
		immediate_context->VSSetConstantBuffers(static_cast<size_t>(CONSTANT_BUFFER_R::PBR), 1, constant_buffers[static_cast<size_t>(CONSTANT_BUFFER::PBR)].GetAddressOf());
		immediate_context->PSSetConstantBuffers(static_cast<size_t>(CONSTANT_BUFFER_R::PBR), 1, constant_buffers[static_cast<size_t>(CONSTANT_BUFFER::PBR)].GetAddressOf());

		// ディゾルブ
		dissolve_constants dissolve{};
		dissolve.parameters.x = dissolve_value;
		immediate_context->UpdateSubresource(constant_buffers[static_cast<size_t>(CONSTANT_BUFFER::MASK)].Get(), 0, 0, &dissolve, 0, 0);
		immediate_context->VSSetConstantBuffers(static_cast<size_t>(CONSTANT_BUFFER_R::MASK), 1, constant_buffers[static_cast<size_t>(CONSTANT_BUFFER::MASK)].GetAddressOf());
		immediate_context->PSSetConstantBuffers(static_cast<size_t>(CONSTANT_BUFFER_R::MASK), 1, constant_buffers[static_cast<size_t>(CONSTANT_BUFFER::MASK)].GetAddressOf());

		// FOG
		dis_fog_constants fogs{};
		fogs.fog_color = fog_color;
		fogs.fog_range = fog_range;
		immediate_context->UpdateSubresource(constant_buffers[static_cast<size_t>(CONSTANT_BUFFER::D_FOG)].Get(), 0, 0, &fogs, 0, 0);
		immediate_context->VSSetConstantBuffers(static_cast<size_t>(CONSTANT_BUFFER_R::D_FOG), 1, constant_buffers[static_cast<size_t>(CONSTANT_BUFFER::D_FOG)].GetAddressOf());
		immediate_context->PSSetConstantBuffers(static_cast<size_t>(CONSTANT_BUFFER_R::D_FOG), 1, constant_buffers[static_cast<size_t>(CONSTANT_BUFFER::D_FOG)].GetAddressOf());
	}

	if (enableSkyMap)
	{
		// SKYMAP
		immediate_context->OMSetDepthStencilState(depth_stencil_states[static_cast<size_t>(DEPTH_STATE::ZT_OFF_ZW_OFF)].Get(), 0);
		immediate_context->RSSetState(rasterizer_states[static_cast<size_t>(RASTER_STATE::CULL_NONE)].Get());
		bit_block_transfer[static_cast<size_t>(BIT_BLOCK::SKY)]->blit(immediate_context, skymap.GetAddressOf(), 0, 1, pixel_shaders[static_cast<size_t>(PS::SKY)].Get());
	}

	// テクスチャをセット
	{
		// PBR実験用
		immediate_context->PSSetShaderResources(9/*slot(1番にセットします)*/, 1, noise.GetAddressOf());//TODO
		immediate_context->PSSetShaderResources(10/*slot(1番にセットします)*/, 1, BaseColor.GetAddressOf());//TODO
		immediate_context->PSSetShaderResources(11/*slot(1番にセットします)*/, 1, Normal.GetAddressOf());//TODO
		immediate_context->PSSetShaderResources(12/*slot(1番にセットします)*/, 1, Roughness.GetAddressOf());//TODO
		//for (auto enemy : EnemyManager::Instance().GetEnemies())
		//{
		//	immediate_context->PSSetShaderResources(0, 13, enemy->spider_color.GetAddressOf());
		//	immediate_context->PSSetShaderResources(0, 14, enemy->spider_normal.GetAddressOf());
		//	immediate_context->PSSetShaderResources(0, 15, enemy->spider_roughness.GetAddressOf());
		//	immediate_context->PSSetShaderResources(0, 16, enemy->spider_metalness.GetAddressOf());
		//}
		// シャドウ
		immediate_context->PSSetShaderResources(8, 1, double_speed_z->shader_resource_view.GetAddressOf());
	}

	if (enableBloom)
	{
		framebuffers[static_cast<size_t>(FRAME_BUFFER::BLOOM)]->clear(immediate_context);
		framebuffers[static_cast<size_t>(FRAME_BUFFER::BLOOM)]->activate(immediate_context);
	}
	if (enableFog)
	{
		framebuffers[static_cast<size_t>(FRAME_BUFFER::FOG_1)]->clear(immediate_context);
		framebuffers[static_cast<size_t>(FRAME_BUFFER::FOG_1)]->activate(immediate_context);
	}
	//3D描画
	{
		immediate_context->OMSetDepthStencilState(depth_stencil_states[static_cast<size_t>(DEPTH_STATE::ZT_ON_ZW_ON)].Get(), 0);
		immediate_context->RSSetState(rasterizer_states[static_cast<size_t>(RASTER_STATE::CULL_NONE)].Get());

#if 0
		immediate_context->OMSetDepthStencilState(depth_stencil_states[static_cast<size_t>(DEPTH_STATE::ZT_ON_ZW_ON)].Get(), 0);
		immediate_context->RSSetState(rasterizer_states[static_cast<size_t>(RASTER_STATE::CULL_NONE)].Get());
		{
			const DirectX::XMFLOAT4X4 coordinate_system_transforms[]{
				{ -1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 },	// 0:RHS Y-UP
				{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 },		// 1:LHS Y-UP
				{ -1, 0, 0, 0, 0, 0, -1, 0, 0, 1, 0, 0, 0, 0, 0, 1 },	// 2:RHS Z-UP
				{ 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1 },		// 3:LHS Z-UP
			};
#if 1
			float scale_factor = 1.0f; // To change the units from centimeters to meters, set 'scale_factor' to 0.01.
#else
			const float scale_factor = 0.01f; // To change the units from centimeters to meters, set 'scale_factor' to 0.01.
#endif
			// STATIC_BATCHING
			DirectX::XMMATRIX C{ DirectX::XMLoadFloat4x4(&coordinate_system_transforms[0])* DirectX::XMMatrixScaling(scale_factor, scale_factor, scale_factor) };
			DirectX::XMMATRIX S{ DirectX::XMMatrixScaling(scaling.x, scaling.y, scaling.z) };
			DirectX::XMMATRIX R{ DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z) };
			DirectX::XMMATRIX T{ DirectX::XMMatrixTranslation(translation.x, translation.y, translation.z) };
			DirectX::XMFLOAT4X4 world;
			DirectX::XMStoreFloat4x4(&world, C * S * R * T);

			scale_factor = 0.01f; // To change the units from centimeters to meters, set 'scale_factor' to 0.01.
			C = { DirectX::XMLoadFloat4x4(&coordinate_system_transforms[0]) * DirectX::XMMatrixScaling(scale_factor, scale_factor, scale_factor) };
			S = { DirectX::XMMatrixScaling(scaling.x, scaling.y, scaling.z) };
			R = { DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z) };
			T = { DirectX::XMMatrixTranslation(translation.x, translation.y, translation.z) };
			DirectX::XMStoreFloat4x4(&world, C * S * R * T);
			skinned_meshes[0]->render(immediate_context,{ -0.01f, 0, 0, 0, 0, 0.01f, 0, 0, 0, 0, 0.01f, 0, 0, 0, 0, 1 }, material_color, nullptr, Try.Get());
			skinned_meshes[2]->render(immediate_context,{ -0.01f, 0, 0, 0, 0, 0.01f, 0, 0, 0, 0, 0.01f, 0, 0, 0, 0, 1 }, material_color, nullptr, Try.Get());
		}
#endif
		// プレイヤー描画
		player->Render(elapsedTime);
		EnemyManager::Instance().Render(elapsedTime);
		stage->Render();
	}

	// 3Dエフェクト描画
	{
		DirectX::XMFLOAT4X4 view{};
		DirectX::XMFLOAT4X4 projection{};

		DirectX::XMStoreFloat4x4(&view, camera.GetViewMatrix());
		DirectX::XMStoreFloat4x4(&projection, camera.GetProjectionMatrix());

		EffectManager::Instance().Render(view, projection);

		//player->DrawDebugPrimitive();
		//EnemyManager::Instance().DrawDebugPrimitive();
	}


	if (enableFog)	framebuffers[static_cast<size_t>(FRAME_BUFFER::FOG_1)]->deactivate(immediate_context);
	if (enableFog)
	{
		// FOG
		framebuffers[static_cast<size_t>(FRAME_BUFFER::FOG_2)]->clear(immediate_context);
		framebuffers[static_cast<size_t>(FRAME_BUFFER::FOG_2)]->activate(immediate_context);
		immediate_context->OMSetDepthStencilState(depth_stencil_states[static_cast<size_t>(DEPTH_STATE::ZT_OFF_ZW_OFF)].Get(), 0);
		immediate_context->RSSetState(rasterizer_states[static_cast<size_t>(RASTER_STATE::CULL_NONE)].Get());
		immediate_context->OMSetBlendState(blend_states[static_cast<size_t>(BLEND_STATE::NONE)].Get(), nullptr, 0xFFFFFFFF);
		bit_block_transfer[static_cast<size_t>(BIT_BLOCK::FOG)]->blit(immediate_context, framebuffers[static_cast<size_t>(FRAME_BUFFER::FOG_1)]->shader_resource_views[1].GetAddressOf()/*Depth*/, 0, 1, pixel_shaders[0].Get());
		framebuffers[static_cast<size_t>(FRAME_BUFFER::FOG_2)]->deactivate(immediate_context);

		immediate_context->OMSetDepthStencilState(depth_stencil_states[static_cast<size_t>(DEPTH_STATE::ZT_OFF_ZW_OFF)].Get(), 0);
		immediate_context->RSSetState(rasterizer_states[static_cast<size_t>(RASTER_STATE::CULL_NONE)].Get());
		ID3D11ShaderResourceView* shader_resource_views[]{ framebuffers[static_cast<size_t>(FRAME_BUFFER::FOG_1)]->shader_resource_views[0].Get(), framebuffers[static_cast<size_t>(FRAME_BUFFER::FOG_2)]->shader_resource_views[0].Get() };
		bit_block_transfer[static_cast<size_t>(BIT_BLOCK::FOG)]->blit(immediate_context, shader_resource_views, 0, _countof(shader_resource_views), pixel_shaders[static_cast<size_t>(PS::FINAL)].Get());
	}

	if (enableBloom)	framebuffers[static_cast<size_t>(FRAME_BUFFER::BLOOM)]->deactivate(immediate_context);
	if (enableBloom)
	{
		framebuffers[static_cast<size_t>(FRAME_BUFFER::BLOOM)]->deactivate(immediate_context);
		// BLOOM
		bloomer->make(immediate_context, framebuffers[static_cast<size_t>(FRAME_BUFFER::BLOOM)]->shader_resource_views[0].Get());

		immediate_context->OMSetDepthStencilState(depth_stencil_states[static_cast<size_t>(DEPTH_STATE::ZT_OFF_ZW_OFF)].Get(), 0);
		immediate_context->RSSetState(rasterizer_states[static_cast<size_t>(RASTER_STATE::CULL_NONE)].Get());
		immediate_context->OMSetBlendState(blend_states[static_cast<size_t>(BLEND_STATE::ALPHA)].Get(), nullptr, 0xFFFFFFFF);
		ID3D11ShaderResourceView* shader_resource_views[] =
		{
			framebuffers[static_cast<size_t>(FRAME_BUFFER::BLOOM)]->shader_resource_views[0].Get(),
			bloomer->shader_resource_view(),
		};
		bit_block_transfer[static_cast<size_t>(BIT_BLOCK::BLOOM)]->blit(immediate_context, shader_resource_views, 0, 2, pixel_shaders[static_cast<size_t>(PS::BLOOM)].Get());
	}


	// 2D描画
#if 1
	// sprite描画
	{
		if (dummy_sprite)
		{
			immediate_context->OMSetDepthStencilState(depth_stencil_states[static_cast<size_t>(DEPTH_STATE::ZT_OFF_ZW_OFF)].Get(), 0);
			immediate_context->RSSetState(rasterizer_states[static_cast<size_t>(RASTER_STATE::CULL_NONE)].Get());
			immediate_context->OMSetBlendState(blend_states[static_cast<size_t>(BLEND_STATE::ALPHA)].Get(), nullptr, 0xFFFFFFFF);

			immediate_context->IASetInputLayout(sprite_input_layout.Get());
			immediate_context->VSSetShader(sprite_vertex_shader.Get(), nullptr, 0);
			immediate_context->PSSetShader(sprite_pixel_shader.Get(), nullptr, 0);
			immediate_context->PSSetSamplers(0, 1, sampler_states[static_cast<size_t>(SAMPLER_STATE::LINEAR)].GetAddressOf());
			immediate_context->PSSetShaderResources(1, 1, mask_texture.GetAddressOf());

			// 定数バッファの更新（ディゾルブ）
			{
				//dissolve_constants dissolve{};
				//dissolve.parameters.x = dissolve_value;
				//immediate_context->UpdateSubresource(dissolve_constant_buffer.Get(), 0, 0, &dissolve, 0, 0);
				//immediate_context->VSSetConstantBuffers(3, 1, dissolve_constant_buffer.GetAddressOf());
				//immediate_context->PSSetConstantBuffers(3, 1, dissolve_constant_buffer.GetAddressOf());
			}
			//dummy_sprite->render(immediate_context, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		}
		immediate_context->OMSetDepthStencilState(depth_stencil_states[static_cast<size_t>(DEPTH_STATE::ZT_OFF_ZW_OFF)].Get(), 0);
		immediate_context->RSSetState(rasterizer_states[static_cast<size_t>(RASTER_STATE::CULL_NONE)].Get());
		immediate_context->OMSetBlendState(blend_states[static_cast<size_t>(BLEND_STATE::ALPHA)].Get(), nullptr, 0xFFFFFFFF);

		UIRender();
		EnemyManager::Instance().DamageRender();
		//camera.RenderEnemyHP(player_hp_gauge.get());

		PauseRender();

		if (wave_count == 1)
		{
			if (!tutorial_end[2])
				tutorial_03->render(graphics.GetDeviceContext(), spr_tutorial_pos.x, spr_tutorial_pos.y, SCREEN_WIDTH, SCREEN_HEIGHT);
			if (!tutorial_end[1])
				tutorial_02->render(graphics.GetDeviceContext(), spr_tutorial_pos.x, spr_tutorial_pos.y, SCREEN_WIDTH, SCREEN_HEIGHT);
			if (!tutorial_end[0])
				tutorial_01->render(graphics.GetDeviceContext(), spr_tutorial_pos.x, spr_tutorial_pos.y, SCREEN_WIDTH, SCREEN_HEIGHT);
		}
	}
#endif

	//TODO debug
	{
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 projection;
		DirectX::XMStoreFloat4x4(&view, camera.GetViewMatrix());
		DirectX::XMStoreFloat4x4(&projection, camera.GetProjectionMatrix());
		graphics.GetDebugRenderer()->Render(immediate_context, view, projection);
	}

	// 2Dデバッグ描画　
	{
		DebugImGui();
		stage->DrawImGui();
		camera.DrawDebug();
	}

}

void GameScene::DebugImGui()
{

}

void GameScene::CreateEnemy_KARI()
{
	// エネミー初期化
	EnemyManager& enemyManager = EnemyManager::Instance();

	//quest_pattern = BOSS;
	switch (quest_pattern)
	{
	case QuestPattern::A:

		bet_rate = 1.2f;
		bet_rate_max = 1.5f;

		// 小クモ
		for (int i = 0; i < 3; ++i)
		{
			Enemy* small_spider = CreateEnemy<SmallSpider>();
			small_spider->Initialize();
			enemyManager.Register(small_spider);

			ColliderManager::Instance().SetCollider(small_spider);
		}

		// Aクモ
		{
			Enemy* spider_a = CreateEnemy<Spider_A>();
			spider_a->Initialize();
			enemyManager.Register(spider_a);

			ColliderManager::Instance().SetCollider(spider_a);
		}

		break;
	case QuestPattern::B:

		bet_rate = 1.2f;
		bet_rate_max = 1.5f;

		// 小クモ
		for (int i = 0; i < 3; ++i)
		{
			Enemy* small_spider = CreateEnemy<SmallSpider>();
			small_spider->Initialize();
			enemyManager.Register(small_spider);

			ColliderManager::Instance().SetCollider(small_spider);
		}

		// Bクモ
		{
			Enemy* spider_b = CreateEnemy<Spider_B>();
			spider_b->Initialize();
			enemyManager.Register(spider_b);

			ColliderManager::Instance().SetCollider(spider_b);
		}	

		break;

	case QuestPattern::C:

		bet_rate = 1.2f;
		bet_rate_max = 1.5f;

		// 小クモ
		for (int i = 0; i < 3; ++i)
		{
			Enemy* small_spider = CreateEnemy<SmallSpider>();
			small_spider->Initialize();
			enemyManager.Register(small_spider);

			ColliderManager::Instance().SetCollider(small_spider);
		}

		// Cクモ
		{
			Enemy* spider_c = CreateEnemy<Spider_C>();
			spider_c->Initialize();
			enemyManager.Register(spider_c);

			ColliderManager::Instance().SetCollider(spider_c);
		}

		break;

	case QuestPattern::D:

		bet_rate = 1.2f;
		bet_rate_max = 1.5f;

		// 小クモ
		for (int i = 0; i < 1; ++i)
		{
			Enemy* small_spider = CreateEnemy<SmallSpider>();
			small_spider->Initialize();
			enemyManager.Register(small_spider);

			ColliderManager::Instance().SetCollider(small_spider);
		}

		// Dクモ
		{
			Enemy* spider_d = CreateEnemy<Spider_D>();
			spider_d->Initialize();
			enemyManager.Register(spider_d);

			ColliderManager::Instance().SetCollider(spider_d);
		}


		break;
	case QuestPattern::E:

		bet_rate = 1.2f;
		bet_rate_max = 1.5f;

		// 小クモ
		for (int i = 0; i < 1; ++i)
		{
			Enemy* small_spider = CreateEnemy<SmallSpider>();
			small_spider->Initialize();
			enemyManager.Register(small_spider);

			ColliderManager::Instance().SetCollider(small_spider);
		}

		// Eクモ
		{
			Enemy* spider_e = CreateEnemy<Spider_E>();
			spider_e->Initialize();
			enemyManager.Register(spider_e);

			ColliderManager::Instance().SetCollider(spider_e);
		}


		break;
	case QuestPattern::F:

		bet_rate = 1.5f;
		bet_rate_max = 2.5f;

		// 小クモ
		for (int i = 0; i < 4; ++i)
		{
			Enemy* small_spider = CreateEnemy<SmallSpider>();
			small_spider->Initialize();
			enemyManager.Register(small_spider);

			ColliderManager::Instance().SetCollider(small_spider);
		}

		// Dクモ
		{
			Enemy* spider_d = CreateEnemy<Spider_D>();
			spider_d->Initialize();
			enemyManager.Register(spider_d);

			ColliderManager::Instance().SetCollider(spider_d);
		}


		break;
	case QuestPattern::G:

		bet_rate = 1.5f;
		bet_rate_max = 2.5f;

		// 小クモ
		for (int i = 0; i < 3; ++i)
		{
			Enemy* small_spider = CreateEnemy<SmallSpider>();
			small_spider->Initialize();
			enemyManager.Register(small_spider);

			ColliderManager::Instance().SetCollider(small_spider);
		}

		// Eクモ
		{
			Enemy* spider_e = CreateEnemy<Spider_E>();
			spider_e->Initialize();
			enemyManager.Register(spider_e);

			ColliderManager::Instance().SetCollider(spider_e);
		}


		break;
	case QuestPattern::H:

		bet_rate = 1.5f;
		bet_rate_max = 2.5f;

		// 小クモ
		for (int i = 0; i < 3; ++i)
		{
			Enemy* small_spider = CreateEnemy<SmallSpider>();
			small_spider->Initialize();
			enemyManager.Register(small_spider);

			ColliderManager::Instance().SetCollider(small_spider);
		}

		// Fクモ
		{
			Enemy* spider_f = CreateEnemy<Spider_F>();
			spider_f->Initialize();
			enemyManager.Register(spider_f);

			ColliderManager::Instance().SetCollider(spider_f);
		}

		break;

	case QuestPattern::I:

		bet_rate = 1.5f;
		bet_rate_max = 2.5f;

		// 小クモ
		for (int i = 0; i < 5; ++i)
		{
			Enemy* small_spider = CreateEnemy<SmallSpider>();
			small_spider->Initialize();
			enemyManager.Register(small_spider);

			ColliderManager::Instance().SetCollider(small_spider);
		}

		// Gクモ
		{
			Enemy* spider_g = CreateEnemy<Spider_G>();
			spider_g->Initialize();
			enemyManager.Register(spider_g);

			ColliderManager::Instance().SetCollider(spider_g);
		}

		break;
	case QuestPattern::J:

		bet_rate = 1.5f;
		bet_rate_max = 2.5f;

		// Aクモ
		for (int i = 0; i < 1; ++i)
		{
			Enemy* spider_a = CreateEnemy<Spider_A>();
			spider_a->Initialize();
			enemyManager.Register(spider_a);

			ColliderManager::Instance().SetCollider(spider_a);
		}

		// Bクモ
		{
			Enemy* spider_b = CreateEnemy<Spider_B>();
			spider_b->Initialize();
			enemyManager.Register(spider_b);

			ColliderManager::Instance().SetCollider(spider_b);
		}

		break;

	case QuestPattern::K:

		bet_rate = 2.0f;
		bet_rate_max = 3.5f;

		// Dクモ
		for (int i = 0; i < 1; ++i)
		{
			Enemy* spider_d = CreateEnemy<Spider_D>();
			spider_d->Initialize();
			enemyManager.Register(spider_d);

			ColliderManager::Instance().SetCollider(spider_d);
		}

		// Eクモ
		{
			Enemy* spider_e = CreateEnemy<Spider_E>();
			spider_e->Initialize();
			enemyManager.Register(spider_e);

			ColliderManager::Instance().SetCollider(spider_e);
		}

		break;
	case QuestPattern::L:

		bet_rate = 2.0f;
		bet_rate_max = 3.5f;

		// Cクモ
		for (int i = 0; i < 1; ++i)
		{
			Enemy* spider_c = CreateEnemy<Spider_C>();
			spider_c->Initialize();
			enemyManager.Register(spider_c);

			ColliderManager::Instance().SetCollider(spider_c);
		}

		// Dクモ
		{
			Enemy* spider_d = CreateEnemy<Spider_D>();
			spider_d->Initialize();
			enemyManager.Register(spider_d);

			ColliderManager::Instance().SetCollider(spider_d);
		}

		break;
	case QuestPattern::M:

		bet_rate = 2.0f;
		bet_rate_max = 3.5f;

		// Aクモ
		for (int i = 0; i < 2; ++i)
		{
			Enemy* spider_a = CreateEnemy<Spider_A>();
			spider_a->Initialize();
			enemyManager.Register(spider_a);

			ColliderManager::Instance().SetCollider(spider_a);
		}

		// Gクモ
		{
			Enemy* spider_g = CreateEnemy<Spider_G>();
			spider_g->Initialize();
			enemyManager.Register(spider_g);

			ColliderManager::Instance().SetCollider(spider_g);
		}

		break;
	case QuestPattern::N:

		bet_rate = 2.0f;
		bet_rate_max = 3.5f;

		// Dクモ
		for (int i = 0; i < 1; ++i)
		{
			Enemy* spider_d = CreateEnemy<Spider_D>();
			spider_d->Initialize();
			enemyManager.Register(spider_d);

			ColliderManager::Instance().SetCollider(spider_d);
		}

		// Fクモ
		{
			Enemy* spider_f = CreateEnemy<Spider_F>();
			spider_f->Initialize();
			enemyManager.Register(spider_f);

			ColliderManager::Instance().SetCollider(spider_f);
		}

		break;

	case QuestPattern::O:

		bet_rate = 2.0f;
		bet_rate_max = 3.5f;

		// Gクモ
		for (int i = 0; i < 1; ++i)
		{
			Enemy* spider_g = CreateEnemy<Spider_G>();
			spider_g->Initialize();
			enemyManager.Register(spider_g);

			ColliderManager::Instance().SetCollider(spider_g);
		}

		// Eクモ
		{
			Enemy* spider_e = CreateEnemy<Spider_E>();
			spider_e->Initialize();
			enemyManager.Register(spider_e);

			ColliderManager::Instance().SetCollider(spider_e);
		}

		break;

	case QuestPattern::P:

		bet_rate = 5.0f;
		bet_rate_max = 10.0f;

		// Hクモ
		for (int i = 0; i < 1; ++i)
		{
			Enemy* spider_h = CreateEnemy<Spider_H>();
			spider_h->Initialize();
			enemyManager.Register(spider_h);

			ColliderManager::Instance().SetCollider(spider_h);
		}

		break;

	case QuestPattern::BOSS:

		for (int i = 0; i < 1; ++i)
		{
			boss_enemy = CreateEnemy<BossSpider>();
			boss_enemy->Initialize();
			enemyManager.Register(boss_enemy);

			ColliderManager::Instance().SetCollider(boss_enemy);
		}
		break;
		
	}
}

void GameScene::QuestClear()
{
	int enemy_count = EnemyManager::Instance().GetEnemyCount();

	if (enemy_count <= 0)
	{
		player->SkillFin();
		wave_count++;
		Lemur::Scene::SceneManager::Instance().ChangeScene(new ResultScene(true));
	}
	//player->SkillFin();
	//wave_count++;
	//Lemur::Scene::SceneManager::Instance().ChangeScene(new ResultScene(true));


}

void GameScene::QuestFailed()
{
	if (player->death_anim_end)
	{

		// スキルの影響を受けない状態での最大HPに設定。
		player->SkillFin();
	    player->health = player->max_health;

		if (player->down_count < 0)
		{
			Lemur::Scene::SceneManager::Instance().ChangeScene(new TitleScene);
		}
		else
		{
			player->animStop = true;
			player->death_anim_end = false;
			player->death = false;
			player->GetStateMachine()->SetNextState(player->Idle_State);
			Lemur::Scene::SceneManager::Instance().ChangeScene(new ResultScene(false));
		}
	}
}

void GameScene::Pause()
{
	GamePad& game_pad = Input::Instance().GetGamePad();
	Mouse& mouse = Input::Instance().GetMouse();
	if (game_pad.GetButtonDown() & GamePad::BTN_START)
		is_pause = !is_pause;

	// 一応ポーズ中以外は処理しないようにする
	if (!is_pause) return;
	if(mouse.IsArea(pause_return_UI_pos.x,pause_return_UI_pos.y,pause_UI_size.x,pause_UI_size.y))
	{
		is_pause_Up_selected = true;
		if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
		{
			is_pause = false;
		}
	}
	if (mouse.IsArea(pause_title_UI_pos.x, pause_title_UI_pos.y, pause_UI_size.x, pause_UI_size.y))
	{
		is_pause_Up_selected = false;
		if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
		{
			Lemur::Scene::SceneManager::Instance().ChangeScene(new TitleScene);
		}
	}
}

void GameScene::UIRender()
{
	ID3D11DeviceContext* dc = Lemur::Graphics::Graphics::Instance().GetDeviceContext();

	/*------------- プレイヤーのHPゲージ ------------*/
	player_hp_gauge_zabuton->render(dc, 0, 0, 619, 124);
	rate = static_cast<float>(player->health) / static_cast<float>(player->max_health);
	player_hp_gauge->render(dc, 0, 0, 619.0f * rate, 124, 1, 1, 1, 1,0 ,
		0, 0, 619.0f * rate, 124);

	/*--------------- 敵ボスのHPゲージ --------------*/
	if (boss_enemy)
	{
		enemy_hp_gauge_zabuton->render(dc, ene_HP_gauge_pos.x, ene_HP_gauge_pos.y, 768, 122);
		rate = static_cast<float>(boss_enemy->health) / static_cast<float>(boss_enemy->max_health);
		enemy_hp_gauge->render(dc, ene_HP_gauge_pos.x-10.0f, ene_HP_gauge_pos.y, 768.0f * rate, 122, 1, 1, 1, 1, 0,
			0, 0, 768.0f * rate, 122);
	}

	/*-------------------- スキル -------------------*/
	player->UIRender();


	/*------------------- 操作方法 ------------------*/
	Method_B_Button->render(dc, UI_pos[0].x, UI_pos[0].y, 212, 237);
	Method_A_Button->render(dc, UI_pos[1].x, UI_pos[1].y, 212, 237);
	Method_Y_Button->render(dc, UI_pos[2].x, UI_pos[2].y, 212, 237);
	Method_LBRB_Button->render(dc, UI_pos[3].x, UI_pos[3].y, 212, 237);
}

void GameScene::PauseRender()
{
	// todo 何で真っ黒？
	if (!is_pause)return;

	ID3D11DeviceContext* dc = Lemur::Graphics::Graphics::Instance().GetDeviceContext();
	// todo 半透明化
	pause_back->render(dc, 0,0, 1920, 1080,1,1,1,1.0f,0);
	pause_title->render(dc, pause_title_UI_pos.x, pause_title_UI_pos.y, pause_UI_size.x, pause_UI_size.y);
	pause_return->render(dc, pause_return_UI_pos.x, pause_return_UI_pos.y, pause_UI_size.x, pause_UI_size.y);

	if (is_pause_Up_selected)
		pause_select->animation(dc, pause_select_UI_pos[0],DirectX::XMFLOAT2(25,25),DirectX::XMFLOAT4(1,1,1,1),DirectX::XMConvertToRadians(180), DirectX::XMFLOAT2(25,25));
	else
		pause_select->animation(dc, pause_select_UI_pos[1], DirectX::XMFLOAT2(25, 25), DirectX::XMFLOAT4(1, 1, 1, 1), DirectX::XMConvertToRadians(180), DirectX::XMFLOAT2(25, 25));


}

bool GameScene::EasingTutorial(int start_pos, int end_pos, float stop_time_ms, float easing_time_ms)
{
	/*----------- 最初の待機の部分 ----------*/

    // ストップする時間立ってなかったら
	if (stop_timer_ms <= stop_time_ms) {
		// 時間進める
		stop_timer_ms += high_resolution_timer::Instance().time_interval();
		return false;
	}

	/*------------ イージングでスライドする部分 -----------*/
	spr_tutorial_pos.x = Easing::OutSine(easing_timer_ms, easing_time_ms, static_cast<float>(end_pos), static_cast<float>(start_pos));

	easing_timer_ms += high_resolution_timer::Instance().time_interval();

	// イージング到達したら
	if (easing_timer_ms >= easing_time_ms)
	{
		// イージングのタイムを止める。
		easing_timer_ms = easing_time_ms;

		// 一回の処理終了
		return true;
	}
	else return false;

}
