#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <directxmath.h>
#include <vector>

class sprite_batch
{
private:

    // �����o�ϐ�
    Microsoft::WRL::ComPtr<ID3D11VertexShader> vertex_shader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shader;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> input_layout;
    Microsoft::WRL::ComPtr<ID3D11Buffer> vertex_buffer;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shader_resource_view;
    D3D11_TEXTURE2D_DESC texture2d_desc;

public:
    // ���_�t�H�[�}�b�g�i���R�ɍ���j
    struct vertex
    {
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT4 color;
        DirectX::XMFLOAT2 texcoord;
    };

private:
    const size_t max_vertices;
    std::vector<vertex> vertices;

public:
    // �R���X�g���N�^�E�f�X�g���N�^
    sprite_batch(ID3D11Device* device, const wchar_t* filename, size_t max_sprites);
    ~sprite_batch() {}

    // �����o�֐�
    void begin(ID3D11DeviceContext* immediate_context,
        ID3D11PixelShader* replaced_pixel_shader = nullptr, ID3D11ShaderResourceView* replaced_shader_resource_view = nullptr);
    void end(ID3D11DeviceContext* immediate_context);

    void render(ID3D11DeviceContext*,
        float dx, float dy,// ��`�̍���̍��W�i�X�N���[�����W�n�j
        float dw, float dh, // ��`�̃T�C�Y�i�X�N���[�����W�n�j
        float r, float g, float b, float a,
        float angle/*degree*/);
    // �I�[�o�[���[�h
    void render(ID3D11DeviceContext*,
        float dx, float dy,// ��`�̍���̍��W�i�X�N���[�����W�n�j
        float dw, float dh, // ��`�̃T�C�Y�i�X�N���[�����W�n�j
        float r, float g, float b, float a,
        float angle/*degree*/,
        float sx, float sy, float sw, float sh);
    void render(ID3D11DeviceContext* immediate_context, float dx, float dy, float dw, float dh);

};