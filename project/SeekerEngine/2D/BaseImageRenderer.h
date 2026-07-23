#pragma once
#include <cstdint>
#include <d3d12.h>
#include <wrl.h>
#include <unordered_map>
#include "PostEffectType.h"
#include "RenderTexture.h"

class DxcCompiler;
class BaseImageRenderer
{
public:
	// シングルトンインスタンスの取得
	static BaseImageRenderer* GetInstance();
	void Init(DxcCompiler& dxcCompiler, ID3D12RootSignature* rootSignature);
	void Shutdown();
	void PreDraw(EffectType type);

private:
	BaseImageRenderer() = default;
	~BaseImageRenderer() = default;
	BaseImageRenderer(const BaseImageRenderer&) = delete;
	BaseImageRenderer& operator=(const BaseImageRenderer&) = delete;

	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;
	std::unordered_map<EffectType, Microsoft::WRL::ComPtr<ID3D12PipelineState>> psoMap_;
};

