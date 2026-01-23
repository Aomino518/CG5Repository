#pragma once
#include "SeekerEngine.h"
#include "BaseScene.h"
#include "TitleScene.h"
#include "PlayScene.h"

class SceneManager
{
public:
	void SetNextScene(std::unique_ptr<BaseScene> nextScene) { nextScene_ = std::move(nextScene); }

	void Update();

	void Draw();

	SceneManager();
	~SceneManager();

private:

	std::unique_ptr<BaseScene> scene_;
	std::unique_ptr<BaseScene> nextScene_;
};

