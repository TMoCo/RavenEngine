//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#include "AnimationLoader.h"




namespace Raven
{


AnimationLoader::AnimationLoader()
{

}


IResource* AnimationLoader::LoadResource(const ResourceHeaderInfo& info, RavenInputArchive& archive)
{
	RAVEN_ASSERT(0, "TODO RAVEN Implement MaterialLoader.");
	return nullptr;
}


void AnimationLoader::SaveResource(RavenOutputArchive& archive, IResource* Resource)
{
	//RAVEN_ASSERT(0, "TODO RAVEN Implement MaterialLoader.");
}


void AnimationLoader::ListResourceTypes(std::vector<EResourceType>& outRscTypes)
{
	outRscTypes.push_back(RT_Animation);
}



} // End of namespace Raven.

