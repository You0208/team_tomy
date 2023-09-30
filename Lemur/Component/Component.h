#pragma once
#include "GameObject.h"
#include "misc.h"
#include "../Graphics/skinned_mesh.h"
#include "../Graphics/shader.h"
#include "../Graphics/texture.h"
#include <filesystem>
#include <sstream>
#include <functional>

#include <fstream>
class GameObject;

// �R���|�[�l���g���N���X
class Component
{
public:
    virtual ~Component() {}
};

// �C���v�b�g�p���N���X
class InputComponent 
{
public:
    virtual ~InputComponent() {}
    virtual void Initialize(GameObject* gameobj) = 0;
    virtual void Update(GameObject* gameobj, float elapsedTime) = 0;
};

// �O���t�B�b�N�p���N���X
class GraphicsComponent 
{
public:
    virtual ~GraphicsComponent() {}
    virtual void Initialize(GameObject* gameobj) = 0;
    virtual void Update(GameObject* gameobj) = 0;
    virtual void Render(GameObject* gameobj, float elapsedTime, ID3D11PixelShader* replaced_pixel_shader) = 0;
};

// �����p���N���X
class PhysicsComponent 
{
public:
    virtual ~PhysicsComponent() {}
    virtual void Initialize(GameObject* gameobj) = 0;
    virtual void EnemyInitialize(GameObject* gameobj,int StartTime,int EnemyType) {};
    virtual void Update(GameObject* gameobj, float elapsedTime) = 0;
};
