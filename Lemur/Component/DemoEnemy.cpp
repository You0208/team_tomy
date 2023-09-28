#include "DemoEnemy.h"

void DemoEnemyGraphicsComponent::Initialize(GameObject* gameobj)
{
	DemoEnemy* demoEnemy = dynamic_cast<DemoEnemy*> (gameobj);
    Lemur::Graphics::Graphics& graphics = Lemur::Graphics::Graphics::Instance();
	demoEnemy->SetModel(ResourceManager::Instance().LoadModelResource(graphics.GetDevice(), ".\\resources\\Stage\\floor_60.fbx"));
}

void DemoEnemyInputComponent::Update(GameObject* gameobj, float elapsedTime)
{
	DemoEnemy* demoEnemy = dynamic_cast<DemoEnemy*> (gameobj);
	//demoEnemy->position.x += elapsedTime * 10.0f;
}

void DemoEnemyGraphicsComponent::Update(GameObject* gameobj)
{
}

void DemoEnemyGraphicsComponent::Render(GameObject* gameobj, float elapsedTime, ID3D11PixelShader* replaced_pixel_shader)
{
	DemoEnemy* demoEnemy = dynamic_cast<DemoEnemy*> (gameobj);

	demoEnemy->Render(elapsedTime,replaced_pixel_shader);
}
