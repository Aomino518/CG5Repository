#include "CopyImageRenderer.h"
#include "Graphics.h"
#include "InputLayout.h"
#include "PsoBuilder.h"
#include "DxcCompiler.h"
#include "BlendStateUtils.h"
#include "SrvManager.h"
#include <assert.h>

CopyImageRenderer* CopyImageRenderer::GetInstance()
{
    static CopyImageRenderer instance;
    return &instance;
}

void CopyImageRenderer::Init(DxcCompiler& dxcCompiler, ID3D12RootSignature* rootSignature)
{
    assert(rootSignature);

    rootSignature_ = rootSignature;

    Microsoft::WRL::ComPtr<IDxcBlob> vsBlob =
        dxcCompiler.CompileShader(L"resources/hlsl/Fullscreen.VS.hlsl", L"vs_6_0");

    struct ShaderInfo { EffectType type; std::wstring path; };
    std::vector<ShaderInfo> shaders = {
        {EffectType::Copy, L"resources/hlsl/Fullscreen.PS.hlsl"},
        {EffectType::Grayscale, L"resources/hlsl/Grayscale.PS.hlsl"},
        {EffectType::Sepia, L"resources/hlsl/Sepia.PS.hlsl"},
        {EffectType::Vignetting, L"resources/hlsl/Vignetting.PS.hlsl"},
    };

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

    for (const auto& shader : shaders) {
        Microsoft::WRL::ComPtr<IDxcBlob> psBlob =
            dxcCompiler.CompileShader(shader.path.c_str(), L"ps_6_0");

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

        psoMap_[shader.type] = builder.BuildPso(psoDesc);
    }
}

void CopyImageRenderer::Draw(uint32_t textureSrvIndex, EffectType type)
{
    ID3D12GraphicsCommandList* commandList = Graphics::GetCmdList();
    assert(commandList);
    assert(rootSignature_);
    assert(psoMap_.count(type));

    commandList->SetGraphicsRootSignature(rootSignature_.Get());
    commandList->SetPipelineState(psoMap_[type].Get());
    commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    SrvManager::GetInstance()->PreDraw();
    SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(0, textureSrvIndex);

    commandList->DrawInstanced(3, 1, 0, 0);

    Graphics::GetInstance()->AddDrawCallCount();
}
