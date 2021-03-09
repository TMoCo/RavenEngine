#include "MeshLoader.h"

namespace Raven
{
	// calls the inherited constructor
	MeshLoader::MeshLoader() : ILoader(ELoaderType::LT_Mesh) {}

	bool MeshLoader::LoadAsset(const std::string& path)
	{
		return true;
	}
}