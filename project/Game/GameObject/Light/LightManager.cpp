#include "LightManager.h"

LightManager* LightManager::GetInstance()
{
    static LightManager instance;
    return &instance;
}

void LightManager::Init()
{
    // 平行光源用のリソース
    directionalLightResource_ = CreateBufferResource(Graphics::GetDevice(), sizeof(DirectionalLight));
    directionalLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&dirLight_));
    // 初期化値
    dirLight_->color = { 1.0f, 1.0f, 1.0f, 1.0f };
    dirLight_->direction = { 1.0f, 0.0f, 0.0f };
    dirLight_->intensity = 0.0f;

    pointLightGroupResource_ = CreateBufferResource(Graphics::GetDevice(), sizeof(PointLightGroup));
    pointLightGroupResource_->Map(0, nullptr, reinterpret_cast<void**>(&pointLightGroup_));
    // 明示的にメモリを0クリア
    memset(pointLightGroup_, 0, sizeof(PointLightGroup));

    spotLightGroupResource_ = CreateBufferResource(Graphics::GetDevice(), sizeof(SpotLightGroup));
    spotLightGroupResource_->Map(0, nullptr, reinterpret_cast<void**>(&spotLightGroup_));
    // 明示的にメモリを0クリア
    memset(spotLightGroup_, 0, sizeof(SpotLightGroup));
}

void LightManager::Update()
{
    uint32_t pointCount = 0;
    for (auto& [name, entry] : pointLights_) {
        if (pointCount >= kMaxPointLights) { break; }
        pointLightGroup_->lights[pointCount] = entry;
        ++pointCount;
    }
    pointLightGroup_->count = static_cast<int32_t>(pointCount);

    for (uint32_t i = pointCount; i < kMaxPointLights; ++i) {
        pointLightGroup_->lights[i] = {};
    }

    uint32_t spotCount = 0;
    for (auto& [name, entry] : spotLights_) {
        if (spotCount >= kMaxSpotLights) { break; }
        spotLightGroup_->lights[spotCount] = entry;
        ++spotCount;
    }
    spotLightGroup_->count = static_cast<int32_t>(spotCount);

    for (uint32_t i = spotCount; i < kMaxSpotLights; ++i) {
        spotLightGroup_->lights[i] = {};
    }
}

void LightManager::Shutdown()
{
    directionalLightResource_.Reset();
    pointLightGroupResource_.Reset();
    spotLightGroupResource_.Reset();

    pointLights_.clear();
    spotLights_.clear();
    Logger::Write("LightManager Shutdown");
}

void LightManager::CreatePointLight(const std::string& name)
{
    // 同名がないかチェック
    if (pointLights_.find(name) != pointLights_.end()) {
        Logger::Write(Logger::LogLevel::Warning, "PointLight already exists: " + name);
        return;
    }

    PointLight entry{};
    // 初期化値
    entry.color = { 1.0f, 1.0f, 1.0f, 1.0f };
    entry.position = { 0.0f, 2.0f, 2.0f };
    entry.intensity = 1.0f;
    entry.radius = 5.0f;
    entry.decay = 1.0f;

    pointLights_.emplace(name, std::move(entry));
}

void LightManager::CreateSpotLight(const std::string& name)
{
    // 同名がないかチェック
    if (spotLights_.find(name) != spotLights_.end()) {
        Logger::Write(Logger::LogLevel::Warning, "SpotLight already exists: " + name);
        return;
    }

    SpotLight entry{};
    // スポットライトのリリース
    entry.color = { 1.0f, 1.0f, 1.0f, 1.0f };
    entry.position = { -10.0f, 1.0f, 0.0f };
    entry.intensity = 15.0f;
    entry.decay = 2.0f;
    entry.cosAngle = std::cos(std::numbers::pi_v<float> / 3.0f);
    entry.cosFalloffStart = 1.0f;
    entry.direction = Normalize({ -1.0f, -1.0f, 0.0f });
    entry.distance = 20.0f;

    spotLights_.emplace(name, std::move(entry));
}

PointLight* LightManager::GetPointLight(const std::string& name)
{
    auto it = pointLights_.find(name);
    if (it == pointLights_.end()) {
        return nullptr;
    }
    return &(it->second);
}

SpotLight* LightManager::GetSpotLight(const std::string& name)
{
    auto it = spotLights_.find(name);
    if (it == spotLights_.end()) {
        return nullptr;
    }
    return &(it->second);
}

void LightManager::SetDirectionalLight(DirectionalLight* dirLight)
{
    dirLight_->direction = dirLight->direction;
    dirLight_->color = dirLight->color;
    dirLight_->intensity = dirLight->intensity;
}

void LightManager::SetPointLight(std::string& name, PointLight* pointLight)
{
    auto it = pointLights_.find(name);
    if (it == pointLights_.end()) {
        return;
    }

    it->second.position = pointLight->position;
    it->second.color = pointLight->color;
    it->second.intensity = pointLight->intensity;
    it->second.radius = pointLight->radius;
    it->second.decay = pointLight->decay;
}

void LightManager::SetSpotLight(std::string& name, SpotLight* spotLight)
{
    auto it = spotLights_.find(name);
    if (it == spotLights_.end()) {
        return;
    }

    it->second.position = spotLight->position;
    it->second.color = spotLight->color;
    it->second.intensity = spotLight->intensity;
    it->second.distance = spotLight->distance;
    it->second.decay = spotLight->decay;
    it->second.direction = spotLight->direction;
    it->second.cosAngle = spotLight->cosAngle;
    it->second.cosFalloffStart = spotLight->cosFalloffStart;
}
