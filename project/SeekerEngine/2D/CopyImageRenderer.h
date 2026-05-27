#pragma once
#include <cstdint>
#include <d3d12.h>
#include <wrl.h>

class DxcCompiler;

class CopyImageRenderer
{
public: 
	void Init(DxcCompiler& dxcCompiler, ID3D12RootSignature* rootSignature);
	void Draw(uint32_t textureSrvIndex);

private:
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pso_;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;
};

