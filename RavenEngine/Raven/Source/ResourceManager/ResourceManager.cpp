#include "ResourceManager.h"
#include "Engine.h"


#include "Utilities/Core.h"
#include "Utilities/StringUtils.h"
#include "Utilities/Serialization.h"

// Importers...
#include "ResourceManager/Importers/ImageImporter.h"
#include "ResourceManager/Importers/OBJImporter.h"
#include "ResourceManager/Importers/FBXImporter.h"

// Loaders...
#include "ResourceManager/Loaders/ImageLoader.h"
#include "ResourceManager/Loaders/MeshLoader.h"
#include "ResourceManager/Loaders/LayoutLoader.h"
#include "ResourceManager/Loaders/AnimationLoader.h"
#include "ResourceManager/Loaders/SkinnedMeshLoader.h"


#include "miniz.h"

#include <fstream>
#include <iostream>
#include <filesystem>


// -- - --- -- - --- -- - --- -- - --- -- - --- -- - --- -- - --- -- - --- -- - --- -- - --- -- - --- -- - --




bool Raven::Compress(uint32_t size, const uint8_t* data, uint8_t*& comp, uint32_t& outCompSize)
{
	RAVEN_ASSERT(!comp, "We allocate our own memory.");

	int cmp_status;
	mz_ulong comp_size = compressBound(size);

	// Allocate buffers to hold compressed data.
	comp = (mz_uint8*)malloc((size_t)comp_size);

	if (!comp)
	{
		LOGE("Invalid memory allocation.");
		return false;
	}

	// Compress.
	cmp_status = compress(comp, &comp_size, (const unsigned char*)data, size);

	if (cmp_status != Z_OK)
	{
		LOGE("compress() failed!.");
		return false;
	}

	outCompSize = (uint32_t)comp_size;
	return true;
}


bool Raven::Uncompress(uint32_t compSize, const uint8_t* comp, uint8_t* data, uint32_t size)
{
	RAVEN_ASSERT(data && comp, "Invalid Input.");

	int cmp_status;
	mz_ulong uncomp_size = (mz_ulong)size;

	// Decompress.
	cmp_status = uncompress(data, &uncomp_size, (const unsigned char*)comp, (mz_ulong)compSize);

	if (cmp_status != Z_OK)
	{
		LOGE("uncompress() failed!.");
		return false;
	}

	RAVEN_ASSERT(size == uncomp_size, "Uncompress Size Mismatch.");
	return true;
}





// -- - --- -- - --- -- - --- -- - --- -- - --- -- - --- -- - --- -- - --- -- - --- -- - --- -- - --- -- - --





namespace Raven {





Ptr<IResource> ResourceRef::FindOrLoad()
{
	// Invalid Ref?
	if (type == INVALID_RSC_INDEX || path.empty())
		return nullptr;

	Ptr<IResource> resource = Engine::GetModule<ResourceManager>()->FindOrLoad(*this);
	rsc = resource.get();
	return resource;
}


Ptr<IResource> ResourceRef::GetResrouce()
{
	if (!IsValid())
		return nullptr;

	return Engine::GetModule<ResourceManager>()->GetResource(*this);
}


// -- - --- -- - --- -- - --- -- - --- -- - --- -- - --- -- - --- -- - --- -- - --- -- - --- -- - --- -- - --


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
	std::string cleanPath = CleanRscPath(path);
	uint32_t index = INVALID_RSC_INDEX;

	if (!resourcePathMap.count(cleanPath))
	{
		ResourceData data;
		data.info = info;
		data.type = info.GetType();
		data.path = path;
		data.cleanPath = cleanPath;
		data.rsc = newResource;

		index = static_cast<uint32_t>(resources.size());
		resources.push_back(data);

		// Map path to resource.
		resourcePathMap[cleanPath] = index;
	}
	else
	{
		// Get resource data from path.
		index = resourcePathMap[cleanPath];
	}


	// Is Loaded?
	if (newResource)
	{
		resources[index].rsc = newResource;
		resourceMap[newResource.get()] = index;
		newResource->resourceIndex = index;
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


const ResourceData* ResourcesRegistry::GetResource(uint32_t index) const
{
	if (index >= 0 && index < resources.size())
	{
		return &resources[index];
	}

	return nullptr;
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
	RegisterLoader<SkinnedMeshLoader>();
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


bool ResourceManager::LoadResource(const std::string& path, EResourceType type)
{
	ILoader* loader = GetLoader(type);
	return LoadResource(loader, path);
}


bool ResourceManager::LoadResource(ILoader* loader, const std::string& path)
{
	std::string absPath = StringUtils::GetCurrentWorkingDirectory() + "/" + path;

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

	// Set current load version.
	archive.version = info.GetVersion();

	// Load the resource.
	IResource* loadedRsc = loader->LoadResource(info, archive);
	loadedRsc->path = path;


	// Load Render Resrouces...
	if (loadedRsc->HasRenderResources() && !loadedRsc->IsOnGPU())
	{
		loadedRsc->LoadRenderResource();
	}

	// Add the loaded resource to the registry.
	registry.AddResource(path, info, Ptr<IResource>(loadedRsc));

	return true;
}


bool ResourceManager::AddResource(const std::string& path)
{
	std::string absPath = StringUtils::GetCurrentWorkingDirectory() + "/" + path;

	RavenInputArchive archive(path);

	// Failed to open archive?
	if (!archive.IsValid())
	{
		LOGW("Failed to add reference to a resrouce file {0}.", path.c_str());
		return false;
	}

	// Load Header.
	ResourceHeaderInfo info = ILoader::LoadHeader(archive);

	// Invalid Raven Resrouce?
	if (!info.IsValid())
	{
		LOGE("Failed to add resrouce. Resrouce is not RAVEN.");
		return false;
	}

	// Add the loaded resource to the registry.
	registry.AddResource(path, info, nullptr);

	return true;
}


Ptr<IResource> ResourceManager::FindOrLoad(const ResourceRef& ref)
{
	const ResourceData* rscData = registry.FindResource(ref.path);

	// Doesn't Exist?
	if (!rscData)
	{
		LOGW("No Resource found that matches the one in ResourceRef.");
		return nullptr;
	}

	// Type Mismatch?
	if (rscData->type != ref.type)
	{
		RAVEN_ASSERT(0, "Type Mismatch");
		return nullptr;
	}

	Ptr<IResource> resource;

	// Not Loaded?
	if (!rscData->rsc)
	{
		if (!LoadResource(rscData->path, rscData->type))
		{
			RAVEN_ASSERT(0, "Failed to load resource.");
			return nullptr;
		}
	}

	return rscData->rsc;
}


Ptr<IResource> ResourceManager::GetResource(const ResourceRef& ref)
{
	const ResourceData* rscData = registry.GetResource(ref.rsc->resourceIndex);

	// Not Found?
	if (!rscData)
		return nullptr;

	return rscData->rsc;
}



} // End of namespace Raven.
