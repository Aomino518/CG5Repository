#pragma once
#include "SoundCommon.h"
#include <vector>

class Sound
{
public:
	Sound();
	SoundData SoundLoad(const char* filename);
	void SoundUnload(SoundData* soundData);
	
	void PlayBGM(const SoundData& data, bool loop = false, float volume = 1.0f);
	void PlaySE(const SoundData& data, bool loop = false, float volume = 1.0f);
	void StopBGM();
	void StopSE();
	void RestartBGM();
	void SetVolumeBGM(float volume);
	void SetVolumeSE(float volume);

private:
	// 内部用
	static std::wstring ToWide(const char* utf8);
	static std::string  ToLowerExt(const std::string& path);
	SoundData SoundLoadMP3(const wchar_t* wpath);
	SoundData SoundLoadWave(const char* filename);

	SoundCommon* soundCommon_ = nullptr;
	//IXAudio2SourceVoice* pSourceVoice_ = nullptr;
	IXAudio2SourceVoice* bgmVoice_ = nullptr;
	std::vector<IXAudio2SourceVoice*> seVoices_;
	
	bool isLooping_ = false;
	float currentVolume_ = 1.0f;
	float currentBGMVolume_ = 1.0f;
	float currentSEVolume_ = 1.0f;
	SoundData currentData_ = {};
};

