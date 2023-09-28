#pragma once
#include "GameObject.h"
#include "Component.h"
#include "../Graphics/skinned_mesh.h"
#include "../Graphics/Graphics.h"
#include "../Resource/ResourceManager.h"
#include"../Input/Input.h"

// 実体で生成されるのがこいつ
class DemoPlayer :public GameObject
{
public:
    DemoPlayer() {}
    DemoPlayer(InputComponent* input_,
        PhysicsComponent* physics_,
        GraphicsComponent* graphics_) :GameObject(input_, physics_, graphics_) {}



};

//こいつらは実体にはならない。コンポーネントとして実体になるやつに搭載される。

class DemoPlayerInputComponent :public InputComponent
{
    void Initialize(GameObject* gameobj) override {}
    void Update(GameObject* gameobj, float elapsedTime) override;

    DirectX::XMFLOAT3 GetMoveVec(float input_x,float input_y);

};

class DemoPlayerPhysicsComponent :public PhysicsComponent
{
    void Initialize(GameObject* gameobj) override;
    void Update(GameObject* gameobj, float elapsedTime) override {}
};

class DemoPlayerGraphicsComponent:public GraphicsComponent
{
    void Initialize(GameObject* gameobj) override;
    void Update(GameObject* gameobj) override;
    void Render(GameObject* gameobj, float elapsedTime,ID3D11PixelShader* replaced_pixel_shader) override;
private:
    std::shared_ptr<skinned_mesh> DemoPlayerModel;
};