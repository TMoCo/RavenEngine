#include "ResourceManager.h"


#include "Utilities/Core.h"
#include "Utilities/StringUtils.h"

// Importers...
#include "ResourceManager/Importers/ImageImporter.h"
#include "ResourceManager/Importers/OBJImporter.h"
#include "ResourceManager/Importers/FBXImporter.h"

// Loaders...
#include "ResourceManager/Loaders/ImageLoader.h"
#include "ResourceManager/Loaders/MeshLoader.h"
#include "ResourceManager/Loaders/LayoutLoader.h"
#include "ResourceManager/Loaders/AnimationLoader.h"


#include <fstream>
#include <iostream>
#include <filesystem>




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
	data.cleanPath = CleanRscPath(path);
	data.rsc = newResource;

	uint32_t index = static_cast<uint32_t>(resources.size());
	resources.push_back(data);

	// Map path to resource.
	resourcePathMap[data.cleanPath] = index;

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
	// Clean the path before searching.
	std::string cleanPath = CleanRscPath(path);

	// Search...
	auto iter = resourcePathMap.find(cleanPath);

	// Not Found?
	if (iter == resourcePathMap.end())
	{
		return nullptr;
	}

	return &resources[iter->second];
}


std::string ResourcesRegistry::CleanRscPath(const std::string& path) const
{
	std::string cleanPath;

	if (path.empty())
		return cleanPath;

	if (path.size() > 2 && path[0] == '.' && (path[1] == '/' || path[1] == '\\'))
	{
		cleanPath = StringUtils::RemoveExtension(path.substr(2));
	}
	else if (path.size() > 1 && (path[1] == '/' || path[1] == '\\'))
	{
		cleanPath = StringUtils::RemoveExtension(path.substr(1));
	}
	else
	{
		cleanPath = StringUtils::RemoveExtension(path);
	}

	std::replace(cleanPath.begin(), cleanPath.end(), '\\', '/');

	return cleanPath;
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
	RegisterImporter<FBXImporter>();

	// Loaders...
	RegisterLoader<ImageLoader>();
	RegisterLoader<MeshLoader>();
	RegisterLoader<LayoutLoader>();
	RegisterLoader<AnimationLoader>();

	// scan the directory and populate the 

}


void ResourceManager::Destroy()
{
	registry.Reset();
}


void ResourceManager::ScanDirectory(const std::string& path)
{
	// construct a path from the input string
	auto p = std::filesystem::path(path);
	// use the path passed as argument to explore the directory (if it is)
	if (std::filesystem::is_directory(p))
	{
		for (const auto& entry : std::filesystem::recursive_directory_iterator(path))
		{
			// extract file extension 
			std::string ext = StringUtils::GetExtension(path);

			// raven extension, load the resource
			if (ext == std::string("raven"))
			{
				ResourceHeaderInfo info = ILoader::LoadHeader(RavenInputArchive(path));
				LoadResource(GetLoader(info.GetType()), path);
			}

			// if file extension on is supported (an importer exists), notify the registry
			if (GetImporter(ext))
			{
				registry.AddResource(path, ILoader::LoadHeader(RavenInputArchive(path)), nullptr);
			}

			// recursivivly check for subdirectories
			ScanDirectory(entry.path().string());
		}
	}
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
	RAVEN_ASSERT(0, "TODO Implement HasResource.");
	return false;
}


bool ResourceManager::IsResourceLoaded(const std::string& path)
{
	RAVEN_ASSERT(0, "TODO Implement HasResource.");
	return false;
}


bool ResourceManager::Import(const std::string& file, std::string optionalSaveDir)
{
	std::string name = StringUtils::GetFileNameWithoutExtension(file);
	std::string ext = StringUtils::GetExtension(file);

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
	std::vector< Ptr<IResource> > newResources;
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
		if (!optionalSaveDir.empty())
		{
			outputFile = optionalSaveDir + newResource->GetName() + ".raven";
		}


		// -- - -- - - - --- 
		// Save...
		if ( !SaveNewResource(newResource, outputFile) )
		{
			return false;
		}

	}


	return true;
}


bool ResourceManager::SaveNewResource(Ptr<IResource> newResource, const std::string& saveFile)
{
	std::string absSavePath = StringUtils::GetCurrentWorkingDirectory() + "/" + saveFile;
	RavenOutputArchive archive(absSavePath);

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
	newResource->path = saveFile;

	// Add the new resource to the registry.
	registry.AddResource(saveFile, info, newResource);

	return true;
}


bool ResourceManager::LoadResource(ILoader* loader, const std::string& path)
{
	RavenInputArchive archive(path);

	// Failed to open archive?
	if (!archive.IsValid())
	{
		LOGE("Failed to open archive at file {0}.", path.c_str());
		return false;
	}

	// Load Header.
	ResourceHeaderInfo info = ILoader::LoadHeader(archive);

	// Invalid Raven Resrouce?
	if (!info.IsValid())
	{
		return false;
	}

	// Load the resource.
	IResource* loadedRsc = loader->LoadResource(info, archive);
	loadedRsc->path = path;

	// Add the loaded resource to the registry.
	registry.AddResource(path, info, Ptr<IResource>(loadedRsc));
}



} // End of namespace Raven.
