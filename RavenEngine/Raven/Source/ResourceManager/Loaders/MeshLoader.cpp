#include "MeshLoader.h"
#include "Utilities/StringUtils.h"

#include "ResourceManager/Resources/Mesh.h"



namespace Raven {


MeshLoader::MeshLoader()
{

}


MeshLoader::~MeshLoader()
{

}


IResource* MeshLoader::LoadResource(const ResourceHeaderInfo& info, RavenInputArchive& archive)
{
	switch (info.GetType())
	{
	case EResourceType::RT_Mesh:
	{
		Mesh* mesh = new Mesh();
		archive.ArchiveLoad(*mesh);
		return mesh;
	}
		break;

	default:
		RAVEN_ASSERT(0, "Not Supported.");
		break;
	}

	return nullptr;
}


void MeshLoader::SaveResource(RavenOutputArchive& archive, IResource* Resource)
{
	switch (Resource->GetType())
	{
	case EResourceType::RT_Mesh:
	{
		Mesh* mesh = static_cast<Mesh*>(Resource);
		archive.ArchiveSave(*mesh);
	}
		break;


	default:
		RAVEN_ASSERT(0, "Not Supported.");
		break;
	}
}


void MeshLoader::ListResourceTypes(std::vector<EResourceType>& outRscTypes)
{
	outRscTypes.push_back(RT_Mesh);
}



} // End of namespace Raven 