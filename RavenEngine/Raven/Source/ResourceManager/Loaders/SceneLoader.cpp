//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#include "SceneLoader.h"
#include "Scene/Scene.h"


#include "Utilities/Serialization.h"




namespace Raven
{


SceneLoader::SceneLoader()
{

}


IResource* SceneLoader::LoadResource(const ResourceHeaderInfo& info, RavenInputArchive& archive)
{
	RavenVersionGlobals::SCENE_ARCHIVE_VERSION = info.GetVersion();
	RAVEN_ASSERT(info.GetType() == EResourceType::RT_Scene, "Must be a scene.");

	Scene* scene = new Scene("LOAD_TMP_NAME");

	std::stringstream ss;
	{
		std::string str;
		archive.ArchiveLoad(str); // Load JSON...

		ss << str;
	}

	scene->LoadFromStream(ss);
	RavenVersionGlobals::SCENE_ARCHIVE_VERSION = RAVEN_VERSION;
	return scene;
}


void SceneLoader::SaveResource(RavenOutputArchive& archive, IResource* Resource)
{
	switch (Resource->GetType())
	{
	case EResourceType::RT_Scene:
	{
		Scene* scene = static_cast<Scene*>(Resource);

		std::string str;
		{
			std::stringstream ss;
			scene->SaveToStream(ss); // Save JSON...

			str = ss.str();
		}

		archive.ArchiveSave(str);
	}
	break;

	default:
		RAVEN_ASSERT(0, "Not Supported.");
		break;
	}

}


void SceneLoader::ListResourceTypes(std::vector<EResourceType>& outRscTypes)
{
	outRscTypes.push_back(RT_Scene);
}


} // End of namespace Raven.

