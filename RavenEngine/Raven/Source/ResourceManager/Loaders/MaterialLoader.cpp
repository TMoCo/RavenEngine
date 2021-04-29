//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#include "MaterialLoader.h"




namespace Raven
{


MaterialLoader::MaterialLoader()
{

}


IResource* MaterialLoader::LoadResource(const ResourceHeaderInfo& info, RavenInputArchive& archive)
{
	RAVEN_ASSERT(0, "TODO RAVEN Implement MaterialLoader.");
	return nullptr;
}


void MaterialLoader::SaveResource(RavenOutputArchive& archive, IResource* Resource)
{
	RAVEN_ASSERT(0, "TODO RAVEN Implement MaterialLoader.");
}


void MaterialLoader::ListResourceTypes(std::vector<EResourceType>& outRscTypes)
{
	outRscTypes.push_back(RT_Material);
	outRscTypes.push_back(RT_MaterialShader);
}



} // End of namespace Raven.

