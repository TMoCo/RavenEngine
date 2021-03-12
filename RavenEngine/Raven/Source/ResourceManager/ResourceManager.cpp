#include <iostream>

#include "ResourceManager.h"
#include "ResourceManager/Loaders/ILoader.h"
#include "ResourceManager/Loaders/ImageLoader.h"


namespace Raven {

	//
	// IModule methods
	//

	void ResourceManager::Initialize()
	{
		std::cout << "Initialised the resource manager" << '\n';
		AddLoader(std::make_unique<ImageLoader>(*this)); // create an image loader (resource manager as constructor argument
	}

	void ResourceManager::Destroy()
	{
		// TODO: Clean up resource manager
		std::cout << "Destroyed the resource manager" << std::endl;
	}


	//
	// Loader management
	//

	void ResourceManager::AddLoader(std::unique_ptr<ILoader> loader)
	{
		if (std::find(loaders.begin(), loaders.end(), loader) == loaders.end())
		{
			loaders.push_back(std::move(loader));
			std::cout << "Added a loader of type " << ILoader::TypeToString(loaders.rbegin()->get()->GetType()) << '\n';
		}
	}

	void ResourceManager::RemoveLoader(const ILoader* loader)
	{
		// loop over the loaders, finding the loader we want to remove
		const auto iter = std::find_if(loaders.begin(), loaders.end(), [loader](const auto& ownedLoader) noexcept { return loader == ownedLoader.get(); });
		// check the iterator isn't at the end, means we found a loader to remove
		if (iter != loaders.end())
		{
			loaders.erase(iter);
		}
	}

	//
	// Resource management
	//
	
	bool ResourceManager::LoadResource(const std::string& path, EResourceType type)
	{
		// TODO: Add loader logic
		// choose appropriate loader
		switch (type)
		{
		case Raven::EResourceType::RT_Image:

			break;
		default:
			break;
		}
		return true;
	}

	Texture2D* ResourceManager::GetResource(const std::string& id) 
	{
		auto resourceIter = textures.find(id); // a key/value pair iterator
		if (resourceIter == textures.end())
		{
			return nullptr;
		}
		else
		{
			return resourceIter->second;
		}
	}

	bool ResourceManager::HasResource(const std::string& id)
	{
		// true if a key exists
		return textures.count(id);
	}

	bool ResourceManager::AddResource(const std::string& id, Texture2D* resource)
	{
		switch (resource->GetType())
		{
		case EResourceType::RT_Image:
			textures.insert(std::make_pair(id, resource));
			return true;
		default:
			return false;
		}
	}
}