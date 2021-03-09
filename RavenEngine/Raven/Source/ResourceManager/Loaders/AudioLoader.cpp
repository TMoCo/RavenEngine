#include "AudioLoader.h"

namespace Raven
{
	AudioLoader::AudioLoader() : ILoader(ELoaderType::LT_Audio) {}

	bool AudioLoader::LoadAsset(const std::string& path)
	{
		return true;
	}
}