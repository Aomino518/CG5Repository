#include "CopyImageRenderer.h"
#include "Graphics.h"
#include "InputLayout.h"
#include "PsoBuilder.h"
#include "DxcCompiler.h"
#include "BlendStateUtils.h"
#include "SrvManager.h"
#include <assert.h>

void CopyImageRenderer::Init(DxcCompiler& dxcCompiler, ID3D12RootSignature* rootSignature)
{
    assert(rootSignature);

    rootSignature_ = rootSignature;

    Microsoft::WRL::ComPtr<IDxcBlob> vsBlob =
        dxcCompiler.CompileShader(L"resources/hlsl/CopyImage.VS.hlsl", L"vs_6_0");

    Microsoft::WRL::ComPtr<IDxcBlob> psBlob =
        dxcCompiler.CompileShader(L"resources/hlsl/CopyImage.PS.hlsl", L"ps_6_0");

    InputLayout inputLayout;
    D3D12_INPUT_LAYOUT_DESC inputLayoutDesc = inputLayout.CreateInputLayoutOffScreen();

    D3D12_BLEND_DESC blendDesc = CreateBlendDesc(kBlendModeNone);

    D3D12_RASTERIZER_DESC rasterizerDesc{};
    rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
    rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

    D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
    depthStencilDesc.DepthEnable = false;
    depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
    depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;

    PsoBuilder builder;
    builder.Init(Graphics::GetInstance());

    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = builder.CreatePsoDesc(
        rootSignature_,
        inputLayoutDesc,
        vsBlob,
        psBlob,
        blendDesc,
        rasterizerDesc,
        depthStencilDesc,
        D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
    );

    pso_ = builder.BuildPso(psoDesc);
}

void CopyImageRenderer::Draw(uint32_t textureSrvIndex)
{
    ID3D12GraphicsCommandList* commandList = Graphics::GetCmdList();
    assert(commandList);
    assert(rootSignature_);
    assert(pso_);

    commandList->SetGraphicsRootSignature(rootSignature_.Get());
    commandList->SetPipelineState(pso_.Get());
    commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    SrvManager::GetInstance()->PreDraw();
    SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(0, textureSrvIndex);

    commandList->DrawInstanced(3, 1, 0, 0);

    Graphics::GetInstance()->AddDrawCallCount();
}
