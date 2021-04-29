#include "ResourceManager.h"


#include "Utilities/Core.h"
#include "Utilities/StringUtils.h"

// Importers...
#include "ResourceManager/Importers/ImageImporter.h"
#include "ResourceManager/Importers/OBJImporter.h"

// Loaders...
#include "ResourceManager/Loaders/ImageLoader.h"
#include "ResourceManager/Loaders/MeshLoader.h"
#include "ResourceManager/Loaders/LayoutLoader.h"


#include <fstream>
#include <iostream>





namespace Raven {




ResourceHeaderInfo ILoader::LoadHeader(RavenInputArchive& archive)
{
	ResourceHeaderInfo header;
	archive.ArchiveLoad(header);

	return header;
}


ResourceHeaderInfo ILoader::SaveHeader(RavenOutputArchive& archive, IResource* rsc)
{
	ResourceHeaderInfo header;
	header.type = rsc->GetType();
	header.version = RAVEN_VERSION;

	archive.ArchiveSave(header);

	return header;
}





// -- - --- -- - --- -- - --- -- - --- -- - --- -- - --- -- - --- -- - --- -- - --- -- - --- -- - --- -- - --




void ResourcesRegistry::AddResource(const std::string& path, const ResourceHeaderInfo& info, Ptr<IResource> newResource)
{
	ResourceData data;
	data.info = info;
	data.type = info.GetType();
	data.path = path;
	data.rsc = newResource;

	uint32_t index = static_cast<uint32_t>(resources.size() - 1);
	resources.push_back(data);

	// Map path to resource.
	resourcePathMap[path] = index;

	// Is Loaded?
	if (newResource)
	{
		resourceMap[newResource.get()] = index;
	}
}


void ResourcesRegistry::RemoveResource(const std::string& path)
{
	// Find resource.
	auto iter= resourcePathMap.find(path);

	// Not Found?
	if (iter == resourcePathMap.end())
		return;

	// Unreference.
	resources[iter->second].rsc.reset();
}


void ResourcesRegistry::Reset()
{
	// Remove all references to the resource, but leave mapping intact
	for (auto& data : resources)
	{
		data.rsc.reset();
	}

}


const ResourceData* ResourcesRegistry::FindResource(const std::string& path) const
{
	auto iter = resourcePathMap.find(path);

	// Not Found?
	if (iter == resourcePathMap.end())
	{
		return nullptr;
	}

	return &resources[iter->second];
}





// -- - --- -- - --- -- - --- -- - --- -- - --- -- - --- -- - --- -- - --- -- - --- -- - --- -- - --- -- - --




ResourceManager::ResourceManager()
{

}


ResourceManager::~ResourceManager()
{

}



void ResourceManager::Initialize()
{
	// Importers...
	RegisterImporter<ImageImporter>();
	RegisterImporter<OBJImporter>();

	// Loaders...
	RegisterLoader<ImageLoader>();
	RegisterLoader<MeshLoader>();
	RegisterLoader<LayoutLoader>();

}


void ResourceManager::Destroy()
{
	registry.Reset();
}


IImporter* ResourceManager::GetImporter(const std::string& ext)
{
	// Search for importer mapped to this ext.
	auto iter = importersExtMap.find(ext);

	if (iter == importersExtMap.end())
		return nullptr;

	return iter->second;
}


ILoader* ResourceManager::GetLoader(EResourceType rscType)
{
	auto iter = loadersRscMap.find(rscType);

	// Not Found?
	if (iter == loadersRscMap.end())
	{
		RAVEN_ASSERT(0, "Load type not found, invalid loader type.");
		return nullptr;
	}

	return iter->second;
}


bool ResourceManager::HasResource(const std::string& path)
{

}


bool ResourceManager::IsResourceLoaded(const std::string& path)
{

}


bool ResourceManager::Import(const std::string& file, std::string* optionalSaveDir)
{
	std::string ext = StringUtils::GetFileName(file);
	std::string name = StringUtils::GetExtension(file);

	// -- - -- - - - --- 
	// 1. Import.
	// 2. Save.
	// 3. Register.


	// Find importer that support this extension.
	IImporter* importer = GetImporter(ext);

	// File Not Supported?
	if (!importer)
	{
		LOGE("File format not supported, {0}", file.c_str());
		return false;
	}

	// -- - -- - - - --- 
	// Import...
	std::vector<IResource*> newResources;
	bool isImportSuccess = importer->Import(file, newResources);

	// Failed to Import?
	if (!isImportSuccess)
	{
		LOGE("Failed to import a file, {0}", file.c_str());
		return false;
	}


	// Load render data?
	for (auto newResource : newResources)
	{
		if (newResource->HasRenderResources() && !newResource->IsOnGPU())
		{
			newResource->LoadRenderResource();
		}

		// Default - Current Directory.
		std::string outputFile = "./" + newResource->GetName() + ".raven";

		// Override Save Directory?
		if (optionalSaveDir)
		{
			outputFile = *optionalSaveDir + newResource->GetName() + ".raven";
		}


		// -- - -- - - - --- 
		// Save...
		Ptr<IResource> rscPtr(newResource);

		if (!SaveNewResource(rscPtr, outputFile))
		{
			return false;
		}
	}


	return true;
}


bool ResourceManager::SaveNewResource(Ptr<IResource> newResource, const std::string& saveFile)
{
	RavenOutputArchive archive(saveFile);

	// Failed to open archive?
	if (!archive.IsValid())
	{
		LOGE("Failed to open archive at file {0}.", saveFile.c_str());
		return false;
	}

	// Save Header
	ResourceHeaderInfo info = ILoader::SaveHeader(archive, newResource.get());

	// Save...
	GetLoader(info.GetType())->SaveResource(archive, newResource.get());

	// Add the new resource to the registry.
	registry.AddResource(saveFile, info, newResource);

	return true;
}


bool ResourceManager::LoadResource(ILoader* loader, const std::string& path)
{

}



} // End of namespace Raven.
