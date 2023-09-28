#include "sprite.h"
#include "misc.h"
#include "texture.h"
#include "shader.h"
#include <sstream>
#include <functional>

sprite::sprite(ID3D11Device* device, const wchar_t* filename)
{
    // ���_�o�b�t�@�I�u�W�F�N�g�̐���
    HRESULT hr{ S_OK };

    // ���_���̃Z�b�g�i0.5�Ȃ̂�NDC������j
    vertex vertices[]
    {
     { { -1.0, +1.0, 0 }, { 1, 1, 1, 1 }, { 0, 0 } },
     { { +1.0, +1.0, 0 }, { 1, 1, 1, 1 }, { 1, 0 } },
     { { -1.0, -1.0, 0 }, { 1, 1, 1, 1 }, { 0, 1 } },
     { { +1.0, -1.0, 0 }, { 1, 1, 1, 1 }, { 1, 1 } },
    };

    // ���_�o�b�t�@�I�u�W�F�N�g�̐���
    D3D11_BUFFER_DESC buffer_desc{};
    buffer_desc.ByteWidth = sizeof(vertices);// �o�b�t�@�[�̃T�C�Y
    buffer_desc.Usage = D3D11_USAGE_DYNAMIC;// �o�b�t�@�[�̓ǂݎ��Ə������ݕ��@
    buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER; 
    buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // CPU�A�N�Z�X�t���O�i�Ȃ��ꍇ��0�j
    buffer_desc.MiscFlags = 0; // ���̑��̃t���O�i���g�p�̏ꍇ��0�j
    buffer_desc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA subresource_data{};
    subresource_data.pSysMem = vertices;// �������f�[�^�ւ̃|�C���^�[
    subresource_data.SysMemPitch = 0; // �e�N�X�`���� 1 �s�̐擪���玟�̍s�܂ł̋���
    subresource_data.SysMemSlicePitch = 0; // 1 �̐[�x���x���̐擪���玟�̐[�����x���܂ł̋���
    // GPU��VRAM�Ƀo�b�t�@���쐬���Ă���
    hr = device->CreateBuffer(&buffer_desc, &subresource_data, vertex_buffer.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    // ���̓��C�A�E�g�I�u�W�F�N�g�̐���
    // ""�̒��g�̓Z�}���e�B�b�N
    D3D11_INPUT_ELEMENT_DESC input_element_desc[]
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
            D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
            D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
        D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    create_vs_from_cso(device, "Shader/sprite_vs.cso", vertex_shader.GetAddressOf(), input_layout.GetAddressOf(), input_element_desc, _countof(input_element_desc));
    create_ps_from_cso(device, "Shader/sprite_ps.cso", pixel_shader.GetAddressOf());

    load_texture_from_file(device, filename, shader_resource_view.GetAddressOf(), &texture2d_desc);
}

void sprite::render(ID3D11DeviceContext* immediate_context,
    float dx, float dy,// ��`�̍���̍��W�i�X�N���[�����W�n�j
    float dw, float dh, // ��`�̃T�C�Y�i�X�N���[�����W�n�j
    float r, float g, float b, float a,
    float angle/*degree*/)
{
    render(immediate_context, dx, dy, dw, dh, r, g, b, a, angle, 0.0f, 0.0f, static_cast<float>(texture2d_desc.Width), static_cast<float>(texture2d_desc.Height));
}

void sprite::render(ID3D11DeviceContext* immediate_context, float dx, float dy, float dw, float dh, float r, float g, float b, float a, float angle, float sx, float sy, float sw, float sh)
{
    // �X�N���[���i�r���[�|�[�g�j�̃T�C�Y���擾����
    D3D11_VIEWPORT viewport{};
    UINT num_viewports{ 1 };
    // RSGetViewports �͌��݃o�C���h����Ă���r���[�|�[�g�̐���ϐ��ɑ��
    immediate_context->RSGetViewports(&num_viewports, &viewport);

    //�@render �����o�֐��̈����idx, dy, dw, dh�j�����`�̊e���_�̈ʒu�i�X�N���[�����W�n�j���v�Z����
    // (x0, y0) *----* (x1, y1)
    //         |   / |
    //         |  /  |
    //         | /   |
    //         |/    |
    // (x2, y2) *----* (x3, y3) 

    // left-top
    float x0{ dx };
    float y0{ dy };
    // right-top
    float x1{ dx + dw };
    float y1{ dy };
    // left-bottom
    float x2{ dx };
    float y2{ dy + dh };
    // right-bottom
    float x3{ dx + dw };
    float y3{ dy + dh };

    // �����_��
    //auto rotate = [](float& x, float& y, float cx, float cy, float angle)
    std::function<void(float&, float&, float, float, float)> rotate = [](float& x, float& y, float cx, float cy, float angle)
    {
        x -= cx;
        y -= cy;

        float cos{ cosf(DirectX::XMConvertToRadians(angle)) };
        float sin{ sinf(DirectX::XMConvertToRadians(angle)) };
        float tx{ x }, ty{ y };
        x = cos * tx + -sin * ty;
        y = sin * tx + cos * ty;

        x += cx;
        y += cy;
    };
    //��]�̒��S����`�̒��S�_�ɂ����ꍇ
    float cx = dx + dw * 0.5f;
    float cy = dy + dh * 0.5f;
    rotate(x0, y0, cx, cy, angle);
    rotate(x1, y1, cx, cy, angle);
    rotate(x2, y2, cx, cy, angle);
    rotate(x3, y3, cx, cy, angle);

    // �X�N���[�����W�n���� NDC �ւ̍��W�ϊ��������Ȃ�
    x0 = 2.0f * x0 / viewport.Width - 1.0f;
    y0 = 1.0f - 2.0f * y0 / viewport.Height;
    x1 = 2.0f * x1 / viewport.Width - 1.0f;
    y1 = 1.0f - 2.0f * y1 / viewport.Height;
    x2 = 2.0f * x2 / viewport.Width - 1.0f;
    y2 = 1.0f - 2.0f * y2 / viewport.Height;
    x3 = 2.0f * x3 / viewport.Width - 1.0f;
    y3 = 1.0f - 2.0f * y3 / viewport.Height;

    // �v�Z���ʂŒ��_�o�b�t�@�I�u�W�F�N�g���X�V����
    HRESULT hr{ S_OK };
    D3D11_MAPPED_SUBRESOURCE mapped_subresource{};
    // D3D11_MAP_WRITE_DISCARD�����\�[�X�͏������ݗp�Ƀ}�b�v����܂�
    hr = immediate_context->Map(vertex_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource);
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    vertex* vertices{ reinterpret_cast<vertex*>(mapped_subresource.pData) };
    if (vertices != nullptr)
    {
        vertices[0].position = { x0, y0 , 0 };
        vertices[1].position = { x1, y1 , 0 };
        vertices[2].position = { x2, y2 , 0 };
        vertices[3].position = { x3, y3 , 0 };
        vertices[0].color = vertices[1].color = vertices[2].color = vertices[3].color = { r,g,b,a };

        vertices[0].texcoord = { sx / texture2d_desc.Width, sy / texture2d_desc.Height };
        vertices[1].texcoord = { (sx + sw) / texture2d_desc.Width, sy / texture2d_desc.Height };
        vertices[2].texcoord = { sx / texture2d_desc.Width, (sy + sh) / texture2d_desc.Height };
        vertices[3].texcoord = { (sx + sw) / texture2d_desc.Width, (sy + sh) / texture2d_desc.Height };
    }

    immediate_context->Unmap(vertex_buffer.Get(), 0);

    // ���_�o�b�t�@�[�̃o�C���h
    UINT stride{ sizeof(vertex) };
    UINT offset{ 0 };
    immediate_context->IASetVertexBuffers(0, 1, vertex_buffer.GetAddressOf(), &stride, &offset);

    // �v���~�e�B�u�^�C�v����уf�[�^�̏����Ɋւ�����̃o�C���h
    // D3D11_PRIMITIVE(��{�`)_TOPOLOGY(�Ȃ���)_TRIANGLESTRIP(STRIP���Ȃ����Ă����)
    immediate_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    // ���̓��C�A�E�g�I�u�W�F�N�g�̃o�C���h
    immediate_context->IASetInputLayout(input_layout.Get());

    // �V�F�[�_�[�̃o�C���h
    immediate_context->VSSetShader(vertex_shader.Get(), nullptr, 0);
    immediate_context->PSSetShader(pixel_shader.Get(), nullptr, 0);

    //�V�F�[�_�[ ���\�[�X�̃o�C���h
    immediate_context->PSSetShaderResources(0, 1, shader_resource_view.GetAddressOf());

    // �v���~�e�B�u�̕`��
    immediate_context->Draw(4, 0);
}

void sprite::render(ID3D11DeviceContext* immediate_context, float dx, float dy, float dw, float dh)
{
    render(immediate_context, dx, dy, dw, dh, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        0.0f, 0.0f, static_cast<float>(texture2d_desc.Width), static_cast<float>(texture2d_desc.Height));
}

void sprite::textout(ID3D11DeviceContext* immediate_context, std::string s, float x, float y, float w, float h, float r, float g, float b, float a)
{
    float sw = static_cast<float>(texture2d_desc.Width / 16);
    float sh = static_cast<float>(texture2d_desc.Height / 16);
    float carriage = 0;
    for (const char c : s)
    {
        render(immediate_context, x + carriage, y, w, h, r, g, b, a, 0, sw * (c & 0x0F), sh * (c >> 4), sw, sh);
        carriage += w;
    }
}
