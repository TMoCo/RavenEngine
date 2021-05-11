//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#include "MaterialLoader.h"

#include "ResourceManager/Resources/MaterialShader.h"
#include "ResourceManager/Resources/Material.h"






namespace Raven
{


MaterialLoader::MaterialLoader()
{

}


IResource* MaterialLoader::LoadResource(const ResourceHeaderInfo& info, RavenInputArchive& archive)
{
	switch (info.GetType())
	{
	case EResourceType::RT_Material:
	{
		Material* mat = new Material();
		archive.ArchiveLoad(*mat);
		return mat;
	}
		break;

	case EResourceType::RT_MaterialShader:
	{
		MaterialShader* shader = new MaterialShader();
		archive.ArchiveLoad(*shader);
		return shader;
	}
		break;

	default:
		RAVEN_ASSERT(0, "Not Supported.");
		break;
	}

	return nullptr;
}


void MaterialLoader::SaveResource(RavenOutputArchive& archive, IResource* Resource)
{
	switch (Resource->GetType())
	{
	case EResourceType::RT_Material:
	{
		Material* mat = static_cast<Material*>(Resource);
		archive.ArchiveSave(*mat);
	}
		break;

	case EResourceType::RT_MaterialShader:
	{
		MaterialShader* shader = static_cast<MaterialShader*>(Resource);
		archive.ArchiveSave(*shader);
	}
		break;

	default:
		RAVEN_ASSERT(0, "Not Supported.");
		break;
	}
}


void MaterialLoader::ListResourceTypes(std::vector<EResourceType>& outRscTypes)
{
	outRscTypes.push_back(RT_Material);
	outRscTypes.push_back(RT_MaterialShader);
}



} // End of namespace Raven.

