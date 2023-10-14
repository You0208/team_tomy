#include "SceneGame.h"

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
#include "Lemur/Scene/SceneManager.h"
extern QuestPattern quest_pattern;

void GameScene::Initialize()
{
	Lemur::Graphics::Graphics& graphics = Lemur::Graphics::Graphics::Instance();

	// �V�F�[�_�[�֘A
	{
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
				hr = graphics.GetDevice()->CreateBuffer(&buffer_desc, nullptr, constant_buffers[static_cast<size_t>(CONSTANT_BUFFER::SCENE)].GetAddressOf());
				_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
			}
			{ // �t�H�O
				buffer_desc.ByteWidth = sizeof(fog_constants);
				hr = graphics.GetDevice()->CreateBuffer(&buffer_desc, nullptr, constant_buffers[static_cast<size_t>(CONSTANT_BUFFER::FOG)].GetAddressOf());
				_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
			}
			{ // �����t�H�O
				buffer_desc.ByteWidth = sizeof(dis_fog_constants);
				hr = graphics.GetDevice()->CreateBuffer(&buffer_desc, nullptr, constant_buffers[static_cast<size_t>(CONSTANT_BUFFER::D_FOG)].GetAddressOf());
				_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
			}
			{ // �f�B�]���u
				buffer_desc.ByteWidth = sizeof(dissolve_constants);
				hr = graphics.GetDevice()->CreateBuffer(&buffer_desc, nullptr, constant_buffers[static_cast<size_t>(CONSTANT_BUFFER::MASK)].GetAddressOf());
				_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
			}
			{ // PBR
				buffer_desc.ByteWidth = sizeof(adjust_constants);
				hr = graphics.GetDevice()->CreateBuffer(&buffer_desc, nullptr, constant_buffers[static_cast<size_t>(CONSTANT_BUFFER::PBR)].GetAddressOf());
				_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
			}
			{ // ���C�g
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

		//BLOOM
		framebuffers[static_cast<size_t>(FRAME_BUFFER::BLOOM)] = std::make_unique<framebuffer>(graphics.GetDevice(), SCREEN_WIDTH, SCREEN_HEIGHT, true);
		bit_block_transfer[static_cast<size_t>(BIT_BLOCK::BLOOM)] = std::make_unique<fullscreen_quad>(graphics.GetDevice());
		bloomer = std::make_unique<bloom>(graphics.GetDevice(), 1920, 1080);
		create_ps_from_cso(graphics.GetDevice(), "Shader/bloom_ps.cso", pixel_shaders[static_cast<size_t>(PS::BLOOM)].GetAddressOf());

		// SKYMAP
		bit_block_transfer[static_cast<size_t>(BIT_BLOCK::SKY)] = std::make_unique<fullscreen_quad>(graphics.GetDevice());
		create_ps_from_cso(graphics.GetDevice(), "./Shader/skymap_ps.cso", pixel_shaders[static_cast<size_t>(PS::SKY)].GetAddressOf());
		load_texture_from_file(graphics.GetDevice(), L".\\resources\\winter_evening_4k.hdr", skymap.GetAddressOf(), graphics.GetTexture2D());

		// SHADOW
		double_speed_z = std::make_unique<shadow_map>(graphics.GetDevice(), shadowmap_width, shadowmap_height);

		// dissolve
		load_texture_from_file(graphics.GetDevice(), L".\\resources\\Image\\dissolve_animation.png", noise.GetAddressOf(), graphics.GetTexture2D());//TODO


		//TODO �����p
		create_ps_from_cso(graphics.GetDevice(), "./Shader/fog_pbr_ps.cso", Try.GetAddressOf());

		//TODO�@���艟��PBR
		load_texture_from_file(graphics.GetDevice(), L".\\resources\\Model\\Jummo\\Textures\\mixbot_low_mixamo_edit1_AlbedoTransparency.png", BaseColor.GetAddressOf(), graphics.GetTexture2D());
		load_texture_from_file(graphics.GetDevice(), L".\\resources\\Model\\Jummo\\Textures\\mixbot_low_mixamo_edit1_Normal.png", Normal.GetAddressOf(), graphics.GetTexture2D());
		load_texture_from_file(graphics.GetDevice(), L".\\resources\\Model\\Jummo\\Textures\\mixbot_low_mixamo_edit1_MetallicSmoothness.png", Roughness.GetAddressOf(), graphics.GetTexture2D());

		// �V�F�[�_�[�̓ǂݍ���
		{
			// sprite�p�f�t�H���g�`��V�F�[�_�[�i�f�B�]���u�j
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

	// �Q�[������
	{

		// �v���C���[���L�����N�^�[�}�l�[�W�����玝���Ă���
		player = CharacterManager::Instance().GetPlayer();
		// �v���C���[��������
<<<<<<< HEAD
		if (!player->is_initialize)
			player->Initialize();

		player->SkillInit();
=======
		player->Initialize();
		// �v���C���[�ɃV�F�[�_�[���Z�b�g
		player->SetPixelShader(Try.Get());
		// �v���C���[���L�����N�^�[�}�l�[�W���ɃZ�b�g
		CharacterManager::Instance().SetPlayer(player);
>>>>>>> muta

		ColliderManager::Instance().SetCollider(player);

		CreateEnemy_KARI();

	}

	// ���C�g
	{	// �f�B���N�V�������C�g�̃f�[�^��ݒ肷��
	// ���C�g�͉E�����瓖�����Ă���
		light.dirDirection.x = 1.0f;
		light.dirDirection.y = -1.0f;
		light.dirDirection.z = -1.0f;
		DirectX::XMFLOAT3 normalizedDirection{};
		DirectX::XMVECTOR directionVector = DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(normalizedDirection.x, normalizedDirection.y, normalizedDirection.z));
		directionVector = DirectX::XMVector3Normalize(directionVector);
		DirectX::XMStoreFloat3(&normalizedDirection, directionVector);

		light.dirDirection = { normalizedDirection.x,normalizedDirection.y,normalizedDirection.z,0.0f };

		// ���C�g�̃J���[�͔�
		light.dirColor.x = 0.5f;
		light.dirColor.y = 0.5f;
		light.dirColor.z = 0.5f;

		Camera& camera = Camera::Instance();

		// ���_�̈ʒu��ݒ肷��
		DirectX::XMFLOAT3 eyePos;
		DirectX::XMStoreFloat3(&eyePos, camera.GetEye());
		light.eyePos.x = eyePos.x;
		light.eyePos.y = eyePos.y;
		light.eyePos.z = eyePos.z;

		// ����
		light.ambientLight.x = 0.3f;
		light.ambientLight.y = 0.3f;
		light.ambientLight.z = 0.3f;

		// step-2 �|�C���g���C�g�̏������W��ݒ肷��
		light.ptPosition.x = player->GetPosition().x;
		light.ptPosition.y = player->GetPosition().y;
		light.ptPosition.z = player->GetPosition().z;

		// step-3 �|�C���g���C�g�̏����J���[��ݒ肷��
		light.ptColor.x = 1.0f;
		light.ptColor.y = 1.0f;
		light.ptColor.z = 1.0f;

		// step-4 �|�C���g���C�g�̉e���͈͂�ݒ肷��
		light.ptRange.x = 50.0f;

	}
}

void GameScene::Finalize()
{
	//player->Delete();
	//delete player;
	//�G�l�~�[�I��
	EnemyManager::Instance().Clear();
}

void GameScene::Update(HWND hwnd, float elapsedTime)
{
	Pause();

	Camera& camera = Camera::Instance();
	camera.Update(elapsedTime);

	if (!is_update)return;

	timer += elapsedTime;

	// �G�t�F�N�g�X�V����
	EffectManager::Instance().Update(elapsedTime);

	// �G�̍X�V����
	EnemyManager::Instance().Update(elapsedTime);


	player->Update(elapsedTime);

	ColliderManager::Instance().Update();

	QuestClear();

//---------------------------------------------------------------------------------------
// Imgui
//---------------------------------------------------------------------------------------
	ImGui::Begin("Shader");

	// STATIC_BATCHING
	ImGui::Text("drawcall_count : %d", drawcall_count);

	if (ImGui::TreeNode("enable_shader"))
	{
		ImGui::Checkbox("enableShadow", &enableShadow);
		ImGui::Checkbox("enableSkyMap", &enableSkyMap);
		ImGui::Checkbox("enableBloom", &enableBloom);
		ImGui::Checkbox("enableFog", &enableFog);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("dis_fog"))
	{
		ImGui::ColorEdit3("fog_color", &fog_color.x);
		ImGui::SliderFloat("fog_near", &fog_range.x, 0.1f, +100.0f);
		ImGui::SliderFloat("fog_far", &fog_range.y, 0.1f, +100.0f);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("mask"))
	{
		ImGui::SliderFloat("dissolve_value", &dissolve_value, -1.0f, +1.0f);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("fog"))
	{
		ImGui::ColorEdit4("fog_color", fog_constants.fog_color);
		ImGui::SliderFloat("fog_density", &fog_constants.fog_density, +0.0f, +0.005f, "%.4f");
		ImGui::SliderFloat("fog_height_falloff", &fog_constants.fog_height_falloff, +0.0001f, +10.0f, "%.4f");
		ImGui::SliderFloat("fog_cutoff_distance", &fog_constants.fog_cutoff_distance, +0.0f, +1000.0f, "%.4f");
		ImGui::SliderFloat("start_distance", &fog_constants.start_distance, +0.0f, +100.0f, "%.4f");
		ImGui::SliderFloat("time_scale", &fog_constants.time_scale, +0.0f, +10.0f, "%.4f");
		ImGui::SliderFloat("seed_scale", &fog_constants.seed_scale, +0.0f, +0.5f, "%.4f");
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("pbr"))
	{
		ImGui::SliderFloat("adjustMetalness", &pbr.adjustMetalness.x, -1.0f, +1.0f);
		ImGui::SliderFloat("adjustSmoothness", &pbr.adjustSmoothness.x, 0.0f, +1.0f);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("point_light"))
	{
		ImGui::SliderFloat3("ptPosition", &light.ptPosition.x, -100.0f, +100.0f);
		ImGui::SliderFloat3("ptRange", &light.ptRange.x, 1.0f, +100.0f);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("direction_light"))
	{
		ImGui::SliderFloat("light_view_distance", &light_view_distance, 1.0f, +100.0f);
		ImGui::SliderFloat("light_view_size", &light_view_size, 1.0f, +100.0f);
		ImGui::SliderFloat("light_view_near_z", &light_view_near_z, 1.0f, light_view_far_z - 1.0f);
		ImGui::SliderFloat("light_view_far_z", &light_view_far_z, light_view_near_z + 1.0f, +100.0f);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("shadow"))
	{
		ImGui::Image(reinterpret_cast<void*>(double_speed_z->shader_resource_view.Get()), ImVec2(shadowmap_width / 5.0f, shadowmap_height / 5.0f));
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("bloom"))
	{
		ImGui::SliderFloat("bloom_extraction_threshold", &bloomer->bloom_extraction_threshold, +0.0f, +5.0f);
		ImGui::SliderFloat("bloom_intensity", &bloomer->bloom_intensity, +0.0f, +5.0f);
		ImGui::TreePop();
	}
	ImGui::End();
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
	// �[�x�X�e���V���X�e�[�g�̐ݒ�
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
		XMVECTOR F{ XMLoadFloat4(&light_view_focus) };
		XMVECTOR E{ F - XMVector3Normalize(XMLoadFloat4(&light_direction)) * light_view_distance };
		XMVECTOR U{ XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f) };
		XMMATRIX V{ XMMatrixLookAtLH(E, F, U) };
		XMMATRIX P{ XMMatrixOrthographicLH(light_view_size * aspect_ratio, light_view_size, light_view_near_z, light_view_far_z) };

		DirectX::XMStoreFloat4x4(&scene_constants.view_projection, V * P);
		scene_constants.light_view_projection = scene_constants.view_projection;
		immediate_context->UpdateSubresource(constant_buffers[0].Get(), 0, 0, &scene_constants, 0, 0);
		immediate_context->VSSetConstantBuffers(1, 1, constant_buffers[0].GetAddressOf());

		double_speed_z->clear(immediate_context, 1.0f);
		double_speed_z->activate(immediate_context);

		ID3D11PixelShader* null_pixel_shader{ NULL };
		player->Render(elapsedTime);
		skinned_meshes[1]->render(immediate_context, { -0.01f, 0, 0, 0, 0, 0.01f, 0, 0, 0, 0, 0.01f, 0, 0, 0, 0, 1 }, material_color, nullptr, null_pixel_shader);

		double_speed_z->deactivate(immediate_context);
}

	//TODO �v�C��
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
		// UNIT.16
		scene_constants.camera_position = camera_position;
		if (enableFog)
		{
			// FOG
			DirectX::XMStoreFloat4x4(&scene_constants.inverse_projection, DirectX::XMMatrixInverse(NULL, P));
			DirectX::XMStoreFloat4x4(&scene_constants.inverse_view_projection, DirectX::XMMatrixInverse(NULL, V * P));
			scene_constants.time = high_resolution_timer::Instance().time_stamp();
		}
	}
#else
	// ��{�o�b�t�@�[�X�V
	DirectX::XMStoreFloat4x4(&scene_constants.view_projection, camera.GetViewMatrix() * camera.GetProjectionMatrix());
	scene_constants.light_direction = light_direction;
	DirectX::XMStoreFloat4(&scene_constants.camera_position, camera.GetEye());

	// FOG
	DirectX::XMStoreFloat4x4(&scene_constants.inverse_projection, DirectX::XMMatrixInverse(NULL, camera.GetProjectionMatrix()));
	DirectX::XMStoreFloat4x4(&scene_constants.inverse_view_projection, DirectX::XMMatrixInverse(NULL, camera.GetProjectionMatrix() * camera.GetViewMatrix()));
	scene_constants.time = high_resolution_timer::Instance().time_stamp();
#endif

	// �萔�o�b�t�@�̍X�V
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

		// �|�C���g���C�g
		immediate_context->UpdateSubresource(light_constant_buffer.Get(), 0, 0, &light, 0, 0);
		immediate_context->VSSetConstantBuffers(static_cast<size_t>(CONSTANT_BUFFER_R::LIGHT), 1, light_constant_buffer.GetAddressOf());
		immediate_context->PSSetConstantBuffers(static_cast<size_t>(CONSTANT_BUFFER_R::LIGHT), 1, light_constant_buffer.GetAddressOf());

		// PBR
		immediate_context->UpdateSubresource(constant_buffers[static_cast<size_t>(CONSTANT_BUFFER::PBR)].Get(), 0, 0, &pbr, 0, 0);
		immediate_context->VSSetConstantBuffers(static_cast<size_t>(CONSTANT_BUFFER_R::PBR), 1, constant_buffers[static_cast<size_t>(CONSTANT_BUFFER::PBR)].GetAddressOf());
		immediate_context->PSSetConstantBuffers(static_cast<size_t>(CONSTANT_BUFFER_R::PBR), 1, constant_buffers[static_cast<size_t>(CONSTANT_BUFFER::PBR)].GetAddressOf());

		// �f�B�]���u
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

	// �e�N�X�`�����Z�b�g
	{
		// PBR�����p
		immediate_context->PSSetShaderResources(9/*slot(1�ԂɃZ�b�g���܂�)*/, 1, noise.GetAddressOf());//TODO
		immediate_context->PSSetShaderResources(10/*slot(1�ԂɃZ�b�g���܂�)*/, 1, BaseColor.GetAddressOf());//TODO
		immediate_context->PSSetShaderResources(11/*slot(1�ԂɃZ�b�g���܂�)*/, 1, Normal.GetAddressOf());//TODO
		immediate_context->PSSetShaderResources(12/*slot(1�ԂɃZ�b�g���܂�)*/, 1, Roughness.GetAddressOf());//TODO
		// �V���h�E
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
	//3D�`��
	{
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
			DirectX::XMMATRIX C{ DirectX::XMLoadFloat4x4(&coordinate_system_transforms[0])* DirectX::XMMatrixScaling(scale_factor, scale_factor, scale_factor) };
			DirectX::XMMATRIX S{ DirectX::XMMatrixScaling(scaling.x, scaling.y, scaling.z) };
			DirectX::XMMATRIX R{ DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z) };
			DirectX::XMMATRIX T{ DirectX::XMMatrixTranslation(translation.x, translation.y, translation.z) };
			DirectX::XMFLOAT4X4 world;
			DirectX::XMStoreFloat4x4(&world, C * S * R * T);
			// STATIC_BATCHING
			drawcall_count = static_meshes[0]->render(immediate_context, world, material_color);

			scale_factor = 0.01f; // To change the units from centimeters to meters, set 'scale_factor' to 0.01.
			C = { DirectX::XMLoadFloat4x4(&coordinate_system_transforms[0]) * DirectX::XMMatrixScaling(scale_factor, scale_factor, scale_factor) };
			S = { DirectX::XMMatrixScaling(scaling.x, scaling.y, scaling.z) };
			R = { DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z) };
			T = { DirectX::XMMatrixTranslation(translation.x, translation.y, translation.z) };
			DirectX::XMStoreFloat4x4(&world, C * S * R * T);
			skinned_meshes[0]->render(immediate_context, world, material_color, nullptr, Try.Get());
			skinned_meshes[1]->render(immediate_context, world, material_color, nullptr, Try.Get());
			skinned_meshes[2]->render(immediate_context, world, material_color, nullptr, Try.Get());
		}
#endif
			// �v���C���[�`��
			player->Render(elapsedTime);
			EnemyManager::Instance().Render(elapsedTime);
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


	// 2D�`��
#if 0
	// sprite�`��
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

			// �萔�o�b�t�@�̍X�V�i�f�B�]���u�j
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

	//TODO debug
	{
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 projection;
		DirectX::XMStoreFloat4x4(&view, camera.GetViewMatrix());
		DirectX::XMStoreFloat4x4(&projection, camera.GetProjectionMatrix());
		graphics.GetDebugRenderer()->Render(immediate_context, view, projection);
	}
	// 3D�G�t�F�N�g�`��
	{
		DirectX::XMFLOAT4X4 view{};
		DirectX::XMFLOAT4X4 projection{};

		DirectX::XMStoreFloat4x4(&view, camera.GetViewMatrix());
		DirectX::XMStoreFloat4x4(&projection, camera.GetProjectionMatrix());

		EffectManager::Instance().Render(view, projection);

		player->DrawDebugPrimitive();
		EnemyManager::Instance().DrawDebugPrimitive();
	}

	// 2D�f�o�b�O�`��@
	{
		DebugImGui();
		camera.DrawDebug();
	}

}

void GameScene::DebugImGui()
{
	ImGui::Begin("Scene");
	ImGui::Checkbox("is_update", &is_update);
	ImGui::End();
}

void GameScene::CreateEnemy_KARI()
{
	// �G�l�~�[������
	EnemyManager& enemyManager = EnemyManager::Instance();

    switch (quest_pattern)
    {
    case QuestPattern::A:

		for (int i = 0; i < 1; ++i)
		{
			Enemy* enemy = CreateEnemy();
			enemy->Initialize();
			enemy->SetPosition({ DirectX::XMFLOAT3(i * 2.0f, 0, 5) });
			enemyManager.Register(enemy);

			ColliderManager::Instance().SetCollider(enemy);
		}

		break;
    case QuestPattern::B:
		for (int i = 0; i < 2; ++i)
		{
			Enemy* enemy = CreateEnemy();
			enemy->Initialize();
			enemy->SetPosition({ DirectX::XMFLOAT3(i * 2.0f, 0, 5) });
			enemyManager.Register(enemy);

			ColliderManager::Instance().SetCollider(enemy);
		}


    }
}

void GameScene::QuestClear()
{
	int enemy_count = EnemyManager::Instance().GetEnemyCount();

	if (enemy_count <= 0)
	{
		player->SkillFin();
		Lemur::Scene::SceneManager::Instance().ChangeScene(new GambleScene);
	}

}
