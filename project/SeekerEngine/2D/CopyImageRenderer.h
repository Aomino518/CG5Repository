#pragma once
#include <cstdint>
#include "PostEffectType.h"

class DxcCompiler;

class CopyImageRenderer
{
public: 
	void Draw(uint32_t sourceTextureSrvIndex, EffectType type);

	EffectType GetEffectType() const { return effectType_; }
	void SetEffectType(EffectType effectType) { this->effectType_ = effectType; }

private:
	EffectType effectType_ = EffectType::Copy;
};

