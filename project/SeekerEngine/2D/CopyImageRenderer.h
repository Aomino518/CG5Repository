#pragma once
#include <cstdint>
#include <d3d12.h>
#include <wrl.h>
#include <unordered_map>

enum class EffectType {
	Copy,
	Grayscale,
	Sepia,
	Vignetting,
	BoxFilter3x3,
	BoxFilter5x5,
};

class DxcCompiler;

class CopyImageRenderer
{
public: 
	// シングルトンインスタンスの取得
	static CopyImageRenderer* GetInstance();
	void Init(DxcCompiler& dxcCompiler, ID3D12RootSignature* rootSignature);
	void Draw(uint32_t textureSrvIndex, EffectType type);

private:
	CopyImageRenderer() = default;
	~CopyImageRenderer() = default;
	CopyImageRenderer(const CopyImageRenderer&) = delete;
	CopyImageRenderer& operator=(const CopyImageRenderer&) = delete;

	Microsoft::WRL::ComPtr<ID3D12PipelineState> pso_;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;
	std::unordered_map<EffectType, Microsoft::WRL::ComPtr<ID3D12PipelineState>> psoMap_;
};

