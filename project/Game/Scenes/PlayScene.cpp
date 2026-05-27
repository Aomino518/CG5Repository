#include "PlayScene.h"
#include "SceneIncludes.h"

void PlayScene::Init()
{
    Logger::Write("現在シーンPlayScene");
   
    ImGuiManager::GetInstance()->LoadScenesJson();
}

void PlayScene::Update()
{
    /*-- 更新処理 --*/
    if (Input::GetInstance()->IsTrigger(DIK_SPACE)) {
        SceneManager::GetInstance()->ChangeScene("TITLE");
    }

}

void PlayScene::Draw()
{
    /*-- 描画処理 --*/

}

void PlayScene::Shutdown()
{
    Editor::GetInstance()->Clear();
}
