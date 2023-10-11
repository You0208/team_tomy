#include "framework.h"
#include "shader.h"
#include "texture.h"
#include "Graphics.h"
#include "..\Scene\SceneManager.h"
#include "./Game/Scene/SceneGame.h"
#include "./Game/Scene/TitleScene.h"

// Effect
#include "../Effekseer/EffekseerManager.h"



framework::framework(HWND hwnd) 
	: hwnd(hwnd),
	graphics(hwnd,FALSE),
	input(hwnd)
{
}

bool framework::initialize()
{
	// エフェクトマネージャー初期化
	EffectManager::Instance().Initialize();

	// シーン初期化
	//Lemur::Scene::SceneManager::Instance().ChangeScene(new DemoScene);


    //merge前にこれをコメント化
	//Lemur::Scene::SceneManager::Instance().ChangeScene(new TitleScene);
	Lemur::Scene::SceneManager::Instance().ChangeScene(new GameScene);

	return true;
}

void framework::update(float elapsed_time/*Elapsed seconds from last frame*/)
{
	// ImGui更新
	IMGUI_CTRL_CLEAR_FRAME();

	// 入力更新処理
	input.Update();

	// シーン更新処理
	Lemur::Scene::SceneManager::Instance().Update(hwnd,elapsed_time);

	// 時間経過更新
	timer += elapsed_time;
}

void framework::render(float elapsed_time/*Elapsed seconds from last frame*/)
{
	using namespace Lemur::Graphics;
	// 別スレッド中にデバイスコンテキストが使われていた場合に
	// 同時にアクセスしないように排他制御をする
	std::lock_guard<std::mutex> lock(graphics.GetMutex());
	// シーン描画処理
	Lemur::Scene::SceneManager::Instance().Render(elapsed_time);

	// ImGui表示（この下にも何も描画しない）
	IMGUI_CTRL_DISPLAY();
	graphics.render(elapsed_time);
	// ↓この下は何も出ない
}

bool framework::uninitialize()
{
	return true;
}

framework::~framework()
{
	Lemur::Scene::SceneManager::Instance().Clear();

	// エフェクトマネージャー終了化
	EffectManager::Instance().Finalize();
}