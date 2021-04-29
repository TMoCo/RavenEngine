//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#include "SceneLoader.h"




namespace Raven
{


SceneLoader::SceneLoader()
{

}


IResource* SceneLoader::LoadResource(const ResourceHeaderInfo& info, RavenInputArchive& archive)
{
	RAVEN_ASSERT(0, "TODO RAVEN Implement MaterialLoader.");
	return nullptr;
}


void SceneLoader::SaveResource(RavenOutputArchive& archive, IResource* Resource)
{
	RAVEN_ASSERT(0, "TODO RAVEN Implement MaterialLoader.");
}


void SceneLoader::ListResourceTypes(std::vector<EResourceType>& outRscTypes)
{
	outRscTypes.push_back(RT_Scene);
}


} // End of namespace Raven.

