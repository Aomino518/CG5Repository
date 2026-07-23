#include "GameOverScene.h"
#include "SeekerEngine.h"
#include "SceneIncludes.h"

void GameOverScene::Init()
{
	Logger::Write("現在シーンGameOverScene");

	ImGuiManager::GetInstance()->LoadScenesJson();
}

void GameOverScene::Update()
{
	
}

void GameOverScene::Draw()
{

}

void GameOverScene::Shutdown()
{
    Editor::GetInstance()->Clear();
}
