#pragma once

#include "ILoader.h"

namespace Raven
{
	class AudioLoader : ILoader
	{
	public:
		AudioLoader(ResourceManager& initResourceManager);

		inline static auto Type() { return ELoaderType::LT_Audio; }

		bool AudioLoader::LoadAsset(const std::string& path);

		virtual bool LoadOnGPU() override { return true; }
	};
}