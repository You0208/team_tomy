#pragma once
#include <d3d11.h>
#pragma once

#include <wrl.h>
#include <directxmath.h>
#include <vector>
#include <unordered_map>
#include <string>
#include <fbxsdk.h>

// シリアライズ
#include <cereal/archives/binary.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/set.hpp>
#include <cereal/types/unordered_map.hpp>

#include "Model.h"
namespace DirectX
{
    template<class T>
    void serialize(T& archive, DirectX::XMFLOAT2& v)
    {
        archive(
            cereal::make_nvp("x", v.x),
            cereal::make_nvp("y", v.y)
        );
    }

    template<class T>
    void serialize(T& archive, DirectX::XMFLOAT3& v)
    {
        archive(
            cereal::make_nvp("x", v.x),
            cereal::make_nvp("y", v.y),
            cereal::make_nvp("z", v.z)
        );
    }

    template<class T>
    void serialize(T& archive, DirectX::XMFLOAT4& v)
    {
        archive(
            cereal::make_nvp("x", v.x),
            cereal::make_nvp("y", v.y),
            cereal::make_nvp("z", v.z),
            cereal::make_nvp("w", v.w)
        );
    }

    template<class T>
    void serialize(T& archive, DirectX::XMFLOAT4X4& m)
    {
        archive(
            cereal::make_nvp("_11", m._11), cereal::make_nvp("_12", m._12),
            cereal::make_nvp("_13", m._13), cereal::make_nvp("_14", m._14),
            cereal::make_nvp("_21", m._21), cereal::make_nvp("_22", m._22),
            cereal::make_nvp("_23", m._23), cereal::make_nvp("_24", m._24),
            cereal::make_nvp("_31", m._31), cereal::make_nvp("_32", m._32),
            cereal::make_nvp("_33", m._33), cereal::make_nvp("_34", m._34),
            cereal::make_nvp("_41", m._41), cereal::make_nvp("_42", m._42),
            cereal::make_nvp("_43", m._43), cereal::make_nvp("_44", m._44)
        );
    }
}

struct skeleton
{
    struct bone
    {
        uint64_t unique_id{ 0 };
        std::string name;
        // 「parent_index」は、それ自体を含む配列内の親ボーンの位置を参照するインデックスです
        int64_t parent_index{ -1 }; // -1 : the bone is orphan
        // 「node_index」は、シーンのノード配列を参照するインデックス
        int64_t node_index{ 0 };

        // 「offset_transform」は、モデル (メッシュ) 空間からボーン (ノード) シーンに変換するために使用されます
        DirectX::XMFLOAT4X4 offset_transform{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };

        bool is_orphan() const { return parent_index < 0; };

        // シリアライズ
        template<class T>
        void serialize(T& archive)
        {
            archive(unique_id, name, parent_index, node_index, offset_transform);
        }
    };
    std::vector<bone> bones;

    int64_t indexof(uint64_t unique_id) const
    {
        int64_t index{ 0 };
        for (const bone& bone : bones)
        {
            if (bone.unique_id == unique_id)
            {
                return index;
            }
            ++index;
        }
        return -1;
    }
    template<class T>
    void serialize(T& archive)
    {
        archive(bones);
    }
};

struct animation
{
    std::string name;
    float sampling_rate{ 0 };

    struct keyframe
    {
        struct node
        {
            // ノードのローカル空間からシーンのグローバル空間に変換するために使用 
            DirectX::XMFLOAT4X4 global_transform{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };

            // ノードの変換データには、その親に対する移動、回転、およびスケーリングベクトルが含まれる
            DirectX::XMFLOAT3 scaling{ 1, 1, 1 };
            DirectX::XMFLOAT4 rotation{ 0, 0, 0, 1 }; // 回転クオータニオン
            DirectX::XMFLOAT3 translation{ 0, 0, 0 };

            template<class T>
            void serialize(T& archive)
            {
                archive(global_transform, scaling, rotation, translation);
            }
        };
        std::vector<node> nodes;
        template<class T>
        void serialize(T& archive)
        {
            archive(nodes);
        }
    };
    std::vector<keyframe> sequence;
    template<class T>
    void serialize(T& archive)
    {
        archive(name, sampling_rate, sequence);
    }
};

struct scene
{
    struct node
    {
        uint64_t unique_id{ 0 };
        std::string name;
        FbxNodeAttribute::EType attribute{ FbxNodeAttribute::EType::eUnknown };
        int64_t parent_index{ -1 };
        template<class T>
        void serialize(T& archive)
        {
            archive(unique_id, name, attribute, parent_index);
        }

    };
    std::vector<node> nodes;
    int64_t indexof(uint64_t unique_id) const
    {
        int64_t index{ 0 };
        for (const node& node : nodes)
        {
            if (node.unique_id == unique_id)
            {
                return index;
            }
            ++index;
        }
        return -1;
    }
    template<class T>
    void serialize(T& archive)
    {
        archive(nodes);
    }
};
// STATIC_BATCHING
struct material
{
    uint64_t unique_id{ 0 };
    std::string name;

    DirectX::XMFLOAT4 Ka{ 0.2f, 0.2f, 0.2f, 1.0f };
    DirectX::XMFLOAT4 Kd{ 0.8f, 0.8f, 0.8f, 1.0f };
    DirectX::XMFLOAT4 Ks{ 1.0f, 1.0f, 1.0f, 1.0f };

    std::string texture_filenames[4];
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shader_resource_views[4];
   //TODO material実験
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> metalness_smoothness;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> metalness;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> roughness;

    // UNIT.30
    template<class T>
    void serialize(T& archive)
    {
        archive(unique_id, name, Ka, Kd, Ks, texture_filenames);
    }
};

class skinned_mesh
{
public:
    static const int MAX_BONE_INFLUENCES{ 4 };

    struct vertex
    {
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT3 normal;
        DirectX::XMFLOAT4 tangent;
        DirectX::XMFLOAT2 texcoord;
        // ボーン
        float bone_weights[MAX_BONE_INFLUENCES]{ 1, 0, 0, 0 };
        uint32_t bone_indices[MAX_BONE_INFLUENCES]{};

        template<class T>
        void serialize(T& archive)
        {
            archive(position, normal, tangent, texcoord, bone_weights, bone_indices);
        }
    };
    // 定数バッファー
    static const int MAX_BONES{ 256 };
    struct constants
    {
        DirectX::XMFLOAT4X4 world;
        DirectX::XMFLOAT4 material_color;
        DirectX::XMFLOAT4X4 bone_transforms[MAX_BONES]{ { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 } };
        float threshold = 1.0f;
        float pads[3];
    };
    float dissolve = 1.0f;
    struct mesh
    {

        uint64_t unique_id{ 0 };
        std::string name;
        // 「node_index」は、シーンのノード配列を参照するインデックス
        int64_t node_index{ 0 };

        std::vector<vertex> vertices;
        std::vector<uint32_t> indices;

        struct subset
        {
            uint64_t material_unique_id{ 0 };
            std::string material_name;

            uint32_t start_index_location{ 0 }; // GPU によってインデックス バッファから読み取られる最初のインデックスの場所
            uint32_t index_count{ 0 }; // Number of indices to draw.

            template<class T>
            void serialize(T& archive)
            {
                archive(material_unique_id, material_name, start_index_location, index_count);
            }
        };
        std::vector<subset> subsets;
        // RAYCAST
        const subset* find_subset(uint32_t index) const
        {
            for (const subset& subset : subsets)
            {
                if (subset.start_index_location <= index && subset.start_index_location + subset.index_count > index)
                {
                    return &subset;
                }
            }
            return nullptr;
        }


        DirectX::XMFLOAT4X4 default_global_transform
            = { 1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1 };

        skeleton bind_pose;

        DirectX::XMFLOAT3 bounding_box[2]
        {
            { +D3D11_FLOAT32_MAX, +D3D11_FLOAT32_MAX, +D3D11_FLOAT32_MAX },
            { -D3D11_FLOAT32_MAX, -D3D11_FLOAT32_MAX, -D3D11_FLOAT32_MAX }
        };

        template<class T>
        void serialize(T& archive)
        {
            archive(unique_id, name, node_index, subsets, default_global_transform, bind_pose, bounding_box, vertices, indices);
        }
    private:
        Microsoft::WRL::ComPtr<ID3D11Buffer> vertex_buffer;
        Microsoft::WRL::ComPtr<ID3D11Buffer> index_buffer;
        friend class skinned_mesh;
    };
    std::vector<mesh> meshes;
#if 0
    //このクラスの外で材料構造を定義することに
    struct material
    {
        uint64_t unique_id{ 0 };
        std::string name;

        DirectX::XMFLOAT4 Ka{ 0.2f, 0.2f, 0.2f, 1.0f };
        DirectX::XMFLOAT4 Kd{ 0.8f, 0.8f, 0.8f, 1.0f };
        DirectX::XMFLOAT4 Ks{ 1.0f, 1.0f, 1.0f, 1.0f };

        std::string texture_filenames[4];
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shader_resource_views[4];

        template<class T>
        void serialize(T& archive)
        {
            archive(unique_id, name, Ka, Kd, Ks, texture_filenames);
        }
    };
#endif
    std::unordered_map<uint64_t, material> materials;

    std::vector<animation> animation_clips;

    //現在のアニメーションの指定したノード取得
    void ComputeAnimation(int animation_index, int frame, animation::keyframe& keyframe);
    //現在のアニメーションの指定したノード取得
    void ComputeAnimation(int animation_index, int node_index, int frame, animation::keyframe::node& node)
    {
        node = animation_clips.at(animation_index).sequence.at(frame).nodes.at(node_index);
    }


private:
    Microsoft::WRL::ComPtr<ID3D11VertexShader> vertex_shader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shader;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> input_layout;
    Microsoft::WRL::ComPtr<ID3D11Buffer> constant_buffer;

    void create_com_objects(ID3D11Device* device, const char* fbx_filename, bool used_as_collider/*RAYCAST*/);


    // ルートモーションで移動した総量
    DirectX::XMFLOAT3 cacheRootMotionTranslation = { 0,0,0 };

    // 1フレームでルートモーションで移動した量
    DirectX::XMFLOAT3 rootMotionTranslation = { 0,0,0 };

    // ルートモーションするノードの位置
    DirectX::XMFLOAT3 root_node_pos{0,0,0};

public:
    // ルートモーションするか
    bool rootMotionFlag = false;

public:
    skinned_mesh(ID3D11Device* device, const char* fbx_filename, bool triangulate = false, float sampling_rate = 0/*UNIT.25*/, bool used_as_collider = false/*RAYCAST*/);
    skinned_mesh(ID3D11Device* device, const char* fbx_filename, std::vector<std::string>& animation_filenames, bool triangulate = false, float sampling_rate = 0);
    virtual ~skinned_mesh() = default;

    void render(ID3D11DeviceContext* immediate_context, const DirectX::XMFLOAT4X4& world, const DirectX::XMFLOAT4& material_color, const animation::keyframe* keyframe, ID3D11PixelShader* replaced_pixel_shader);
    void render(ID3D11DeviceContext* immediate_context, const DirectX::XMFLOAT4X4& world, const DirectX::XMFLOAT4& material_color, const animation::keyframe* keyframe, ID3D11PixelShader** replaced_pixel_shader);
    void update_animation(animation::keyframe& keyframe);
    void update_animation(float elapsedTIme);



    bool append_animations(const char* animation_filename, float sampling_rate /*0:use default damage_value*/);
    void blend_animations(const animation::keyframe* keyframes[2], float factor, animation::keyframe& keyframe);

    /*------------------- ルートモーションで使うから作りました。 by:tomy --------------------*/
    
    // ノードのインデックス番号検索
    int FindMeshNodeIndex(const char* find_name)
    {
        for (int scene_view_i = 0; scene_view_i < scene_view.nodes.size(); scene_view_i++)
        {
            if (scene_view.nodes.at(scene_view_i).name == find_name)
            {
                return scene_view_i;
            }
        }
        _ASSERT_EXPR(false, L"ノードが見つからんかった");
    }
    //// ボーンのインデックス番号検索
    //int FindBoneNodeIndex(const char* find_name, const int mesh_index)
    //{
    //    for (int bone_node_i = 0; bone_node_i < meshes.at(mesh_index).bind_pose.bones.size(); bone_node_i++)
    //    {
    //        if (meshes.at(mesh_index).bind_pose.bones.at(bone_node_i).name == find_name)
    //        {
    //            return bone_node_i;
    //        }
    //    }
    //    _ASSERT_EXPR(false, L"ボーンノード見つからんかった");
    //}

    //void SetupRootMotion(const char* rootMotionMeshNodeName)
    //{
    //    root_motion_node_index = FindMeshNodeIndex(rootMotionMeshNodeName);
    //}

    //// joint_position関数で使うからやむを得ず引数に
    //void ComputeRootMotion(animation::keyframe keyframe,DirectX::XMFLOAT4X4 world)
    //{
    //    if (!rootMotionFlag) return;
    //    // ルートモーション設定されてなかったらreturnする。
    //    if (root_motion_node_index < 0)return;

    //    // ルートモーションするノードデータ。
    //    animation::keyframe::node& rootMotionNode = keyframe.nodes.at(root_motion_node_index);


    //    //std::string mesh_name = meshes.at(root_motion_node_index).name;
    //    //std::string bone_name = meshes.at(root_motion_node_index).bind_pose.bones.at(root_motion_boneNode_index).name;
    //    //root_node_pos = joint_position(mesh_name, bone_name, &keyframe, world);

    //    // 前フレームと今回のフレームの移動値データの差分量を求める
    //    rootMotionTranslation.x = rootMotionNode.translation.x - cacheRootMotionTranslation.x;
    //    rootMotionTranslation.y = rootMotionNode.translation.y - cacheRootMotionTranslation.y;
    //    rootMotionTranslation.z = rootMotionNode.translation.z - cacheRootMotionTranslation.z;

    //    // 次のフレームに差分量を求めるために今回の移動値をキャッシュする
    //    cacheRootMotionTranslation = rootMotionNode.translation;

    //    //// アニメーション内で移動してほしくないのでルートモーション移動値をリセット
    //    rootMotionNode.translation = { 0,0,0 };

    //    // ルートモーションフラグをオフにする
    //    rootMotionFlag = false;

    //}

    //void UpdateRootMotion(DirectX::XMFLOAT3& position, animation::keyframe keyframe, DirectX::XMFLOAT4X4 world)
    //{
    //    if (root_motion_node_index < 0)return;

    //    // ルートモーションしてるノードデータ。
    //    const animation::keyframe::node& root_motion_node = keyframe.nodes.at(root_motion_node_index);

    //    //std::string mesh_name = meshes.at(root_motion_node_index).name;
    //    //std::string bone_name = meshes.at(root_motion_node_index).bind_pose.bones.at(root_motion_boneNode_index).name;
    //    //DirectX::XMFLOAT3 root_node_pos = joint_position(mesh_name, bone_name, &keyframe, world);
    //    //DirectX::XMMATRIX Node_World = DirectX::XMMatrixTranslation(root_node_pos.x, root_node_pos.y, root_node_pos.z);

    //    // ルートモーションで移動する移動ベクトル
    //    DirectX::XMVECTOR Translation = DirectX::XMLoadFloat3(&rootMotionTranslation);
    //    // ルートモーションしてるノードデータのワールド座標
    //    DirectX::XMMATRIX World = DirectX::XMLoadFloat4x4(&root_motion_node.global_transform);

    //    // ワールド座標から移動ベクトル分移動したとこが新しい位置になる。
    //    DirectX::XMVECTOR Position = DirectX::XMVector3Transform(Translation, World);
    //    DirectX::XMStoreFloat3(&position, Position);

    //    // 次フレームで引き継がれないようにリセット。
    //    rootMotionTranslation = { 0,0,0 };
    //}


    /*----------------------------------------------------------------------------------*/


    // RAYCAST
    // The coordinate system of all function arguments is world space.
    bool raycast(const DirectX::XMFLOAT4& position/*ray position*/, const DirectX::XMFLOAT4& direction/*ray direction*/, const DirectX::XMFLOAT4X4& world_transform, DirectX::XMFLOAT4& closest_point, DirectX::XMFLOAT3& intersected_normal,
        std::string& intersected_mesh, std::string& intersected_material);

    // JOINT POSITION
    DirectX::XMFLOAT3 joint_position(
        const std::string& mesh_name,
        const std::string& bone_name,
        const animation::keyframe* keyframe,
        const DirectX::XMFLOAT4X4& transform)
    {
        DirectX::XMFLOAT3 position = {};/*world space*/

        for (const skinned_mesh::mesh& mesh : meshes)
        {
            if (mesh.name == mesh_name)
            {
                for (const skeleton::bone& bone : mesh.bind_pose.bones)
                {
                    if (bone.name == bone_name)
                    {
                        const animation::keyframe::node& node = keyframe->nodes.at(bone.node_index);
                        DirectX::XMFLOAT4X4 global_transform = node.global_transform;
                        position.x = global_transform._41;
                        position.y = global_transform._42;
                        position.z = global_transform._43;
                        DirectX::XMMATRIX M = DirectX::XMLoadFloat4x4(&mesh.default_global_transform) * DirectX::XMLoadFloat4x4(&transform); // todo　ルートモーションにこれ使える
                        DirectX::XMStoreFloat3(&position, DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&position), M));
                        return position;
                    }
                }
            }
        }

        _ASSERT_EXPR(FALSE, "Joint not found.");

        return {};
    }
    DirectX::XMFLOAT3 joint_position(
        size_t mesh_index,
        size_t bone_index,
        const animation::keyframe* keyframe,
        const DirectX::XMFLOAT4X4& transform)
    {
        DirectX::XMFLOAT3 position = {};/*world space*/

        const skinned_mesh::mesh& mesh = meshes.at(mesh_index);
        const skeleton::bone& bone = mesh.bind_pose.bones.at(bone_index);
        const animation::keyframe::node& node = keyframe->nodes.at(bone.node_index);
        DirectX::XMFLOAT4X4 global_transform = node.global_transform;
        position.x = global_transform._41;
        position.y = global_transform._42;
        position.z = global_transform._43;
        DirectX::XMMATRIX M = DirectX::XMLoadFloat4x4(&mesh.default_global_transform) * DirectX::XMLoadFloat4x4(&transform);
        DirectX::XMStoreFloat3(&position, DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&position), M));
        return position;
    }

    // テクスチャ読み込み
    HRESULT LoadTexture(ID3D11Device* device, const char* filename, const char* suffix, bool dummy, ID3D11ShaderResourceView** srv, UINT dummy_color = 0xFFFFFFFF);

protected:
    scene scene_view;
    void fetch_meshes(FbxScene* fbx_scene, std::vector<mesh>& meshes);
    void fetch_materials(FbxScene* fbx_scene, std::unordered_map<uint64_t, material>& materials);
    void fetch_skeleton(FbxMesh* fbx_mesh, skeleton& bind_pose);
    void fetch_animations(FbxScene* fbx_scene, std::vector<animation>& animation_clips, float sampling_rate /*この値が 0 の場合、アニメーション データはデフォルトのフレーム レートでサンプリングされる*/);
    void fetch_scene(const char* fbx_filename, bool triangulate, float sampling_rate/*If this damage_value is 0, the animation data will be sampled at the default frame rate.*/);
};

// STATIC_BATCHING
class static_mesh
{
public:
    struct vertex
    {
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT3 normal;
        DirectX::XMFLOAT4 tangent;
        DirectX::XMFLOAT2 texcoord;
        template<class T>
        void serialize(T& archive)
        {
            archive(position, normal, tangent, texcoord);
        }
    };
    struct constants
    {
        DirectX::XMFLOAT4X4 world;
        DirectX::XMFLOAT4 material_color;
    };
    struct mesh
    {
        uint64_t unique_id{ 0 };
        std::string name;
        // 'node_index' is an index that refers to the node array of the scene.
        int64_t node_index{ 0 };

        std::vector<vertex> vertices;
        std::vector<uint32_t> indices;

        struct subset
        {
            uint64_t material_unique_id{ 0 };
            std::string material_name;

            uint32_t start_index_location{ 0 }; // The location of the first index read by the GPU from the index buffer.
            uint32_t index_count{ 0 }; // Number of indices to draw.

            template<class T>
            void serialize(T& archive)
            {
                archive(material_unique_id, material_name, start_index_location, index_count);
            }
        };
        std::vector<subset> subsets;

        DirectX::XMFLOAT4X4 default_global_transform{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };

        DirectX::XMFLOAT3 bounding_box[2]
        {
            { +D3D11_FLOAT32_MAX, +D3D11_FLOAT32_MAX, +D3D11_FLOAT32_MAX },
            { -D3D11_FLOAT32_MAX, -D3D11_FLOAT32_MAX, -D3D11_FLOAT32_MAX }
        };

        template<class T>
        void serialize(T& archive)
        {
            archive(unique_id, name, node_index, subsets, default_global_transform, bounding_box, vertices, indices);
        }
    private:
        Microsoft::WRL::ComPtr<ID3D11Buffer> vertex_buffer;
        Microsoft::WRL::ComPtr<ID3D11Buffer> index_buffer;
        friend class static_mesh;
    };
    std::vector<mesh> meshes;

    std::unordered_map<uint64_t, material> materials;

private:
    Microsoft::WRL::ComPtr<ID3D11VertexShader> vertex_shader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shader;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> input_layout;
    Microsoft::WRL::ComPtr<ID3D11Buffer> constant_buffer;
    void create_com_objects(ID3D11Device* device, const char* fbx_filename);

    // STATIC_BATCHING
    bool batching = true;

public:
    static_mesh(ID3D11Device* device, const char* fbx_filename, bool triangulate = false);
    virtual ~static_mesh() = default;

    size_t render(ID3D11DeviceContext* immediate_context, const DirectX::XMFLOAT4X4& world, const DirectX::XMFLOAT4& material_color);

protected:
    scene scene_view;
    void fetch_meshes(FbxScene* fbx_scene, std::vector<mesh>& meshes);
    void fetch_materials(FbxScene* fbx_scene, std::unordered_map<uint64_t, material>& materials);
    void fetch_scene(const char* fbx_filename, bool triangulate);
};

class static_mesh_batch
{
public:
    struct vertex
    {
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT3 normal;
        DirectX::XMFLOAT4 tangent;
        DirectX::XMFLOAT2 texcoord;
        template<class T>
        void serialize(T& archive)
        {
            archive(position, normal, tangent, texcoord);
        }
    };
    struct constants
    {
        DirectX::XMFLOAT4X4 world;
        DirectX::XMFLOAT4 material_color;
    };

    std::unordered_map<uint64_t, material> materials;
    std::unordered_map<uint64_t/*material_unique_id*/, std::vector<vertex>> vertices;
    std::unordered_map<uint64_t/*material_unique_id*/, Microsoft::WRL::ComPtr<ID3D11Buffer>> vertex_buffers;

private:
    Microsoft::WRL::ComPtr<ID3D11VertexShader> vertex_shader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shader;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> input_layout;
    Microsoft::WRL::ComPtr<ID3D11Buffer> constant_buffer;
    void create_com_objects(ID3D11Device* device, const char* fbx_filename);

public:
    static_mesh_batch(ID3D11Device* device, const char* fbx_filename, bool triangulate = false);
    virtual ~static_mesh_batch() = default;

    size_t render(ID3D11DeviceContext* immediate_context, const DirectX::XMFLOAT4X4& world, const DirectX::XMFLOAT4& material_color);

protected:
    scene scene_view;
    void fetch_meshes(FbxScene* fbx_scene);
    void fetch_materials(FbxScene* fbx_scene, std::unordered_map<uint64_t, material>& materials);
    void fetch_scene(const char* fbx_filename, bool triangulate);
};
