#pragma once
#include "SeekerEngine.h"
#include "CreateResorceUtils.h"
#include <map>
#include <nlohmann/json.hpp>
#include "JsonTransform.h"

class LightManager
{
public:
	static LightManager* GetInstance();

	void Init();
	void Update();
	void Shutdown();

	// ライト作成関数
	void CreatePointLight(const std::string& name);
	void CreateSpotLight(const std::string& name);

	// Getter
	const DirectionalLight& GetDirectionalLight() const { return *dirLight_; }
	const std::map<std::string, PointLight>& GetPointLights() const { return pointLights_; };
	const std::map<std::string, SpotLight>& GetSpotLights() const { return spotLights_; };
	PointLight* GetPointLight(const std::string& name);
	SpotLight* GetSpotLight(const std::string& name);
	const Microsoft::WRL::ComPtr<ID3D12Resource> GetDirLightResource() const { return directionalLightResource_; }
	const Microsoft::WRL::ComPtr<ID3D12Resource> GetPointLightGroupResource() const { return pointLightGroupResource_; }
	const Microsoft::WRL::ComPtr<ID3D12Resource> GetSpotLightGroupResource() const { return spotLightGroupResource_; }

	// Setter
	void SetDirectionalLight(DirectionalLight* dirLight);
	void SetPointLight(std::string& name, PointLight* pointLight);
	void SetSpotLight(std::string& name, SpotLight* spotLight);

	// Json保存と読み込み
	json SaveToJson() const;
	void LoadFromJson(const json& j);

private:
	LightManager() = default;
	~LightManager() = default;
	LightManager(const LightManager&) = delete;
	LightManager& operator=(const LightManager&) = delete;

	Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightResource_ = nullptr;
	DirectionalLight* dirLight_ = nullptr;

	std::map<std::string, PointLight> pointLights_;
	std::map<std::string, SpotLight> spotLights_;

	// GPU送信用の配列CB
	Microsoft::WRL::ComPtr<ID3D12Resource> pointLightGroupResource_ = nullptr;
	PointLightGroup* pointLightGroup_ = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> spotLightGroupResource_ = nullptr;
	SpotLightGroup* spotLightGroup_ = nullptr;
};

