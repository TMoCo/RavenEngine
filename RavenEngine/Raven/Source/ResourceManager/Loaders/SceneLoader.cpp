#include "SceneLoader.h"

namespace Raven
{
	SceneLoader::SceneLoader() : ILoader(ELoaderType::LT_Audio) {}

	bool SceneLoader::LoadAsset(const std::string& path)
	{
		return true;
	}
}