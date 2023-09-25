//#pragma once
//
//#include "../Lemur/Component/GameObject.h"
//#include "../Lemur/Component/Component.h"
//#include "../Lemur/Graphics/skinned_mesh.h"
//#include "../Lemur/Graphics/Graphics.h"
//#include "../Lemur/Resource/ResourceManager.h"
//#include"../Lemur/Input/Input.h"
//
//
//
//class Projectile :public GameObject
//{
//public:
//    Projectile(InputComponent* input_,
//        PhysicsComponent* physics_,
//        GraphicsComponent* graphics_) :GameObject(input_, physics_, graphics_) {}
//    float ProjectileSpeed = 0.0f;
//    float ProjectileDamage = 0.0f;
//    float ProjectileDamageSpeed = 0.0f;
//
//    float ProjectileLevel = 0.0f;
//
//    bool life;
//
//   
//
//    //位置取得
//    const DirectX::XMFLOAT3& GetPosition() const { return position; }
//    //方向取得
//    const DirectX::XMFLOAT3& GetDirection() const { return direction; }
//    //スケール取得
//    const DirectX::XMFLOAT3& GetScale() const { return scale; }
//}
//
//
////class ProjectilePhysicsComponent :public PhysicsComponent
////{
////    void Initialize(GameObject* gameobj) override;
////    void Update(GameObject* gameobj, float elapsedTime) override;
////};
////
////class ProjectileGraphicsComponent :public GraphicsComponent
////{
////    void Initialize(GameObject* gameobj) override;
////    void Update(GameObject* gameobj) override;
////    void Render(GameObject* gameobj, float elapsedTime,ID3D11PixelShader* replaced_pixel_shader) override;
////
////private:
////    std::shared_ptr<skinned_mesh> DemoPlayerModel; ///モデル待ち
////}