#include "SkinnedMeshLoader.h"
#include "Utilities/StringUtils.h"


#include "ResourceManager/Resources/SkinnedMesh.h"
#include "Animation/Skeleton.h"




namespace Raven {


SkinnedMeshLoader::SkinnedMeshLoader()
{

}


SkinnedMeshLoader::~SkinnedMeshLoader()
{

}


IResource* SkinnedMeshLoader::LoadResource(const ResourceHeaderInfo& info, RavenInputArchive& archive)
{
	switch (info.GetType())
	{
	case EResourceType::RT_SkinnedMesh:
	{
		SkinnedMesh* skinnedMesh = new SkinnedMesh();
		archive.ArchiveLoad(*skinnedMesh);
		return skinnedMesh;
	}
		break;

	case EResourceType::RT_Skeleton:
	{
		Skeleton* skeleton = new Skeleton();
		archive.ArchiveLoad(*skeleton);
		return skeleton;
	}
		break;

	default:
		RAVEN_ASSERT(0, "Not Supported.");
		break;
	}

	return nullptr;
}


void SkinnedMeshLoader::SaveResource(RavenOutputArchive& archive, IResource* Resource)
{
	switch (Resource->GetType())
	{
	case EResourceType::RT_SkinnedMesh:
	{
		SkinnedMesh* skinnedMesh = static_cast<SkinnedMesh*>(Resource);
		archive.ArchiveSave(*skinnedMesh);
	}
		break;

	case EResourceType::RT_Skeleton:
	{
		Skeleton* skeleton = static_cast<Skeleton*>(Resource);
		archive.ArchiveSave(*skeleton);
	}
		break;

	default:
		RAVEN_ASSERT(0, "Not Supported.");
		break;
	}
}


void SkinnedMeshLoader::ListResourceTypes(std::vector<EResourceType>& outRscTypes)
{
	outRscTypes.push_back(RT_SkinnedMesh);
	outRscTypes.push_back(RT_Skeleton);
}



} // End of namespace Raven 