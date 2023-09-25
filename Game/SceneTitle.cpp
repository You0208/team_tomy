#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneLoading.h"
#include"./Lemur/Graphics/Camera.h"
#include"./Lemur/Resource/ResourceManager.h"
#include"./Lemur/Effekseer/EffekseerManager.h"

void SceneTitle::Initialize()
{
	Lemur::Graphics::Graphics& graphics = Lemur::Graphics::Graphics::Instance();
	SetState();

	HRESULT hr{ S_OK };
	// �V�[���萔�o�b�t�@�I�u�W�F�N�g�𐶐�
	{
		D3D11_BUFFER_DESC buffer_desc{};
		buffer_desc.Usage = D3D11_USAGE_DEFAULT;// �ǂݎ�肨��я������݂��ǂ̂悤�ɍs���邩
		buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;// �o�b�t�@���p�C�v���C���ɂǂ̂悤�Ƀo�C���h(���т�)����邩�����
		buffer_desc.CPUAccessFlags = 0;// CPU �A�N�Z�X �t���O�iCPU �A�N�Z�X���K�v�Ȃ��ꍇ�� 0�j
		buffer_desc.MiscFlags = 0;// ���̑��̃t���O�i���g�p�ɏꍇ��0�j
		buffer_desc.StructureByteStride = 0;//�o�b�t�@���\�����o�b�t�@��\���ꍇ�́A�o�b�t�@�\�����̊e�v�f�̃T�C�Y
		{
			buffer_desc.ByteWidth = sizeof(scene_constants);// �o�b�t�@�̃T�C�Y 
			hr = graphics.GetDevice()->CreateBuffer(&buffer_desc, nullptr, constant_buffers[0].GetAddressOf());
			_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
		}
		{
			buffer_desc.ByteWidth = sizeof(dissolve_constants);
			hr = graphics.GetDevice()->CreateBuffer(&buffer_desc, nullptr, dissolve_constant_buffer.GetAddressOf());
			_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
		}
	}

	framebuffers[0] = std::make_unique<framebuffer>(graphics.GetDevice(), 1280, 720);
	bit_block_transfer = std::make_unique<fullscreen_quad>(graphics.GetDevice());

	hr = XAudio2Create(xaudio2.GetAddressOf(), 0, XAUDIO2_DEFAULT_PROCESSOR);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	hr = xaudio2->CreateMasteringVoice(&master_voice);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	title = std::make_unique<Lemur::Audio::audio>(xaudio2.Get(), L".\\resources\\Audio\\BGM\\Play.wav");

	// ZELDA
	create_ps_from_cso(graphics.GetDevice(), "./Shader/zelda_ps.cso", zelda_ps.GetAddressOf());

	// SHADOW
	//skinned_meshes[1] = std::make_unique<skinned_mesh>(graphics.GetDevice(), ".\\resources\\grid.fbx");
	double_speed_z = std::make_unique<shadow_map>(graphics.GetDevice(), shadowmap_width, shadowmap_height);

	sprdissolve = std::make_unique<sprite_d>(graphics.GetDevice(), L".\\resources\\Image\\black.png");
	sprTitle = std::make_unique<sprite>(graphics.GetDevice(), L".\\resources\\Image\\titile.png");
	sprStart = std::make_unique<sprite>(graphics.GetDevice(), L".\\resources\\Image\\start.png");
	sprEnd = std::make_unique<sprite>(graphics.GetDevice(), L".\\resources\\Image\\end.png");
	for (int i = 0; i < 5; i++)
	{
		sprTitleDeco[i] = std::make_unique<sprite>(graphics.GetDevice(), L".\\resources\\Image\\point.png");
	}

	load_texture_from_file(graphics.GetDevice(), L".\\resources\\Image\\dissolve_animation.png",mask_texture.GetAddressOf(), &mask_texture2dDesc);

	start.size = { 185,76 };
	end.size = { 102, 68 };
	start.pos = { SCREEN_WIDTH / 2 - start.size.x / 2, SCREEN_HEIGHT / 2 + start.size.y / 2 };
	end.pos = { (SCREEN_WIDTH / 2 - end.size.x / 2), (SCREEN_HEIGHT / 2 + end.size.y / 2) + 100 };


	//std::mt19937 mt{ std::random_device{}() };
	//std::uniform_int_distribution<int> pos_1(0, 1920);
	//std::uniform_int_distribution<int> pos_2(0, 1920);
	//std::uniform_int_distribution<int> pos_3(0, 1920);

	decoPosition[0] = { float(rand() % 1280)- 64,float(rand() % 720)};
	decoPosition[1] = { float(rand() % 1280)- 64,float(rand() % 720)};
	decoPosition[2] = { float(rand() % 1280)- 64,float(rand() % 720)};
	decoPosition[3] = { float(rand() % 1280)- 64,float(rand() % 720)};
	decoPosition[4] = { float(rand() % 1280)- 64,float(rand() % 720)};

	decoSize[0] = { float(rand() % 55)+10};
	decoSize[1] = { float(rand() % 55)+10};
	decoSize[2] = { float(rand() % 55)+10};
	decoSize[3] = { float(rand() % 55)+10};
	decoSize[4] = { float(rand() % 55)+10};

	decoW[0] = { float(rand() % 10) *0.1f+0.1f};
	decoW[1] = { float(rand() % 10) *0.1f+0.1f};
	decoW[2] = { float(rand() % 10) *0.1f+0.1f};
	decoW[3] = { float(rand() % 10) *0.1f+0.1f};
	decoW[4] = { float(rand() % 10) *0.1f+0.1f};
}

void SceneTitle::Finalize()
{
}

void SceneTitle::Update(HWND hwnd,float elapsedTime)
{
	title->volume(0.5f);   ///0.0f�`1.0f�̊Ԃŉ��ʒ�����
	title->play(10);
	Camera& camera = Camera::Instance();
	Mouse& mouse = Input::Instance().GetMouse();

	// �G�t�F�N�g�X�V����
	EffectManager::Instance().Update(elapsedTime);

	camera.Update(elapsedTime);

	for (int i = 0; i < 5; ++i)
	{
		decoPosition[i].y -= 500.0f * elapsedTime;
		if (decoPosition[i].y < 0)
		{
			decoPosition[i].y = 720;
			decoPosition[i].x = float(rand() % 1280) - 64;
			decoSize[i] = { float(rand() % 60) + 4 };
			decoW[i] = { float(rand() % 10) * 0.1f + 0.1f };
		}
	}

		DirectX::XMFLOAT3 screenPosition;
		screenPosition.x = static_cast<float>(mouse.GetOldPositionX());
		screenPosition.y = static_cast<float>(mouse.GetOldPositionY());
		if ((screenPosition.x < start.pos.x + start.size.x
			&& start.pos.x < static_cast<float>(mouse.GetOldPositionX()))&& 
			(screenPosition.y < start.pos.y + start.size.y
				&& start.pos.y < static_cast<float>(mouse.GetOldPositionY())))
		{
			color_[0] = 0.5f;
			if (mouse.GetButton() == mouse.BTN_LEFT)
			{
				dissolveStart = true;
				//Lemur::Scene::SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
			}
		}
		else
		{
			color_[0] = 1.0f;
		}
		//END�̕��̔���
		if ((screenPosition.x < end.pos.x + end.size.x
			&& end.pos.x < static_cast<float>(mouse.GetOldPositionX()))&&
			(screenPosition.y < end.pos.y + end.size.y
				&& end.pos.y < static_cast<float>(mouse.GetOldPositionY())))
		{
			color_[1] = 0.5f;
			if (mouse.GetButton() == mouse.BTN_LEFT)
			{
				//�����ɏI���`���Ă�
				//Lemur::Scene::SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
				//TODO
				PostMessage(hwnd, WM_CLOSE, 0, 0);
			}
		}
		else
		{
			color_[1] = 1.0f;
		}

	

	if (dissolveStart)
	{
		dissolve_value -= elapsedTime;
		if (dissolve_value <= 0.0f)
		{
			Lemur::Scene::SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
		}
	}

	//ImGui::Begin("ImGUI");

	//ImGui::End();
}

void SceneTitle::Render(float elapsedTime)
{
	Camera& camera = Camera::Instance();
	Lemur::Graphics::Graphics& graphics = Lemur::Graphics::Graphics::Instance();

	ID3D11DeviceContext* immediate_context = graphics.GetDeviceContext();
	ID3D11RenderTargetView* render_target_view = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* depth_stencil_view = graphics.GetDepthStencilView();

	// bloom�p
	ID3D11RenderTargetView* null_render_target_views[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT]{};
	immediate_context->OMSetRenderTargets(_countof(null_render_target_views), null_render_target_views, 0);
	ID3D11ShaderResourceView* null_shader_resource_views[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT]{};
	immediate_context->VSSetShaderResources(0, _countof(null_shader_resource_views), null_shader_resource_views);
	immediate_context->PSSetShaderResources(0, _countof(null_shader_resource_views), null_shader_resource_views);

	// �����_�[�^�[�Q�b�g���̐ݒ�ƃN���A
	FLOAT color[]{ 0.2f, 0.2f, 0.2f, 1.0f };
	// �L�����o�X�S�̂��w�肵���F�ɓh��Ԃ�
	immediate_context->ClearRenderTargetView(render_target_view, color);
	// �L�����o�X�S�̂̉��s���������Z�b�g����
	immediate_context->ClearDepthStencilView(depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	// ���ꂩ��`���L�����o�X���w�肷��
	immediate_context->OMSetRenderTargets(1, &render_target_view, depth_stencil_view);

	immediate_context->PSSetSamplers(0, 1, sampler_states[static_cast<size_t>(SAMPLER_STATE::POINT)].GetAddressOf());
	immediate_context->PSSetSamplers(1, 1, sampler_states[static_cast<size_t>(SAMPLER_STATE::LINEAR)].GetAddressOf());
	immediate_context->PSSetSamplers(2, 1, sampler_states[static_cast<size_t>(SAMPLER_STATE::ANISOTROPIC)].GetAddressOf());
	immediate_context->PSSetSamplers(3, 1, sampler_states[static_cast<size_t>(SAMPLER_STATE::LINEAR_BORDER_BLACK)].GetAddressOf());
	immediate_context->PSSetSamplers(4, 1, sampler_states[static_cast<size_t>(SAMPLER_STATE::LINEAR_BORDER_WHITE)].GetAddressOf());
	// SHADOW
	immediate_context->PSSetSamplers(5, 1, sampler_states[static_cast<size_t>(SAMPLER_STATE::COMPARISON_LINEAR_BORDER_WHITE)].GetAddressOf());

	// �u�����h�X�e�[�g�̐ݒ�
	immediate_context->OMSetBlendState(blend_states[static_cast<size_t>(BLEND_STATE::ALPHA)].Get(), nullptr, 0xFFFFFFFF);
	// �[�x�X�e���V���X�e�[�g�̐ݒ�
	immediate_context->OMSetDepthStencilState(depth_stencil_states[static_cast<size_t>(DEPTH_STATE::ZT_ON_ZW_ON)].Get(), 0);
	immediate_context->RSSetState(rasterizer_states[static_cast<size_t>(RASTER_STATE::SOLID)].Get());

	camera.SetPerspectiveFov(immediate_context);

	// �萔�o�b�t�@�̍X�V
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

	//3D�`��
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

	//// SHADOW : make shadow map
	//{
	//	using namespace DirectX;

	//	const float aspect_ratio = double_speed_z->viewport.Width / double_speed_z->viewport.Height;
	//	XMVECTOR F{ XMLoadFloat4(&light_view_focus) };
	//	XMVECTOR E{ F - XMVector3Normalize(XMLoadFloat4(&light_direction)) * light_view_distance };
	//	XMVECTOR U{ XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f) };
	//	XMMATRIX V{ XMMatrixLookAtLH(E, F, U) };
	//	XMMATRIX P{ XMMatrixOrthographicLH(light_view_size * aspect_ratio, light_view_size, light_view_near_z, light_view_far_z) };

	//	DirectX::XMStoreFloat4x4(&data.view_projection, V * P);
	//	data.light_view_projection = data.view_projection;
	//	immediate_context->UpdateSubresource(constant_buffers[0].Get(), 0, 0, &data, 0, 0);
	//	immediate_context->VSSetConstantBuffers(1, 1, constant_buffers[0].GetAddressOf());

	//	double_speed_z->clear(immediate_context, 1.0f);
	//	double_speed_z->activate(immediate_context);

	//	ID3D11PixelShader* null_pixel_shader{ NULL };
	//	player->ShadowRender(elapsedTime);
	//	skinned_meshes[1]->render(immediate_context, { -0.01f, 0, 0, 0, 0, 0.01f, 0, 0, 0, 0, 0.01f, 0, 0, 0, 0, 1 }, material_color, nullptr, null_pixel_shader);
	//	double_speed_z->deactivate(immediate_context);
	//}
#endif
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

	//3D�`��
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
		// ������Render
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

	// ������Render
#endif

	// sprite�`��
	{
		if (sprTitle)
		{
			immediate_context->OMSetDepthStencilState(depth_stencil_states[static_cast<size_t>(DEPTH_STATE::ZT_OFF_ZW_OFF)].Get(), 0);
			immediate_context->RSSetState(rasterizer_states[static_cast<size_t>(RASTER_STATE::CULL_NONE)].Get());
			immediate_context->OMSetBlendState(blend_states[static_cast<size_t>(BLEND_STATE::ALPHA)].Get(), nullptr, 0xFFFFFFFF);

			immediate_context->IASetInputLayout(sprite_input_layout.Get());
			immediate_context->VSSetShader(sprite_vertex_shader.Get(), nullptr, 0);
			immediate_context->PSSetShader(sprite_pixel_shader.Get(), nullptr, 0);
			immediate_context->PSSetSamplers(0, 1, sampler_states[static_cast<size_t>(SAMPLER_STATE::LINEAR)].GetAddressOf());
			immediate_context->PSSetShaderResources(1, 1, mask_texture.GetAddressOf());

			// �萔�o�b�t�@�̍X�V�i�f�B�]���u�j
			{
				dissolve_constants dissolve{};
				dissolve.parameters.x = dissolve_value;
				immediate_context->UpdateSubresource(dissolve_constant_buffer.Get(), 0, 0, &dissolve, 0, 0);
				immediate_context->VSSetConstantBuffers(3, 1, dissolve_constant_buffer.GetAddressOf());
				immediate_context->PSSetConstantBuffers(3, 1, dissolve_constant_buffer.GetAddressOf());
			}

			//dummy_sprite->render(immediate_context, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
			sprTitle->render(immediate_context, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
			sprStart->render(immediate_context, start.pos.x, start.pos.y, start.size.x, start.size.y,color_[0], color_[0], color_[0], 1.0f, (0));
			sprEnd->render(immediate_context, end.pos.x, end.pos.y, end.size.x, end.size.y, color_[1], color_[1], color_[1], 1.0f, (0));
			for (int i = 0; i < 5; i++)
			{
				sprTitleDeco[i]->render(immediate_context, decoPosition[i].x, decoPosition[i].y, decoSize[i], decoSize[i],1,1,1, decoW[i],0);
			}
			sprdissolve->render(immediate_context,0,0, 1280,720);

			//sprEnd->render(immediate_context, , endSize.x, endSize.y);
		}
	}


	//// 3D�G�t�F�N�g�`��
	//{
	//	DirectX::XMFLOAT4X4 view{};
	//	DirectX::XMFLOAT4X4 projection{};

	//	DirectX::XMStoreFloat4x4(&view, camera.GetViewMatrix());
	//	DirectX::XMStoreFloat4x4(&projection, camera.GetProjectionMatrix());

	//	EffectManager::Instance().Render(view, projection);
	//}


}
