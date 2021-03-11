#include "MeshLoader.h"

namespace Raven
{
	// calls the inherited constructor
	MeshLoader::MeshLoader(ResourceManager& initResourceManager) : ILoader(initResourceManager, ELoaderType::LT_Mesh) {}

	bool MeshLoader::LoadAsset(const std::string& path)
	{
		return true;
	}
}