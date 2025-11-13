#include "CameraManager.h"

void CameraManager::Init()
{

}

int CameraManager::CreateCamera(const std::string& cameraName)
{
	CameraInfo cameraInfo;
	cameraInfo.name = cameraName;
	cameraInfo.camera = std::make_unique<Camera>();
	cameras_.push_back(std::move(cameraInfo));

	return cameras_.size() - 1;
}
