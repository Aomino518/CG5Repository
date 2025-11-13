#pragma once
#include "Camera.h"
#include <memory>
#include <vector>
#include <string>

class CameraManager
{
public:
	struct CameraInfo {
		std::string name;
		std::unique_ptr<Camera> camera;
	};

	void Init();

	int CreateCamera(const std::string& cameraName);

private:
	std::vector<CameraInfo> cameras_;
};

