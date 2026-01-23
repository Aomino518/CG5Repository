#pragma once
#include "SeekerEngine.h"
#include "SEFramework.h"
#include "TitleScene.h"
#include "PlayScene.h"

class Game : public SEFramework
{
public:
	void Init() override;
	void Shutdown() override;
	void Update() override;
	void Draw() override;

private:
	std::unique_ptr<TitleScene> scene_;
};

