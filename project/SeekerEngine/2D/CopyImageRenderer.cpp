#include "CopyImageRenderer.h"
#include "Graphics.h"
#include "SrvManager.h"
#include "BaseImageRenderer.h"
#include "RenderTexture.h"

void CopyImageRenderer::Draw(uint32_t sourceTextureSrvIndex, EffectType type)
{
    ID3D12GraphicsCommandList* commandList = Graphics::GetCmdList();
   
    BaseImageRenderer::GetInstance()->PreDraw(type);
    SrvManager::GetInstance()->PreDraw();
    SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(0, sourceTextureSrvIndex);

    commandList->DrawInstanced(3, 1, 0, 0);

    Graphics::GetInstance()->AddDrawCallCount();
}
