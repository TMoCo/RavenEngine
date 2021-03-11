#include "SceneLoader.h"

namespace Raven
{
	SceneLoader::SceneLoader(ResourceManager& initResourceManager) : ILoader(initResourceManager, ELoaderType::LT_Audio) {}

	bool SceneLoader::LoadAsset(const std::string& path)
	{
		return true;
	}
}