#include "SceneGame.h"
#include "SceneTitle.h"


#include <random>
#include "Lemur/Input/Mouse.h"
#include"./Lemur/Graphics/Camera.h"
#include"./Lemur/Resource/ResourceManager.h"
#include"./Lemur/Scene/SceneManager.h"

#include"./Lemur/Effekseer/EffekseerManager.h"
#include "GamePro_ProjectileStraight.h"

#include "interval.h"

DirectX::XMFLOAT3 GiftAngle = { 0,0,0 };
DirectX::XMFLOAT4 GiftPosition = { 0,0,0,0 };
int SceneGame::Timer = 0;

using namespace DirectX;
DirectX::XMFLOAT4 convert_screen_to_world(LONG x/*screen*/, LONG y/*screen*/, float z/*ndc*/, D3D11_VIEWPORT vp, const DirectX::XMFLOAT4X4& view_projection)
{
	using namespace DirectX;
	XMFLOAT4 p;
	DirectX::XMStoreFloat4(&p,
		XMVector3TransformCoord(
			XMVector3TransformCoord(
				XMVectorSet(static_cast<float>(x), static_cast<float>(y), z, 1),
				XMMatrixInverse(NULL,
					XMMatrixSet(
						vp.Width * 0.5f, 0.0f, 0.0f, 0.0f,
						0.0f, -vp.Height * 0.5f, 0.0f, 0.0f,
						0.0f, 0.0f, vp.MaxDepth - vp.MinDepth, 0.0f,
						vp.TopLeftX + vp.Width * 0.5f, vp.Height * 0.5f + vp.TopLeftY, vp.MinDepth, 1.0f))
			), XMMatrixInverse(NULL, XMLoadFloat4x4(&view_projection))
		)
	);
	return p;
}

void SceneGame::Initialize()
{
	Lemur::Graphics::Graphics& graphics = Lemur::Graphics::Graphics::Instance();
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
			hr = graphics.GetDevice()->CreateBuffer(&buffer_desc, nullptr, constant_buffers[0].GetAddressOf());
			_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
		}
		{
			buffer_desc.ByteWidth = sizeof(dissolve_constants);
			hr = graphics.GetDevice()->CreateBuffer(&buffer_desc, nullptr, dissolve_constant_buffer.GetAddressOf());
			_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
		}
	}

	//値の初期化
	ShopItemsNum[10] = {};
	SaveShopUi = {};
	shop_int = 0;
	UiCount = {};
	jank = 0;
	isPaused = false;
	speed = 0.05f;
	damage = 1.0f;
	attack = 1.0f;
	HP = 1.0f;
	Player_HP = 3.0f;
	Player_MAXHP = 3.0f;


	SetPhase = true;

	speed_lv = 1;
	damage_lv = 1;
	attack_lv = 1;
	HP_lv = 1;
	Player_MAXHP_Lv = 1;
	speed_MAXlv = 10;
	damage_MAXlv = 10;
	attack_MAXlv = 10;
	HP_MAXlv = 10;
	Player_MAXHP_MAXLv = 10;


	player_dead = false;

	// Stage
	stage = CreateStage();
	stage->Initialize();
	// Player
	player = CreatePlayer();
	player->Initialize();


	//pauseバック
	addUi(3);
	//shadow
	addUi(1);
	//HP
	addUi(4);
	//shop
	addUi(5);
	//ジャンク　お金
	addUi(6);

	
	//shop
	addUi(2);
	addUi(2);
	addUi(2);
	addUi(7);
	addUi(8);
	addUi(9);
	addUi(10);





	UiCount = {};

	framebuffers[0] = std::make_unique<framebuffer>(graphics.GetDevice(), 1280, 720);
	bit_block_transfer = std::make_unique<fullscreen_quad>(graphics.GetDevice());

	// ZELDA
	create_ps_from_cso(graphics.GetDevice(), "./Shader/zelda_ps.cso", zelda_ps.GetAddressOf());
	create_ps_from_cso(graphics.GetDevice(), "./Shader/stage_ps.cso", stage_ps.GetAddressOf());
	create_ps_from_cso(graphics.GetDevice(), "./Shader/character_ps.cso", chara_ps.GetAddressOf());

	player->pixelShader = chara_ps.Get();
	stage->pixelShader = chara_ps.Get();
	for (auto& it : enemyList)
	{
		it->pixelShader= chara_ps.Get();
	}
	for (auto& it : projectileList)
	{
		it->pixelShader = chara_ps.Get();
	}

	// SHADOW
	//skinned_meshes[1] = std::make_unique<skinned_mesh>(graphics.GetDevice(), ".\\resources\\grid.fbx");
	double_speed_z = std::make_unique<shadow_map>(graphics.GetDevice(), shadowmap_width, shadowmap_height);

	hr = XAudio2Create(xaudio2.GetAddressOf(), 0, XAUDIO2_DEFAULT_PROCESSOR);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	hr = xaudio2->CreateMasteringVoice(&master_voice);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


	shot = std::make_unique<Lemur::Audio::audio>(xaudio2.Get(), L".\\resources\\Audio\\SE\\shot.wav");
	purchase = std::make_unique<Lemur::Audio::audio>(xaudio2.Get(), L".\\resources\\Audio\\SE\\purchase.wav");
	explosion = std::make_unique<Lemur::Audio::audio>(xaudio2.Get(), L".\\resources\\Audio\\SE\\explosion.wav");
	damageSE = std::make_unique<Lemur::Audio::audio>(xaudio2.Get(), L".\\resources\\Audio\\SE\\damage.wav");
	BGM = std::make_unique<Lemur::Audio::audio>(xaudio2.Get(), L".\\resources\\Audio\\BGM\\Play.wav");
	BGM2 = std::make_unique<Lemur::Audio::audio>(xaudio2.Get(), L".\\resources\\Audio\\BGM\\play2.wav");
	BGM3 = std::make_unique<Lemur::Audio::audio>(xaudio2.Get(), L".\\resources\\Audio\\BGM\\play3.wav"); 
	result = std::make_unique<Lemur::Audio::audio>(xaudio2.Get(), L".\\resources\\Audio\\BGM\\Result.wav");

	explosionEffect = new Effect("resources/Effects/explosion.efk");
	barelEffect = new Effect("resources/Effects/barel_stylize.efk");

	// BLOOM
	bloomer = std::make_unique<bloom>(graphics.GetDevice(), 1280, 720);
	create_ps_from_cso(graphics.GetDevice(), "./Shader/final_pass_ps.cso", pixel_shaders[0].ReleaseAndGetAddressOf());

#if 0

	// SKYMAP
	bit_block_transfer_sky = std::make_unique<fullscreen_quad>(graphics.GetDevice());
	create_ps_from_cso(graphics.GetDevice(), "./Shader/skymap_ps.cso", pixel_shaders[1].GetAddressOf());
	load_texture_from_file(graphics.GetDevice(), L".\\resources\\winter_evening_4k.hdr", skymap.GetAddressOf(), graphics.GetTexture2D());

	// MASK
	dummy_sprite = std::make_unique<sprite>(graphics.GetDevice(), L".\\resources\\MASK\\chip_win.png");
	load_texture_from_file(graphics.GetDevice(), L".\\resources\\MASK\\dissolve_animation.png",
		mask_texture.GetAddressOf(), &mask_texture2dDesc);

	// sprite用デフォルト描画シェーダー（ディゾルブ）
	{
		D3D11_INPUT_ELEMENT_DESC input_element_desc[]
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		create_vs_from_cso(graphics.GetDevice(), "./Shader/sprite_dissolve_vs.cso", sprite_vertex_shader.GetAddressOf(),
			sprite_input_layout.GetAddressOf(), input_element_desc, ARRAYSIZE(input_element_desc));
		create_ps_from_cso(graphics.GetDevice(), "./Shader/sprite_dissolve_ps.cso", sprite_pixel_shader.GetAddressOf());
	}
#endif
}

void SceneGame::Finalize()
{
	player->Delete();
	stage->Delete();

	//ui->Delete();

	//enemy->Delete();
	for (auto& it : enemyList)
	{
		it->Delete();
	}
	for (auto& it : UiList)
	{
		it->Delete();
	}
	for (auto& it : projectileList)
	{
		it->Delete();
	}
	enemyList.clear();
	UiList.clear();
	projectileList.clear();

	delete player;
	delete stage;
	//delete enemy;
}

void SceneGame::Update(HWND hwnd, float elapsedTime)
{
	BGM->volume(0.25);
	BGM2->volume(0.25);
	BGM3->volume(0.25);

	if (Player_HP > 0)
	{
		if (WaveNumber < 12)
		{
			BGM->play(10);
		}
		if (WaveNumber < 22 && WaveNumber > 11)
		{
			BGM2->play(10);
		}
		if (WaveNumber < 32 && WaveNumber > 21)
		{
			BGM3->play(10);
		}
	}

	
	
	interval<1000>::run([&] {
		Timer++;
		});

	UiGetUpdate();
	EnemyGetUpdate();

	if (Player_HP <= 0)
	{
		player_dead = true;
	}

	Wave();
	Mouse& mouse = Input::Instance().GetMouse();

	for (auto& it : UiList)
	{
		it->Update(elapsedTime);

	}
	for (auto& it : UiList)
	{
		it->player_ = player;
		if (!player_dead)
		{
			it->NumDelivery[5] = shop_int;
		}
		switch (it->NumDelivery[0])
		{
		case 2:
			it->NumDelivery[7] = jank;
			switch (it->NumDelivery[2])
			{
			case 0:
				//攻撃速度
				it->NumDelivery[3] = attack_lv;
				it->NumDelivery[4] = attack_MAXlv;
				break;
			case 1:
				//弾速度
				it->NumDelivery[3] = speed_lv;
				it->NumDelivery[4] = speed_MAXlv;
				break;
			case 2:
				//貫通力
				it->NumDelivery[3] = HP_lv;
				it->NumDelivery[4] = HP_MAXlv;
				break;
			case 3:
				//攻撃力
				it->NumDelivery[3] = damage_lv;
				it->NumDelivery[4] = damage_MAXlv;
				break;
			case 4:
				//プレイヤー
				it->NumDelivery[3] = Player_MAXHP_Lv;
				it->NumDelivery[4] = Player_MAXHP_MAXLv;
				break;
			}

			break;
		case 4:
			it->NumDelivery[2] = Player_MAXHP;
			it->NumDelivery[1] = Player_HP;
			break;

		case 6:
			it->NumDelivery[1] = jank;
			break;

		case 7:
			it->NumDelivery[3] = WaveNumber - 1;
			break;
		case 8:
			if(it->NumDelivery[9] >= 1)
			{
				Lemur::Scene::SceneManager::Instance().ChangeScene(new SceneTitle);
			}
			if(player_dead)
			{
				it->NumDelivery[3] = 1;
				isPaused = true;
			}
			else
			{
				it->NumDelivery[3] = 0;
				it->NumDelivery[4] = WaveNumber-1;
				it->NumDelivery[6] = jank;
				
			}
			break;
		case 9:
			it->itemLevelsave[0] = attack_lv;

			it->itemLevelsave[1] = speed_lv;

			it->itemLevelsave[2] = HP_lv;

			it->itemLevelsave[3] = damage_lv;

			it->itemLevelsave[4] = Player_MAXHP_Lv;
			break;
		case 10:
			if (165 <= attack * 10 + attacktimer && it->NumDelivery[3] < 1&& mouse.GetButtonDown() == mouse.BTN_LEFT)
			{
				it->NumDelivery[3]++;
			}
		}
	}

	//if (mouse.GetButtonDown() == mouse.BTN_MIDDLE)
	//{
	//	// HACK これで現在の敵を全て削除
	//	enemyList.clear();
	//}
	//if (mouse.GetButtonDown() == mouse.BTN_LEFT)
	//{
	//	if (!isPaused)
	//	{
	//		shop_int = 1;
	//		isPaused = true;
	//	}
	//	//else
	//	//{
	//	//	shop_int = 0;
	//	//	isPaused = false;
	//	//}
	//}
	//if (isPaused && mouse.GetButtonDown() == mouse.BTN_RIGHT)isPaused = false;

	if (isPaused)return;

	// 敵が死んだときにタイマー初期化、敵をセットするフラグをON
	if (enemyList.size() == 0)
	{
		
		Timer = 0;
		SetPhase = true;
	}

	for (auto& it : enemyList)
	{
		it->player_ = player;
	}
	for (auto& it : projectileList)
	{
		it->player_ = player;
		it->enemyList_ = enemyList;
		
	}
	//enemy->player_ = player;
	player->enemy_ = enemy;
	player->projectile_ = projectile;

	Camera& camera = Camera::Instance();
	Lemur::Graphics::Graphics& graphics = Lemur::Graphics::Graphics::Instance();

	ID3D11DeviceContext* immediate_context = graphics.GetDeviceContext();

	// エフェクト更新処理
	EffectManager::Instance().Update(elapsedTime);

	camera.Update(elapsedTime);

	stage->Update(elapsedTime);

	player->Update(elapsedTime);

	//enemy->Update(elapsedTime);


	//ui->Update(elapsedTime);


	if (enemyList.size() != 0)
	{
		for (auto& it : enemyList)
		{
			it->Update(elapsedTime);
		}
	}
	if (projectileList.size() != 0)
	{
		for (auto& it : projectileList)
		{
			it->Update(elapsedTime);
		}
	}

	// 空ノードの削除
	auto it = enemyList.begin();
	while (it != enemyList.end())
	{
		if ((*it)->Death)
		{
			(*it)->Delete();
			it = enemyList.erase(it);
		}
		else
		{
			it++;
		}
	}
	auto Proj = projectileList.begin();
	while (Proj != projectileList.end())
	{
		if ((*Proj)->Death)
		{
			(*Proj)->Delete();
			Proj = projectileList.erase(Proj);
		}
		else
		{
			Proj++;
		}
	}
	auto Uiit = UiList.begin();
	while (Uiit != UiList.end())
	{
		if ((*Uiit)->Death)
		{
			(*Uiit)->Delete();
			Uiit = UiList.erase(Uiit);
		}
		else
		{
			Uiit++;
		}
	}

	ProjectileVSEnemy();

	// エネミー同士の当たり判定
	if(enemyList.size()>=2)
	{
		// 全ての敵と総当たりで衝突判定
		int enemyCount = enemyList.size();
		for (int i = 0; i < enemyCount; ++i)
		{
			GameObject* enemyA = enemyList.at(i);
			for (int j = i + 1; j < enemyCount; ++j)
			{
				GameObject* enemyB = enemyList.at(j);
				// 衝突判定
				DirectX::XMFLOAT3 outPosition;
				if (Collision::IntersectCylinderVsCylinder
				(enemyA->position,
					enemyA->radius,
					enemyA->height,
					enemyB->position,
					enemyB->radius,
					enemyB->height,
					outPosition)
					)
				{
					//if(enemyB->StartTime <= Timer)
					enemyB->position.x+=1;
				}
			}
		}
	}
	/////////////////////////////////////////////


	/////攻撃速度

	attacktimer++;


	if (mouse.GetButton() == mouse.BTN_LEFT)
	{

#if 0	
		scene_constants scene_data{};
		POINT p;
		GetCursorPos(&p);
		ScreenToClient(hwnd, &p);

		D3D11_VIEWPORT viewports[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
		UINT viewport_count = { D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE };
		immediate_context->RSGetViewports(&viewport_count, viewports);
		XMFLOAT4X4 view_projection;
		XMStoreFloat4x4(&view_projection, Camera::Instance().GetViewMatrix()* Camera::Instance().GetProjectionMatrix());
		XMFLOAT4 position_on_near_plane = convert_screen_to_world(p.x, p.y, 0.0f, viewports[0], view_projection);
		DirectX::XMVECTOR WorldPosition0 = XMLoadFloat4(&position_on_near_plane);
#else
		DirectX::XMFLOAT3 screenPosition;
		
		screenPosition.x = static_cast<float>(mouse.GetOldPositionX()); //1160
		screenPosition.y = static_cast<float>(mouse.GetOldPositionY()); //660
		screenPosition.z = 0.0f;

		

		D3D11_VIEWPORT viewport;
		UINT numViewports = 1;
		immediate_context->RSGetViewports(&numViewports, &viewport);

		DirectX::XMMATRIX View = Camera::Instance().GetViewMatrix();
		DirectX::XMMATRIX Projection = Camera::Instance().GetProjectionMatrix();

		DirectX::XMVECTOR WorldPosition0 = DirectX::XMVector3Unproject(
			DirectX::XMLoadFloat3(&screenPosition),
			viewport.TopLeftX, viewport.TopLeftY,
			viewport.Width, viewport.Height,
			viewport.MinDepth, viewport.MaxDepth,
			Projection, View, DirectX::XMMatrixIdentity()
		);
#endif

	

		DirectX::XMVECTOR L0 = Camera::Instance().GetEye();
		DirectX::XMFLOAT4 l0;
		DirectX::XMStoreFloat4(&l0, L0);
		DirectX::XMFLOAT4 l;
		DirectX::XMStoreFloat4(&l, DirectX::XMVector3Normalize(WorldPosition0 - L0));

		std::string intersected_mesh;
		std::string intersected_material;
		DirectX::XMFLOAT3 intersected_normal;
		if (stage->stageModel->raycast(l0, l, stage->transform, intersection_point, intersected_normal, intersected_mesh, intersected_material))
		{
			OutputDebugStringA("Intersected : ");
			OutputDebugStringA(intersected_mesh.c_str());
			OutputDebugStringA(" : ");
			OutputDebugStringA(intersected_material.c_str());
			OutputDebugStringA("\n");

			///自機の回転
			//B-Aのベクトル
			DirectX::XMFLOAT3 rotationangle = { intersection_point.x - player->position.x,intersection_point.y - player->position.y,intersection_point.z - player->position.z };

			GiftPosition = intersection_point;
			GiftAngle = rotationangle;
			//正規化
			tani = DirectX::XMVector3Normalize(XMLoadFloat3(&rotationangle));
			
			DirectX::XMStoreFloat3(&TANI, tani);

			TANI_Effect = { TANI.x * 1.0f, 0.0f, TANI.z * 1.0f };

			player->rotation.y = atan2(rotationangle.x,rotationangle.z);	
		}
		else
		{
			OutputDebugStringA("Unintersected...\n");
		}

		if (attack * 15 + attacktimer >= 165 && (screenPosition.x <= 1160 || screenPosition.y <= 660))

		{
			shot->stop();
			addProjectile();
			barelEffect->Play(TANI_Effect);
			shot->play();

			attacktimer = 0;


		}
	}


	//ImGui::Begin("ImGUI");
	//ImGui::DragFloat("light_direction.x", &light_direction.x);
	//ImGui::DragFloat("light_direction.y", &light_direction.y);
	//ImGui::DragFloat("light_direction.z", &light_direction.z);
	//ImGui::DragInt("Timer", &Timer, -10.0f, +10.0f);
	//ImGui::DragFloat("light_view_distance", &light_view_distance);
	//ImGui::DragFloat("light_view_size", &light_view_size);
	//ImGui::DragFloat("light_view_near_z", &light_view_near_z, 1.0f, light_view_far_z - 1.0f);
	//ImGui::DragFloat("light_view_far_z", &light_view_far_z, light_view_near_z + 1.0f, +100.0f);


	//// BLOOM
	//ImGui::DragFloat("bloom_extraction_threshold", &bloomer->bloom_extraction_threshold);
	//ImGui::DragFloat("bloom_intensity", &bloomer->bloom_intensity);


	//ImGui::End();


}

void SceneGame::Render(float elapsedTime)
{
	Camera& camera = Camera::Instance();
	Lemur::Graphics::Graphics& graphics = Lemur::Graphics::Graphics::Instance();

	//Debug
	graphics.debugRenderer = std::make_unique<DebugRenderer>(graphics.GetDevice());

	ID3D11DeviceContext* immediate_context = graphics.GetDeviceContext();
	ID3D11RenderTargetView* render_target_view = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* depth_stencil_view = graphics.GetDepthStencilView();

	// bloom用
	ID3D11RenderTargetView* null_render_target_views[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT]{};
	immediate_context->OMSetRenderTargets(_countof(null_render_target_views), null_render_target_views, 0);
	ID3D11ShaderResourceView* null_shader_resource_views[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT]{};
	immediate_context->VSSetShaderResources(0, _countof(null_shader_resource_views), null_shader_resource_views);
	immediate_context->PSSetShaderResources(0, _countof(null_shader_resource_views), null_shader_resource_views);

	// レンダーターゲット等の設定とクリア
	FLOAT color[]{ 0.2f, 0.2f, 0.2f, 1.0f };
	// キャンバス全体を指定した色に塗りつぶす
	immediate_context->ClearRenderTargetView(render_target_view, color);
	// キャンバス全体の奥行き情報をリセットする
	immediate_context->ClearDepthStencilView(depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	// これから描くキャンバスを指定する
	immediate_context->OMSetRenderTargets(1, &render_target_view, depth_stencil_view);

	immediate_context->PSSetSamplers(0, 1, sampler_states[static_cast<size_t>(SAMPLER_STATE::POINT)].GetAddressOf());
	immediate_context->PSSetSamplers(1, 1, sampler_states[static_cast<size_t>(SAMPLER_STATE::LINEAR)].GetAddressOf());
	immediate_context->PSSetSamplers(2, 1, sampler_states[static_cast<size_t>(SAMPLER_STATE::ANISOTROPIC)].GetAddressOf());
	immediate_context->PSSetSamplers(3, 1, sampler_states[static_cast<size_t>(SAMPLER_STATE::LINEAR_BORDER_BLACK)].GetAddressOf());
	immediate_context->PSSetSamplers(4, 1, sampler_states[static_cast<size_t>(SAMPLER_STATE::LINEAR_BORDER_WHITE)].GetAddressOf());
	// SHADOW
	immediate_context->PSSetSamplers(5, 1, sampler_states[static_cast<size_t>(SAMPLER_STATE::COMPARISON_LINEAR_BORDER_WHITE)].GetAddressOf());

	// ブレンドステートの設定
	immediate_context->OMSetBlendState(blend_states[static_cast<size_t>(BLEND_STATE::ALPHA)].Get(), nullptr, 0xFFFFFFFF);
	// 深度ステンシルステートの設定
	immediate_context->OMSetDepthStencilState(depth_stencil_states[static_cast<size_t>(DEPTH_STATE::ZT_ON_ZW_ON)].Get(), 0);
	immediate_context->RSSetState(rasterizer_states[static_cast<size_t>(RASTER_STATE::SOLID)].Get());

	camera.SetPerspectiveFov(immediate_context);

	// 定数バッファの更新
	scene_constants data{};
	data.light_direction = light_direction;
	DirectX::XMStoreFloat4(&data.camera_position, camera.GetEye());
	
#if 0

	D3D11_VIEWPORT viewport;
	UINT num_viewports{ 1 };
	immediate_context->RSGetViewports(&num_viewports, &viewport);

	float aspect_ratio{ viewport.Width / viewport.Height };

	// SKYMAP
	DirectX::XMStoreFloat4x4(&data.inv_view_projection, DirectX::XMMatrixInverse(NULL, camera.GetViewMatrix() * camera.GetProjectionMatrix()));

	immediate_context->UpdateSubresource(constant_buffers[0].Get(), 0, 0, &data, 0, 0);
	immediate_context->VSSetConstantBuffers(1, 1, constant_buffers[0].GetAddressOf());
	immediate_context->PSSetConstantBuffers(1, 1, constant_buffers[0].GetAddressOf());
	// SKYMAP
	//immediate_context->OMSetDepthStencilState(depth_stencil_states[static_cast<size_t>(DEPTH_STATE::ZT_OFF_ZW_OFF)].Get(), 0);
	//immediate_context->RSSetState(rasterizer_states[static_cast<size_t>(RASTER_STATE::CULL_NONE)].Get());
	//bit_block_transfer_sky->blit(immediate_context, skymap.GetAddressOf(), 0, 1, pixel_shaders[1].Get());
	//

#endif
	
	// SHADOW : make shadow map
	{
		using namespace DirectX;

		const float aspect_ratio = double_speed_z->viewport.Width / double_speed_z->viewport.Height;
		XMVECTOR F{ XMLoadFloat4(&light_view_focus) };
		XMVECTOR E{ F - XMVector3Normalize(XMLoadFloat4(&light_direction)) * light_view_distance };
		XMVECTOR U{ XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f) };
		XMMATRIX V{ XMMatrixLookAtLH(E, F, U) };
		XMMATRIX P{ XMMatrixOrthographicLH(light_view_size * aspect_ratio, light_view_size, light_view_near_z, light_view_far_z) };

		DirectX::XMStoreFloat4x4(&data.view_projection, V * P);
		data.light_view_projection = data.view_projection;
		immediate_context->UpdateSubresource(constant_buffers[0].Get(), 0, 0, &data, 0, 0);
		immediate_context->VSSetConstantBuffers(1, 1, constant_buffers[0].GetAddressOf());

		double_speed_z->clear(immediate_context, 1.0f);
		double_speed_z->activate(immediate_context);

		for (auto& it : enemyList)
		{
			it->Render(elapsedTime);
		}
		player->Render(elapsedTime);
		double_speed_z->deactivate(immediate_context);
	}
	// Render scene
	D3D11_VIEWPORT viewport;
	UINT num_viewports{ 1 };
	immediate_context->RSGetViewports(&num_viewports, &viewport);
	DirectX::XMStoreFloat4x4(&data.view_projection, camera.GetViewMatrix()* camera.GetProjectionMatrix());

	immediate_context->UpdateSubresource(constant_buffers[0].Get(), 0, 0, &data, 0, 0);
	immediate_context->VSSetConstantBuffers(1, 1, constant_buffers[0].GetAddressOf());
	immediate_context->PSSetConstantBuffers(1, 1, constant_buffers[0].GetAddressOf());

	//3D描画
	{
		framebuffers[0]->clear(immediate_context);
		framebuffers[0]->activate(immediate_context);
		immediate_context->OMSetDepthStencilState(depth_stencil_states[static_cast<size_t>(DEPTH_STATE::ZT_ON_ZW_ON)].Get(), 0);
		immediate_context->RSSetState(rasterizer_states[static_cast<size_t>(RASTER_STATE::SOLID)].Get());
		immediate_context->PSSetShaderResources(8, 1, double_speed_z->shader_resource_view.GetAddressOf());
		player->Render(elapsedTime);

		stage->Render(elapsedTime);


		//enemy->Render(elapsedTime);
		for (auto& it : enemyList)
		{
			it->Render(elapsedTime);
		}

		for (auto& it : projectileList)
		{
			it->Render(elapsedTime);
		}

		framebuffers[0]->deactivate(immediate_context);
		// BLOOM
		bloomer->make(immediate_context, framebuffers[0]->shader_resource_views[0].Get());

		immediate_context->OMSetDepthStencilState(depth_stencil_states[static_cast<size_t>(DEPTH_STATE::ZT_OFF_ZW_OFF)].Get(), 0);
		immediate_context->RSSetState(rasterizer_states[static_cast<size_t>(RASTER_STATE::CULL_NONE)].Get());
		immediate_context->OMSetBlendState(blend_states[static_cast<size_t>(BLEND_STATE::ALPHA)].Get(), nullptr, 0xFFFFFFFF);

		ID3D11ShaderResourceView* shader_resource_views[] =
		{
			framebuffers[0]->shader_resource_views[0].Get(),
			bloomer->shader_resource_view(),
		};
		bit_block_transfer->blit(immediate_context, shader_resource_views, 0, 2, pixel_shaders[0].Get());
	}
	//framebuffers[0]->clear(immediate_context);
	//framebuffers[0]->activate(immediate_context);

	// SHADOW : bind shadow map at slot 8
	//player->Render(elapsedTime);

	//stage->Render(elapsedTime);


	////enemy->Render(elapsedTime);
	//for (auto& it:enemyList)
	//{
	//	it->Render(elapsedTime);
	//}

	//for (auto& it : projectileList)
	//{
	//	it->Render(elapsedTime);
	//}


	// UNIT.32
	//framebuffers[0]->deactivate(immediate_context);
	//
	//// BLOOM
	//bloomer->make(immediate_context, framebuffers[0]->shader_resource_views[0].Get());
	//
	//immediate_context->OMSetDepthStencilState(depth_stencil_states[static_cast<size_t>(DEPTH_STATE::ZT_OFF_ZW_OFF)].Get(), 0);
	//immediate_context->RSSetState(rasterizer_states[static_cast<size_t>(RASTER_STATE::CULL_NONE)].Get());
	//immediate_context->OMSetBlendState(blend_states[static_cast<size_t>(BLEND_STATE::ALPHA)].Get(), nullptr, 0xFFFFFFFF);
	//ID3D11ShaderResourceView* shader_resource_views[] =
	//{
	//	framebuffers[0]->shader_resource_views[0].Get(),
	//	bloomer->shader_resource_view(),
	//};
	//bit_block_transfer->blit(immediate_context, shader_resource_views, 0, 2, pixel_shaders[0].Get());

#if  0


	D3D11_VIEWPORT viewport;
	UINT num_viewports{ 1 };
	immediate_context->RSGetViewports(&num_viewports, &viewport);

	float aspect_ratio{ viewport.Width / viewport.Height };

	// SKYMAP
	DirectX::XMStoreFloat4x4(&data.inv_view_projection, DirectX::XMMatrixInverse(NULL, camera.GetViewMatrix() * camera.GetProjectionMatrix()));

	immediate_context->UpdateSubresource(constant_buffers[0].Get(), 0, 0, &data, 0, 0);
	immediate_context->VSSetConstantBuffers(1, 1, constant_buffers[0].GetAddressOf());
	immediate_context->PSSetConstantBuffers(1, 1, constant_buffers[0].GetAddressOf());

	//3D描画
	{
		framebuffers[0]->clear(immediate_context);
		framebuffers[0]->activate(immediate_context);
		immediate_context->OMSetDepthStencilState(depth_stencil_states[static_cast<size_t>(DEPTH_STATE::ZT_ON_ZW_ON)].Get(), 0);
		immediate_context->RSSetState(rasterizer_states[static_cast<size_t>(RASTER_STATE::SOLID)].Get());
		player->Render(elapsedTime);
		framebuffers[0]->deactivate(immediate_context);
		// BLOOM
		bloomer->make(immediate_context, framebuffers[0]->shader_resource_views[0].Get());

		immediate_context->OMSetDepthStencilState(depth_stencil_states[static_cast<size_t>(DEPTH_STATE::ZT_OFF_ZW_OFF)].Get(), 0);
		immediate_context->RSSetState(rasterizer_states[static_cast<size_t>(RASTER_STATE::CULL_NONE)].Get());
		immediate_context->OMSetBlendState(blend_states[static_cast<size_t>(BLEND_STATE::ALPHA)].Get(), nullptr, 0xFFFFFFFF);

		ID3D11ShaderResourceView* shader_resource_views[] =
		{
			framebuffers[0]->shader_resource_views[0].Get(),
			bloomer->shader_resource_view(),
		};
		bit_block_transfer->blit(immediate_context, shader_resource_views, 0, 2, pixel_shaders[0].Get());
	}
	// SKYMAP
	//immediate_context->OMSetDepthStencilState(depth_stencil_states[static_cast<size_t>(DEPTH_STATE::ZT_OFF_ZW_OFF)].Get(), 0);
	//immediate_context->RSSetState(rasterizer_states[static_cast<size_t>(RASTER_STATE::CULL_NONE)].Get());
	//bit_block_transfer_sky->blit(immediate_context, skymap.GetAddressOf(), 0, 1, pixel_shaders[1].Get());
	//

	// SHADOW : make shadow map
	{
		using namespace DirectX;

		const float aspect_ratio = double_speed_z->viewport.Width / double_speed_z->viewport.Height;
		XMVECTOR F{ XMLoadFloat4(&light_view_focus) };
		XMVECTOR E{ F - XMVector3Normalize(XMLoadFloat4(&light_direction)) * light_view_distance };
		XMVECTOR U{ XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f) };
		XMMATRIX V{ XMMatrixLookAtLH(E, F, U) };
		XMMATRIX P{ XMMatrixOrthographicLH(light_view_size * aspect_ratio, light_view_size, light_view_near_z, light_view_far_z) };

		DirectX::XMStoreFloat4x4(&data.view_projection, V * P);
		data.light_view_projection = data.view_projection;
		immediate_context->UpdateSubresource(constant_buffers[0].Get(), 0, 0, &data, 0, 0);
		immediate_context->VSSetConstantBuffers(1, 1, constant_buffers[0].GetAddressOf());

		double_speed_z->clear(immediate_context, 1.0f);
		double_speed_z->activate(immediate_context);

		ID3D11PixelShader* null_pixel_shader{ NULL };
		// ここにRender
		double_speed_z->deactivate(immediate_context);
	}

	DirectX::XMStoreFloat4x4(&data.view_projection, camera.GetViewMatrix() * camera.GetProjectionMatrix());

	// Render scene
	D3D11_VIEWPORT viewport;
	UINT num_viewports{ 1 };
	immediate_context->RSGetViewports(&num_viewports, &viewport);
	DirectX::XMStoreFloat4x4(&data.view_projection, camera.GetViewMatrix() * camera.GetProjectionMatrix());

	immediate_context->UpdateSubresource(constant_buffers[0].Get(), 0, 0, &data, 0, 0);
	immediate_context->VSSetConstantBuffers(1, 1, constant_buffers[0].GetAddressOf());
	immediate_context->PSSetConstantBuffers(1, 1, constant_buffers[0].GetAddressOf());

	// SHADOW : bind shadow map at slot 8
	immediate_context->PSSetShaderResources(8, 1, double_speed_z->shader_resource_view.GetAddressOf());

	// ここにRender
#endif
	// 3Dエフェクト描画
	{
		DirectX::XMFLOAT4X4 view{};
		DirectX::XMFLOAT4X4 projection{};

		DirectX::XMStoreFloat4x4(&view, camera.GetViewMatrix());
		DirectX::XMStoreFloat4x4(&projection, camera.GetProjectionMatrix());

		EffectManager::Instance().Render(view, projection);

		// デバッグレンダラ描画実行
		graphics.GetDebugRenderer()->Render(graphics.GetDeviceContext(), view, projection);

	}

	// sprite描画
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
		dissolve_constants dissolve{};
		dissolve.parameters.x = dissolve_value;
		immediate_context->UpdateSubresource(dissolve_constant_buffer.Get(), 0, 0, &dissolve, 0, 0);
		immediate_context->VSSetConstantBuffers(3, 1, dissolve_constant_buffer.GetAddressOf());
		immediate_context->PSSetConstantBuffers(3, 1, dissolve_constant_buffer.GetAddressOf());
	}

	for (auto& it : UiList)
	{
		it->Render(elapsedTime);
	}

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
			dissolve_constants dissolve{};
			dissolve.parameters.x = dissolve_value;
			immediate_context->UpdateSubresource(dissolve_constant_buffer.Get(), 0, 0, &dissolve, 0, 0);
			immediate_context->VSSetConstantBuffers(3, 1, dissolve_constant_buffer.GetAddressOf());
			immediate_context->PSSetConstantBuffers(3, 1, dissolve_constant_buffer.GetAddressOf());
		}
	}
}

void SceneGame::addEnemy()
{
	Enemy* e;
	e = CreateEnemy();
	e->Initialize();
	enemyList.push_back(e);
}

void SceneGame::addEnemy(int enemyType, int startTime)
{
	Enemy* e;
	e = CreateEnemy();
	e->EnemyInitialize(enemyType, startTime);
	e->pixelShader = chara_ps.Get();
	enemyList.push_back(e);
}
void SceneGame::addProjectile()
{
	GameObject* p;
	p = CreateProjectile();
	p->NumFloatDelivery[0] = GiftAngle.x;
	p->NumFloatDelivery[1] = GiftAngle.z;
	p->NumFloatDelivery[2] = speed;
	p->NumDelivery[3] = damage;
	p->NumDelivery[4] = HP;
	p->Initialize();
	projectileList.push_back(p);
}

void SceneGame::ProjectileVSEnemy()
{
	//TODO 弾と敵のの当たり判定
	if(enemyList.size()!=0&& projectileList.size() != 0)
	{
		// 総当たりで衝突判定
		int enemyCount = enemyList.size();
		for (int i = 0; i < enemyCount; ++i)
		{
			GameObject* ene = enemyList.at(i);
			if (ene->StartTime <= Timer)
			{
				if (enemyList.at(i)->NumDelivery[int(enemyNum::EnemyDeath_Flag)] != 1 && enemyList.at(i)->NumDelivery[int(enemyNum::EnemyDeath_Call)] != 1)
				{
					int projectileCount = projectileList.size();
					for (int j = 0; j < projectileCount; ++j)
					{
						GameObject* pro = projectileList.at(j);
						// 衝突判定
						DirectX::XMFLOAT3 outPosition;
						if (Collision::IntersectSphereVsCylinder
						(pro->position,
							pro->radius,
							ene->position,
							ene->radius,
							ene->height,
							outPosition)
							)
						{
							if (ene->NumDelivery[9] == 0 && !pro->EnemyHitSave[i])
							{

								ene->NumDelivery[9] = pro->damage;
								pro->HP -= 1;
								pro->EnemyHitSave[i] = true;

								damageSE->stop();
								damageSE->play();

								ene->HitReaction = true;

							}
						}
					}
				}
			}
		}
	}
} 

void SceneGame::addUi(int Uitype)
{
	bool judge = false;
	GameObject* Ui;
	Ui = CreateUi();
	Ui->NumDelivery[0] = Uitype;
	if (Uitype == 4)Ui->NumDelivery[1] = Player_HP;
	if (Uitype == 4)Ui->NumDelivery[2] = Player_MAXHP;
	
	if (Uitype == 2)
	{
		std::mt19937 mt{ std::random_device{}() };
		for (int i = 0; i < 1;)
		{
			std::uniform_int_distribution<int> Type(0, 5);
			ShopItemsNum[SaveShopUi] = int(Type(mt));
			judge = false;
			for (int j = 0; j < SaveShopUi; j++)
			{

				if (ShopItemsNum[SaveShopUi] == ShopItemsNum[j])
				{
					judge = true;
					break;
				}

			}
			if (!judge)
			{
				break;
			}
			continue;
		}
		Ui->NumDelivery[2] = ShopItemsNum[SaveShopUi];
		Ui->NumDelivery[1] = SaveShopUi;
		switch (ShopItemsNum[SaveShopUi])
		{
		case 0:
			//攻撃速度
			if (Uitype == 2)Ui->NumDelivery[3] = attack_lv;
			if (Uitype == 2)Ui->NumDelivery[4] = attack_MAXlv;
			break;
		case 1:
			//弾速度
			if (Uitype == 2)Ui->NumDelivery[3] = speed_lv;
			if (Uitype == 2)Ui->NumDelivery[4] = speed_MAXlv;
			break;
		case 2:
			//貫通力
			if (Uitype == 2)Ui->NumDelivery[3] = HP_lv;
			if (Uitype == 2)Ui->NumDelivery[4] =HP_MAXlv;
			break;
		case 3:
			//攻撃力
			if (Uitype == 2)Ui->NumDelivery[3] = damage_lv;
			if (Uitype == 2)Ui->NumDelivery[4] = damage_MAXlv;
			break;
		case 4:
			//プレイヤー
			if (Uitype == 2)Ui->NumDelivery[3] = Player_MAXHP_Lv;
			if (Uitype == 2)Ui->NumDelivery[4] = Player_MAXHP_MAXLv;
			break;
		case 5:
			//回復
				if (Uitype == 2)Ui->NumDelivery[3] = 1;

				if (Uitype == 2)Ui->NumDelivery[4] = Player_MAXHP_MAXLv;
			break;
		}
	}

	if (Uitype == 7)
	{
		Ui->NumDelivery[3] = WaveNumber;
	}

	Ui->Initialize();
	
	UiList.push_back(Ui);

	if (Uitype == 2)SaveShopUi++;

	UiCount++;
	
}

void SceneGame::UiGetUpdate()
{
	bool judge = false;

	for (auto& it : UiList)
	{
		//Uitype2。能力増加のUIの探索
		if (it->NumDelivery[0] == 2)
		{
			if (it->NumDelivery[6] == 1)
			{
				purchase->play();
				jank -= it->NumDelivery[9];
				switch (it->NumDelivery[2])
				{
				case 0:
					//攻撃速度
					attack += 1;
					it->NumDelivery[6] = 0;
					attack_lv++;

					break;
				case 1:
					//弾速度
					speed += 0.01f;
					it->NumDelivery[6] = 0;
					speed_lv++;

					break;
				case 2:
					//貫通力
					HP += it->NumDelivery[6];
					it->NumDelivery[6] = 0;
					HP_lv++;

					break;
				case 3:
					//攻撃力
					damage += 1;
					damage_lv++;
					it->NumDelivery[6] = 0;

					break;
				case 4:
					//プレイヤー
					Player_HP += 1;
					Player_MAXHP += 1;
					Player_MAXHP_Lv++;
					it->NumDelivery[6] = 0;

					break;
				case 5:
					//プレイヤー回復
					if (Player_HP < Player_MAXHP)
					{
						Player_HP += 1;
					}
					it->NumDelivery[6] = 0;



					break;
				}
				for (auto& it : UiList)
				{
					if (it->NumDelivery[0] == 2)
					{
						std::mt19937 mt{ std::random_device{}() };
						int newshopitems;
						for (int i = 0; i < 1;)
						{
							std::uniform_int_distribution<int> Type(0, 5);
							newshopitems = int(Type(mt));
							judge = false;
							for (int j = 0; j < SaveShopUi; j++)
							{
								if (newshopitems == ShopItemsNum[j] || JudgeState(newshopitems))
								{
									judge = true;
									break;
								}

							}
							if (!judge)
							{
								break;
							}
							continue;
						}
						ShopItemsNum[it->NumDelivery[1]] = newshopitems;
						it->NumDelivery[2] = newshopitems;
						shop_int = 0;
						isPaused = false;
						switch (newshopitems)
						{
						case 0:
							//攻撃速度
							it->NumDelivery[3] = attack_lv;
							it->NumDelivery[4] = attack_MAXlv;
							break;
						case 1:
							//弾速度
							it->NumDelivery[3] = speed_lv;
							it->NumDelivery[4] = speed_MAXlv;
							break;
						case 2:
							//貫通力
							it->NumDelivery[3] = HP_lv;
							it->NumDelivery[4] = HP_MAXlv;
							break;
						case 3:
							//攻撃力
							it->NumDelivery[3] = damage_lv;
							it->NumDelivery[4] = damage_MAXlv;
							break;
						case 4:
							//プレイヤー
							it->NumDelivery[3] = Player_MAXHP_Lv;
							it->NumDelivery[4] = Player_MAXHP_MAXLv;
							break;
						case 5:
							//回復
							it->NumDelivery[3] = 1;
							it->NumDelivery[4] = Player_MAXHP_MAXLv;
							break;
						}




						it->Initialize();
						//for (auto& it : UiList)
						//{
						//	if (it->NumDelivery[0] == 5)
						//	{
						//		it->NumDelivery[6] = 0;
						//	}
						//}
					}
				}
			}

		}

		//UItype5 shop
		if (it->NumDelivery[0] == 5)
		{
			if (it->NumDelivery[6] == 1)
			{
				for (auto& it : UiList)
				{
					if (it->NumDelivery[0] == 10)
					{
					it->NumDelivery[4] = 1;
					}
				}
				shop_int = 1;
				isPaused = true;
			}
			else
			{

				shop_int = 0;
				isPaused = false;
			}
		}
	}
}

void SceneGame::Wave()
{
	//TODO 三澤君
	if (SetPhase)
	{
		switch (WaveNumber)
		{
		case 1:
			addEnemy(0, 2);

			WaveNumber++;
			SetPhase = false;
			break;
		case 2:
			addEnemy(0, 2);
			addEnemy(0, 5);
			addEnemy(0, 8);
			WaveNumber++;
			SetPhase = false;
			break;
		case 3:
			addEnemy(0, 3);
			addEnemy(0, 5);
			addEnemy(0, 7);
			addEnemy(0, 9);

			WaveNumber++;
			SetPhase = false;
			break;
		case 4:
			addEnemy(0, 3);
			addEnemy(0, 4);
			addEnemy(0, 5);
			addEnemy(0, 6);
			addEnemy(0, 7);
			WaveNumber++;
			SetPhase = false;
			break;
		case 5:
			addEnemy(0, 2);
			addEnemy(0, 2);
			addEnemy(0, 4);
			addEnemy(0, 4);
			addEnemy(0, 6);
			addEnemy(0, 6);
			WaveNumber++;
			SetPhase = false;
			break;
		case 6:
			addEnemy(0, 2);
			addEnemy(0, 3);
			addEnemy(0, 4);
			addEnemy(0, 5);
			addEnemy(0, 6);
			addEnemy(1, 7);
			WaveNumber++;
			SetPhase = false;
			break;
		case 7:
			addEnemy(0, 2);
			addEnemy(0, 2);
			addEnemy(0, 3);
			addEnemy(0, 4);
			addEnemy(1, 5);
			addEnemy(1, 6);
			WaveNumber++;
			SetPhase = false;
			break;
		case 8:
			addEnemy(1, 2);
			addEnemy(0, 2);
			addEnemy(1, 3);
			addEnemy(0, 3);
			addEnemy(1, 4);
			addEnemy(0, 4);
			WaveNumber++;
			SetPhase = false;
			break;
		case 9:
			addEnemy(0, 2);
			addEnemy(0, 3);
			addEnemy(0, 4);
			addEnemy(0, 5);
			addEnemy(0, 6);
			addEnemy(0, 7);
			addEnemy(0, 8);
			addEnemy(0, 9);
			addEnemy(0, 10);

			WaveNumber++;
			SetPhase = false;
			break;
		case 10:
			addEnemy(1, 2);
			addEnemy(1, 3);
			addEnemy(1, 4);
			addEnemy(1, 5);
			addEnemy(1, 6);
			WaveNumber++;
			SetPhase = false;
			break;
		case 11:
			addEnemy(0, 2);
			addEnemy(0, 3);
			addEnemy(0, 4);
			addEnemy(1, 5);
			addEnemy(1, 6);
			addEnemy(2, 9);
			WaveNumber++;
			SetPhase = false;
			break;
		case 12:
			addEnemy(0, 2);
			addEnemy(0, 2);
			addEnemy(0, 3);
			addEnemy(0, 3);
			addEnemy(0, 4);
			addEnemy(0, 4);
			addEnemy(0, 5);
			addEnemy(0, 5);
			addEnemy(0, 6);
			addEnemy(0, 6);
			WaveNumber++;
			SetPhase = false;
			break;
		case 13:
			addEnemy(1, 2);
			addEnemy(0, 2);
			addEnemy(1, 3);
			addEnemy(1, 4);
			addEnemy(0, 4);
			addEnemy(1, 5);
			addEnemy(1, 6);
			addEnemy(0, 6);


			WaveNumber++;
			SetPhase = false;
			break;
		case 14:
			addEnemy(1, 2);
			addEnemy(0, 2);
			addEnemy(1, 3);
			addEnemy(1, 4);
			addEnemy(0, 4);
			addEnemy(1, 5);
			addEnemy(1, 6);
			addEnemy(0, 6);



			WaveNumber++;
			SetPhase = false;
			break;
		case 15:
			addEnemy(2, 2);
			addEnemy(0, 4);
			addEnemy(0, 5);
			addEnemy(0, 6);
			addEnemy(0, 7);
			addEnemy(0, 8);
			addEnemy(1, 10);
			addEnemy(1, 10);
			WaveNumber++;
			SetPhase = false;
			break;
		case 16:
			addEnemy(0, 2);
			addEnemy(0, 3);
			addEnemy(0, 4);
			addEnemy(1, 5);
			addEnemy(1, 5);
			addEnemy(3, 10);
			WaveNumber++;
			SetPhase = false;
			break;
		case 17:
			addEnemy(0, 2);
			addEnemy(0, 3);
			addEnemy(0, 4);
			addEnemy(3, 5);
			addEnemy(3, 5);
			addEnemy(3, 5);

			WaveNumber++;
			SetPhase = false;
			break;
		case 18:
			addEnemy(1, 2);
			addEnemy(1, 2);
			addEnemy(1, 2);
			addEnemy(1, 2);
			addEnemy(1, 3);
			addEnemy(1, 3);
			addEnemy(2, 5);

			WaveNumber++;
			SetPhase = false;
			break;
		case 19:
			addEnemy(1, 2);
			addEnemy(1, 2);
			addEnemy(2, 3);
			addEnemy(2, 3);
			addEnemy(0, 4);
			addEnemy(0, 5);
			addEnemy(0, 6);
			addEnemy(3, 7);
			addEnemy(3, 8);
			WaveNumber++;
			SetPhase = false;
			break;
		case 20:
			addEnemy(2, 2);
			addEnemy(2, 3);
			addEnemy(2, 4);
			addEnemy(2, 5);
			addEnemy(2, 6);
			addEnemy(0, 7);
			addEnemy(0, 8);
			addEnemy(1, 9);
			addEnemy(1, 10);
			addEnemy(3, 15);
			addEnemy(3, 16);
			addEnemy(3, 17);
			WaveNumber++;
			SetPhase = false;
			break;
		case 21:

			WaveNumber++;
			SetPhase = false;
			break;
		case 22:

			WaveNumber++;
			SetPhase = false;
			break;
		case 23:

			WaveNumber++;
			SetPhase = false;
			break;
		case 24:

			WaveNumber++;
			SetPhase = false;
			break;
		case 25:

			WaveNumber++;
			SetPhase = false;
			break;
		case 26:

			WaveNumber++;
			SetPhase = false;
			break;
		case 27:

			WaveNumber++;
			SetPhase = false;
			break;
		case 28:

			WaveNumber++;
			SetPhase = false;
			break;
		case 29:

			WaveNumber++;
			SetPhase = false;
			break;
		case 30:

			WaveNumber++;
			SetPhase = false;
			break;
		}
	}
}

void SceneGame::EnemyGetUpdate()
{
	for (auto& it : enemyList)
	{
		if (it->NumDelivery[int(SceneGame::enemyNum::EnemyPlayerDeath_Flag)] >= 1)
		{

			Player_HP -= it->NumDelivery[int(SceneGame::enemyNum::EnemyPlayerDeath_Flag)];
			it->NumDelivery[int(SceneGame::enemyNum::EnemyPlayerDeath_Flag)] = 0;
			it->NumDelivery[int(SceneGame::enemyNum::EnemyPlayerDeath_Call)] = 1;

			Player_HP -= it->NumDelivery[0];
			if (Player_HP >= 1)
			{
				damageSE->stop();
				damageSE->play();
			}
			if (Player_HP <= 0)
			{
				explosionEffect->Play(player->position,2.0f);
				explosion->stop();
				explosion->play();
				BGM->stop();
				BGM2->stop();
				BGM3->stop();
				result->play();
			}
			it->NumDelivery[0] = 0;
			it->NumDelivery[1] = 1;

		}
		if (it->NumDelivery[int(SceneGame::enemyNum::EnemyDeath_Flag)] >= 1)
		{
			jank += it->NumDelivery[int(SceneGame::enemyNum::EnemyMomey)];

			it->NumDelivery[int(SceneGame::enemyNum::EnemyDeath_Flag)] = 0;
			it->NumDelivery[int(SceneGame::enemyNum::EnemyDeath_Call)]++;
		}
	}
}

bool SceneGame::JudgeState(int i)
{

	switch (i)
	{
	case 0:
		//攻撃速度
		if (attack_lv >= attack_MAXlv)
		{
			return true;
		}
		break;
	case 1:
		//弾速度
		if (speed_lv >= speed_MAXlv)
		{
			return true;
		}
		break;
	case 2:
		//貫通力
		if (HP_lv >= HP_MAXlv)
		{
			return true;
		}
		break;
	case 3:
		//攻撃力
		if (damage_lv >= damage_MAXlv)
		{
			return true;
		}
		break;
	case 4:
		//プレイヤー
		if (Player_MAXHP_Lv >= Player_MAXHP_MAXLv)
		{
			return true;
		}


		break;
	}
	return false;
}
