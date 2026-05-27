#include "TitleScene.h"
#include "SceneIncludes.h"

void TitleScene::Init()
{
    Logger::Write("現在シーンTitleScene");
   
    auto camMgr = CameraManager::GetInstance();
    Entity3DCommon::GetInstance()->SetCameraManager(camMgr);
    Entity3DCommon::GetInstance()->SetDebugCamera(camMgr->GetDebugCamera());

    terrain_ = std::make_unique<Entity3D>();
    ModelManager::GetInstance()->LoadModel("terrain.obj");
    terrain_->Init();
    terrain_->SetModel("terrain");
    terrain_->SetTranslate({ 0.0f, 0.0f, 0.0f });

    ImGuiManager::GetInstance()->LoadScenesJson();
}

void TitleScene::Update()
{
    auto camMgr = CameraManager::GetInstance();

    if (Input::GetInstance()->IsTrigger(DIK_ESCAPE)) {
        EndRequset();
    }

    if (Input::GetInstance()->IsTrigger(DIK_SPACE)) {
        SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
    }

    terrain_->SetCamera(camMgr->GetActiveCamera());
    terrain_->Update();
}

void TitleScene::Draw()
{
    terrain_->Draw();
}

void TitleScene::Shutdown()
{
    Editor::GetInstance()->Clear();
}
