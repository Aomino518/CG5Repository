#pragma once
#include "Graphics.h"

class RenderTexture
{
public:
	void Create(uint32_t width, uint32_t height);
	void BeginWrite(ID3D12GraphicsCommandList* cmdList);
	void EndWrite(ID3D12GraphicsCommandList* cmdList);

	D3D12_CPU_DESCRIPTOR_HANDLE GetRTV() const;
	uint32_t GetSrvIndex() const;
	ID3D12Resource* GetResource() const;

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> resource_;
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle_{};
	uint32_t srvIndex_ = 0;
	D3D12_RESOURCE_STATES state_ = D3D12_RESOURCE_STATE_RENDER_TARGET;
};

