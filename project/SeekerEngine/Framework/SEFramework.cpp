#include "SEFramework.h"

void SEFramework::Init()
{
}

void SEFramework::Shutdown()
{
}

void SEFramework::Update()
{
}

void SEFramework::Run()
{
	engine_.Init();

	Init();

	sceneManager_ = std::make_unique<SceneManager>();

	// ウィンドウの×ボタンが押されるまでループ
	while (engine_.GetApp()->ProcessMessage()) {
		engine_.Update();
		Update();
		sceneManager_->Update();

		if (IsEndRequst()) {
			break;
		}

		/*-- 描画処理 --*/
		engine_.BegineFrame();
		sceneManager_->Draw();
		Draw();
		engine_.EndFrame();
	}

	Shutdown();
	engine_.Shutdown();
}
