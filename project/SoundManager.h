#pragma once
#include "SoundCommon.h"
#include "Sound.h"
#include <string>
#include <unordered_map>

class SoundManager
{
public:
	static SoundManager* GetInstance();

	void Init();
	void Shutdown();

	void Load(const std::string& name, const std::string& filepath);

	void PlaySE(const std::string& name, bool loop = false, float volume = 1.0f);
	void PlayBGM(const std::string& name, bool loop = true, float volume = 1.0f);
	void StopSE();
	void StopBGM();
	void SetVolumeSE(float volume);
	void SetVolumeBGM(float volume);

	void Unload(const std::string& name);

private:
	SoundManager() = default;
	~SoundManager() = default;
	SoundManager(const SoundManager&) = delete;
	SoundManager& operator=(const SoundManager&) = delete;

	Sound sound_;
	std::unordered_map<std::string, SoundData> soundDatas_;

	SoundData* currentBGM_ = nullptr;
};

