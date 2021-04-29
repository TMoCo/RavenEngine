//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////



#include "Utilities/StringUtils.h"

#include "ResourceManager/Loaders/MeshLoader.h"


namespace Raven {


MeshLoader::MeshLoader()
{

}


MeshLoader::~MeshLoader()
{

}


IResource* MeshLoader::LoadResource(const ResourceHeaderInfo& info, RavenInputArchive& archive)
{
	RAVEN_ASSERT(0, "TODO RAVEN Implement MeshLoader.");
	return nullptr;
}


void MeshLoader::SaveResource(RavenOutputArchive& archive, IResource* Resource)
{
	RAVEN_ASSERT(0, "TODO RAVEN Implement MeshLoader.");
}


void MeshLoader::ListResourceTypes(std::vector<EResourceType>& outRscTypes)
{
	outRscTypes.push_back(RT_Mesh);
}



} // End of namespace Raven 