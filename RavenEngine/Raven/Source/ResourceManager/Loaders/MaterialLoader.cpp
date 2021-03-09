#include "MaterialLoader.h"

namespace Raven
{
	MaterialLoader::MaterialLoader() : ILoader(ELoaderType::LT_Material) {}

	bool MaterialLoader::LoadAsset(const std::string& path)
	{
		return true;
	}
}