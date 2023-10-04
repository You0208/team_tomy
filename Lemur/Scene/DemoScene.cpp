#include "DemoScene.h"
#include "./Lemur/Component/DemoEnemyManager.h"
#include "../Graphics/Camera.h"
#include "../Resource/ResourceManager.h"

// Effect
#include "../Effekseer/EffekseerManager.h"

void DemoScene::Initialize()
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
		{
			buffer_desc.ByteWidth = sizeof(light_constants);
			hr = graphics.GetDevice()->CreateBuffer(&buffer_desc, nullptr, light_constant_buffer.GetAddressOf());
			_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
		}
		{
			buffer_desc.ByteWidth = sizeof(adjust_constants);
			hr = graphics.GetDevice()->CreateBuffer(&buffer_desc, nullptr, adjust_constant_buffer.GetAddressOf());
			_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
		}
	}

	player = CreatePlayer();
	player->Initialize();

	// エネミー初期化
	DemoEnemyManager& enemyManager = DemoEnemyManager::Instance();
	for (int i = 0; i < 0; ++i)
	{
		DemoEnemy* slime = CreateEnemy();
		slime->Initialize();
		slime->SetPosition({ DirectX::XMFLOAT3(i * 2.0f, 0, 5) });
		enemyManager.Register(slime);
	}

	framebuffers[0] = std::make_unique<framebuffer>(graphics.GetDevice(), 1920, 1080);
	bitBlockTransfer = std::make_unique<fullscreen_quad>(graphics.GetDevice());

	//BLOOM
	bloomer = std::make_unique<bloom>(graphics.GetDevice(), 1920, 1080);
	create_ps_from_cso(graphics.GetDevice(), "./Shader/final_pass_ps.cso", pixel_shaders[2].ReleaseAndGetAddressOf());
	
	// SKYMAP
	bitBlockTransfer_sky = std::make_unique<fullscreen_quad>(graphics.GetDevice());
	create_ps_from_cso(graphics.GetDevice(), "./Shader/skymap_ps.cso", pixel_shaders[1].GetAddressOf());
	load_texture_from_file(graphics.GetDevice(), L".\\resources\\winter_evening_4k.hdr", skymap.GetAddressOf(), graphics.GetTexture2D());

	// TODO
	create_ps_from_cso(graphics.GetDevice(), "./Shader/try_pbr_ps.cso", zelda_ps.GetAddressOf());
	create_ps_from_cso(graphics.GetDevice(), "./Shader/Wall.cso", Wall.GetAddressOf());
	create_ps_from_cso(graphics.GetDevice(), "./Shader/try_ps.cso", Try.GetAddressOf());

	// シェーダーの決定
	player->SetPixelShader(zelda_ps.Get());

	skinned_meshes[1] = std::make_unique<skinned_mesh>(graphics.GetDevice(), ".\\resources\\Model\\grid.fbx");
	cube = std::make_unique<skinned_mesh>(graphics.GetDevice(), ".\\resources\\Model\\latha.fbx");
	
	// SHADOW
	double_speed_z = std::make_unique<shadow_map>(graphics.GetDevice(), shadowmap_width, shadowmap_height);

	// dissolve
	load_texture_from_file(graphics.GetDevice(), L".\\resources\\Image\\dissolve_animation.png", noise.GetAddressOf(), graphics.GetTexture2D());//TODO


	//// MASK
	//dummy_sprite = std::make_unique<sprite>(graphics.GetDevice(), L".\\resources\\MASK\\chip_win.png");
	//load_texture_from_file(graphics.GetDevice(), L".\\resources\\MASK\\dissolve_animation.png",
	//	mask_texture.GetAddressOf(), &mask_texture2dDesc);

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
	Camera& camera = Camera::Instance();
	// ディレクションライトのデータを設定する
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
	light.ptPosition.x = player->GetPosition().x;
	light.ptPosition.y = player->GetPosition().y;
	light.ptPosition.z = player->GetPosition().z;

	// step-3 ポイントライトの初期カラーを設定する
	light.ptColor.x = 1.0f;
	light.ptColor.y = 1.0f;
	light.ptColor.z = 1.0f;

	// step-4 ポイントライトの影響範囲を設定する
	light.ptRange.x = 50.0f;

	//TODO PBR
	//load_texture_from_file(graphics.GetDevice(), L".\\resources\\Stage\\wall.fbm\\wall_lambert1_BaseColor.1001.png", BaseColor.GetAddressOf(), graphics.GetTexture2D());
	//load_texture_from_file(graphics.GetDevice(), L".\\resources\\Stage\\wall.fbm\\wall_lambert1_Normal.1001.png", Normal.GetAddressOf(), graphics.GetTexture2D());
	//load_texture_from_file(graphics.GetDevice(), L".\\resources\\Stage\\wall.fbm\\wall_lambert1_Roughness.1001.png", Roughness.GetAddressOf(), graphics.GetTexture2D());
	load_texture_from_file(graphics.GetDevice(), L".\\resources\\Model\\Jummo\\Textures\\mixbot_low_mixamo_edit1_AlbedoTransparency.png", BaseColor.GetAddressOf(), graphics.GetTexture2D());
	load_texture_from_file(graphics.GetDevice(), L".\\resources\\Model\\Jummo\\Textures\\mixbot_low_mixamo_edit1_Normal.png", Normal.GetAddressOf(), graphics.GetTexture2D());
	load_texture_from_file(graphics.GetDevice(), L".\\resources\\Model\\Jummo\\Textures\\mixbot_low_mixamo_edit1_MetallicSmoothness.png", Roughness.GetAddressOf(), graphics.GetTexture2D());

}

void DemoScene::Finalize()
{
	player->Delete();
	delete player;
	//エネミー終了
	DemoEnemyManager::Instance().Clear();
}

void DemoScene::Update(HWND hwnd, float elapsedTime)
{
	// エフェクト更新処理
	EffectManager::Instance().Update(elapsedTime);

	// 敵の更新処理
	DemoEnemyManager::Instance().Update(elapsedTime);

	Camera& camera = Camera::Instance();

	camera.Update(elapsedTime);
	player->Update(elapsedTime);

	ImGui::Begin("ImGUI");
	ImGui::SliderFloat("dissolve_value", &dissolve_value, -1.0f, +1.0f);
	//TODO PBR
	ImGui::SliderFloat("adjustMetalness", &adjust.adjustMetalness.x, -1.0f, +1.0f);
	ImGui::SliderFloat("adjustSmoothness", &adjust.adjustSmoothness.x, 0.0f, +1.0f);

	ImGui::SliderFloat3("ptPosition", &light.ptPosition.x, -100.0f, +100.0f);
	ImGui::SliderFloat3("ptRange", &light.ptRange.x, 1.0f, +100.0f);

	ImGui::SliderFloat("second", &second, 1.0f, +100.0f);

	ImGui::Checkbox("enableShadow", &enableShadow);
	ImGui::Checkbox("enableSkyMap", &enableSkyMap);
	ImGui::Checkbox("enableBloom", &enableBloom);

	ImGui::SliderFloat("light_view_distance", &light_view_distance, 1.0f, +100.0f);
	ImGui::SliderFloat("light_view_size", &light_view_size, 1.0f, +100.0f);
	ImGui::SliderFloat("light_view_near_z", &light_view_near_z, 1.0f, light_view_far_z - 1.0f);
	ImGui::SliderFloat("light_view_far_z", &light_view_far_z, light_view_near_z + 1.0f, +100.0f);
	ImGui::Image(reinterpret_cast<void*>(double_speed_z->shader_resource_view.Get()), ImVec2(shadowmap_width / 5.0f, shadowmap_height / 5.0f));
	
	// BLOOM
	ImGui::SliderFloat("bloom_extraction_threshold", &bloomer->bloom_extraction_threshold, +0.0f, +5.0f);
	ImGui::SliderFloat("bloom_intensity", &bloomer->bloom_intensity, +0.0f, +5.0f);

	ImGui::End();
}

void DemoScene::Render(float elapsedTime)
{
	Camera& camera = Camera::Instance();
	Lemur::Graphics::Graphics& graphics = Lemur::Graphics::Graphics::Instance();
	ID3D11DeviceContext* immediate_context = graphics.GetDeviceContext();
	ID3D11RenderTargetView* render_target_view = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* depth_stencil_view = graphics.GetDepthStencilView();
	scene_constants data{};

	SetUpRendering();

	// ブレンドステートの設定
	immediate_context->OMSetBlendState(blend_states[static_cast<size_t>(BLEND_STATE::ALPHA)].Get(), nullptr, 0xFFFFFFFF);
	// 深度ステンシルステートの設定
	immediate_context->OMSetDepthStencilState(depth_stencil_states[static_cast<size_t>(DEPTH_STATE::ZT_ON_ZW_ON)].Get(), 0);
	immediate_context->RSSetState(rasterizer_states[static_cast<size_t>(RASTER_STATE::SOLID)].Get());

	camera.SetPerspectiveFov(immediate_context);
	

	// SHADOW : make shadow map
	if(enableShadow)
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
		player->Render(elapsedTime);
		skinned_meshes[1]->render(immediate_context, { -0.01f, 0, 0, 0, 0, 0.01f, 0, 0, 0, 0, 0.01f, 0, 0, 0, 0, 1 }, material_color, nullptr, null_pixel_shader);

		double_speed_z->deactivate(immediate_context);
	}
	// 定数バッファの更新
	{
		DirectX::XMStoreFloat4x4(&data.view_projection, camera.GetViewMatrix() * camera.GetProjectionMatrix());
		data.light_direction = light_direction;
		DirectX::XMStoreFloat4(&data.camera_position, camera.GetEye());

		D3D11_VIEWPORT viewport;
		UINT num_viewports{ 1 };
		immediate_context->RSGetViewports(&num_viewports, &viewport);

		float aspect_ratio{ viewport.Width / viewport.Height };

		// SKYMAP
		if (enableSkyMap)
		{
			DirectX::XMStoreFloat4x4(&data.inv_view_projection, DirectX::XMMatrixInverse(NULL, camera.GetViewMatrix() * camera.GetProjectionMatrix()));
		}
		immediate_context->UpdateSubresource(constant_buffers[0].Get(), 0, 0, &data, 0, 0);
		immediate_context->VSSetConstantBuffers(1, 1, constant_buffers[0].GetAddressOf());
		immediate_context->PSSetConstantBuffers(1, 1, constant_buffers[0].GetAddressOf());

		immediate_context->UpdateSubresource(light_constant_buffer.Get(), 0, 0, &light, 0, 0);
		immediate_context->VSSetConstantBuffers(2, 1, light_constant_buffer.GetAddressOf());
		immediate_context->PSSetConstantBuffers(2, 1, light_constant_buffer.GetAddressOf());

		//TODO PBR
		immediate_context->UpdateSubresource(adjust_constant_buffer.Get(), 0, 0, &adjust, 0, 0);
		immediate_context->VSSetConstantBuffers(3, 1, adjust_constant_buffer.GetAddressOf());
		immediate_context->PSSetConstantBuffers(3, 1, adjust_constant_buffer.GetAddressOf());
	
		// ディゾルブ
		dissolve_constants dissolve{};
		dissolve.parameters.x = dissolve_value;
		immediate_context->UpdateSubresource(dissolve_constant_buffer.Get(), 0, 0, &dissolve, 0, 0);
		immediate_context->VSSetConstantBuffers(4, 1, dissolve_constant_buffer.GetAddressOf());
		immediate_context->PSSetConstantBuffers(4, 1, dissolve_constant_buffer.GetAddressOf());
	}

	// ↓Skinned_mesh オブジェクトと sprite_batch オブジェクトのレンダリング関数の呼び出し
	if (enableBloom)
	{
		framebuffers[0]->clear(immediate_context);
		framebuffers[0]->activate(immediate_context);
	}
	if (enableSkyMap)
	{
		// SKYMAP
		immediate_context->OMSetDepthStencilState(depth_stencil_states[static_cast<size_t>(DEPTH_STATE::ZT_OFF_ZW_OFF)].Get(), 0);
		immediate_context->RSSetState(rasterizer_states[static_cast<size_t>(RASTER_STATE::CULL_NONE)].Get());
		bitBlockTransfer_sky->blit(immediate_context, skymap.GetAddressOf(), 0, 1, pixel_shaders[1].Get());
	}

	//3D描画
	{
		//TODO PBR
		immediate_context->PSSetShaderResources(9/*slot(1番にセットします)*/, 1, noise.GetAddressOf());//TODO
		immediate_context->PSSetShaderResources(10/*slot(1番にセットします)*/, 1, BaseColor.GetAddressOf());//TODO
		immediate_context->PSSetShaderResources(11/*slot(1番にセットします)*/, 1, Normal.GetAddressOf());//TODO
		immediate_context->PSSetShaderResources(12/*slot(1番にセットします)*/, 1, Roughness.GetAddressOf());//TODO

		immediate_context->PSSetShaderResources(8, 1, double_speed_z->shader_resource_view.GetAddressOf());
		immediate_context->OMSetDepthStencilState(depth_stencil_states[static_cast<size_t>(DEPTH_STATE::ZT_ON_ZW_ON)].Get(), 0);
		immediate_context->RSSetState(rasterizer_states[static_cast<size_t>(RASTER_STATE::SOLID)].Get());
		player->Render(elapsedTime);
		skinned_meshes[1]->render(immediate_context, { -0.01f, 0, 0, 0, 0, 0.01f, 0, 0, 0, 0, 0.01f, 0, 0, 0, 0, 1 }, material_color, nullptr, Try.Get());
		
		
		DirectX::XMMATRIX S{ DirectX::XMMatrixScaling(0.01f, 0.01f, 0.01f) };
		DirectX::XMMATRIX R{ DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f) };
		DirectX::XMMATRIX T{ DirectX::XMMatrixTranslation(light.ptPosition.x, light.ptPosition.y, light.ptPosition.z) };
		// ワールド変換行列を作成
		DirectX::XMFLOAT4X4 world;
		DirectX::XMStoreFloat4x4(&world, S* R* T);

		cube->render(immediate_context, world, material_color, nullptr, nullptr);

		//TODO debug
		{
			DirectX::XMFLOAT4X4 view;
			DirectX::XMFLOAT4X4 projection;
			DirectX::XMStoreFloat4x4(&view, camera.GetViewMatrix());
			DirectX::XMStoreFloat4x4(&projection, camera.GetProjectionMatrix());
			graphics.GetDebugRenderer()->Render(immediate_context, view, projection);
		}
	}
	if (enableBloom) 
	{
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
		bitBlockTransfer->blit(immediate_context, shader_resource_views, 0, 2, pixel_shaders[2].Get());

	}

#if 0
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
				dissolve_constants dissolve{};
				dissolve.parameters.x = dissolve_value;
				immediate_context->UpdateSubresource(dissolve_constant_buffer.Get(), 0, 0, &dissolve, 0, 0);
				immediate_context->VSSetConstantBuffers(3, 1, dissolve_constant_buffer.GetAddressOf());
				immediate_context->PSSetConstantBuffers(3, 1, dissolve_constant_buffer.GetAddressOf());
			}
			//dummy_sprite->render(immediate_context, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		}
	}
#endif

	// 3Dエフェクト描画
	{
		DirectX::XMFLOAT4X4 view{};
		DirectX::XMFLOAT4X4 projection{};

		DirectX::XMStoreFloat4x4(&view, camera.GetViewMatrix());
		DirectX::XMStoreFloat4x4(&projection, camera.GetProjectionMatrix());

		EffectManager::Instance().Render(view, projection);
	}
	//Font
	//Lemur::Graphics::Font& font = Lemur::Graphics::Font::Instance();
	//font.render();
}
