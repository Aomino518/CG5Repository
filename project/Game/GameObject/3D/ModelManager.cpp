#include "ModelManager.h"

ModelManager* ModelManager::GetInstance()
{
	static ModelManager instance;
	return &instance;
}

void ModelManager::Shutdown()
{
	models_.clear();
	Logger::Write("ModelManager Shutdown");
}

void ModelManager::Init()
{

}

void ModelManager::LoadModel(const std::string& filePath, const std::string& path)
{
	if (models_.contains(filePath)) {
		return;
	}

	// モデル生成とファイル読み込み、初期化
	std::unique_ptr<Model> model = std::make_unique<Model>();
	model->Init("resources", filePath, path);

	// モデルをマップコンテナに格納する
	models_.insert(std::make_pair(filePath, std::move(model)));
}

Model* ModelManager::FindModel(const std::string& filePath)
{
	// 読み込みモデルを検索
	if (models_.contains(filePath)) {
		return models_.at(filePath).get();
	}
	return nullptr;
}
