#include "ClearScene.h"
#include "SeekerEngine.h"
#include "SceneIncludes.h"

void ClearScene::Init()
{
	Logger::Write("現在シーンClearScene");

	ImGuiManager::GetInstance()->LoadScenesJson();
}

void ClearScene::Update()
{
   
}

void ClearScene::Draw()
{

}

void ClearScene::DrawPostEffect(uint32_t textureSrvIndex)
{
}

void ClearScene::Shutdown()
{
    Editor::GetInstance()->Clear();
}
