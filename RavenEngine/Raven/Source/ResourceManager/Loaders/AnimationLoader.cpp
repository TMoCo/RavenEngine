#include "AnimationLoader.h"



#include "Animation/Animation.h"




namespace Raven
{


AnimationLoader::AnimationLoader()
{

}


IResource* AnimationLoader::LoadResource(const ResourceHeaderInfo& info, RavenInputArchive& archive)
{
	switch (info.GetType())
	{
	case EResourceType::RT_AnimationClip:
	{
		AnimationClip* anime = new AnimationClip();
		archive.ArchiveLoad(*anime);
		return anime;
	}

	default:
		RAVEN_ASSERT(0, "Not Supported.");
		break;
	}

	return nullptr;
}


void AnimationLoader::SaveResource(RavenOutputArchive& archive, IResource* Resource)
{
	switch (Resource->GetType())
	{
	case EResourceType::RT_AnimationClip:
	{
		AnimationClip* anime = static_cast<AnimationClip*>(Resource);
		archive.ArchiveSave(*anime);
	}
		break;

	default:
		RAVEN_ASSERT(0, "Not Supported.");
		break;
	}
}


void AnimationLoader::ListResourceTypes(std::vector<EResourceType>& outRscTypes)
{
	outRscTypes.push_back(RT_AnimationClip);
}



} // End of namespace Raven.

