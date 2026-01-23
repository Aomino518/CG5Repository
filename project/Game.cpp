#include "Game.h"

void Game::Init()
{
	SEFramework::Init();

	scene_ = std::make_unique<TitleScene>();
	scene_->Init();
}

void Game::Shutdown()
{
	SEFramework::Shutdown();
}

void Game::Update()
{
	SEFramework::Update();
	scene_->Update();
}

void Game::Draw()
{
	SpriteCommon::GetInstance()->DrawCommon();
	scene_->Draw();
}
