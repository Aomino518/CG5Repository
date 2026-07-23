#pragma once
#include "SceneIncludes.h"
#include "BaseScene.h"
#include "Particle2DEmitter.h"
#include "CopyImageRenderer.h"
#include <cstdint>

class TitleScene : public BaseScene
{
public:
	// 初期化
	void Init() override;

	// 更新
	void Update() override;

	// 描画
	void Draw() override;

	void DrawPostEffect(uint32_t textureSrvIndex) override;

	void Shutdown() override;

	const char* GetSceneName() const override { return "TITLE"; }

private:
	std::unique_ptr<Entity3D> terrain_;
	std::unique_ptr<CopyImageRenderer> gaussian_;
};