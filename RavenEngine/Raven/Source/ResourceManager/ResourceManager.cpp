//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include "Utilities/Core.h"
#include "Utilities/StringUtils.h"

#include "ResourceManager.h"

#include "ResourceManager/Loaders/ILoader.h"
#include "ResourceManager/Loaders/ImageLoader.h"
#include "ResourceManager/Loaders/MeshLoader.h"

#include "ResourceManager/Resources/Model.h"
#include "ResourceManager/Resources/Texture2D.h"
#include "ResourceManager/Resources/Terrain.h"

// definition of the ResourceManager module

namespace Raven {

	//
	// IModule methods
	//

	void ResourceManager::Initialize()
	{
		LOGV("Initialised the resource manager");
		AddLoader(std::make_unique<ImageLoader>(*this)); // create an image loader (resource manager as constructor argument
		AddLoader(std::make_unique<MeshLoader>(*this));


		std::string path = StringUtils::GetCurrentWorkingDirectory() + std::string("\\assets\\models\\mallard.obj");
		GetLoader<MeshLoader>()->LoadAsset(path);
		// test that all the models can be accessed
		int meshNum = 0;
		char buffer[100]; // up to 100 different meshes in a model
		std::string id = path + std::string(StringUtils::IntToString(meshNum++, buffer, 10));
		while ( HasResource(id) )
		{
			LOGV(id);
			LOGV(GetResource<Mesh>(id)->indices.size());
			id = path + std::string(StringUtils::IntToString(meshNum++, buffer, 10));
		}
	}

	void ResourceManager::Destroy()
	{
		FlushResourceRegister();
		loaders.clear(); // removes the loaders
		LOGV("Destroyed the resource manager");
	}

	//
	// Loader management
	//

	template <class TLoader>
	void ResourceManager::AddLoader(std::unique_ptr<TLoader> loader)
	{
		if (std::find(loaders.begin(), loaders.end(), loader) == loaders.end())
		{
			loaders.push_back(std::move(loader));
			LOGV("Added a loader of type " + ILoader::TypeToString(loaders.rbegin()->get()->GetType()));
		}
	}

	template <class TLoader>
	void ResourceManager::RemoveLoader(const TLoader* loader)
	{
		// loop over the loaders, finding the loader we want to remove
		const auto iter = std::find_if(loaders.begin(), loaders.end(), [loader](const auto& ownedLoader) noexcept { return loader == ownedLoader.get(); });
		// check the iterator isn't at the end, means we found a loader to remove
		if (iter != loaders.end())
		{
			loaders.erase(iter); // deletes the unique pointer, which deletes its owned object
		}
	}

	template <class TLoader>
	TLoader* ResourceManager::GetLoader()
	{
		// loop over loaders, finding the type of loader requested
		ELoaderType type = TLoader::Type();
		const auto iter = std::find_if(loaders.begin(), loaders.end(), [type](const auto& ownedLoader) noexcept { return ownedLoader->GetType() == type; });
		if (iter == loaders.end())
		{
			return nullptr;
		}
		else
		{
			// cast to desired loader type
			return dynamic_cast<TLoader*>(iter->get());
		}
	}

	//
	// Resource loading and management
	// 

	template<class TResource>
	bool ResourceManager::LoadResource(const std::string& path)
	{
		// based on resource type, select approptiate loader and call loader's LoadAsset method
		switch (TResource::Type())
		{
		case EResourceType::RT_Image:
			return GetLoader<ImageLoader>()->LoadAsset(path); // adds a resource to the register
		case EResourceType::RT_Model:
			return GetLoader<ModelLoader>()->LoadAsset(path);
		default:
			return false;
		}
	}

	void ResourceManager::RemoveResource(const std::string& id)
	{
		if (HasResource(id))
		{
			// free the resource
			delete resources[id];
			// then remove entry from register
			resources.erase(id);
		}
	}

	void ResourceManager::FlushResourceRegister()
	{
		// explicitly delete all dynamic resources
		for (auto& resource : resources)
		{
			delete resource.second;
		}
		// then clear the map
		resources.clear();
	}

	bool ResourceManager::HasResource(const std::string& id)
	{
		// true if a key exists
		return resources.count(id);
	}

	void ResourceManager::AddResource(const std::string& id, IResource* resource)
	{
		resources.insert(std::make_pair(id, resource));
	}

	//
	// Resource getting
	//

	template<class TResource>
	TResource* ResourceManager::GetResource(const std::string& id)
	{
		auto resourceIter = resources.find(id); // a key/value pair iterator
		// check IResource pointer exists 
		if (resourceIter == resources.end())
		{
			return nullptr;
		}
		else
		{
			// if resource is right type, return valid pointer, else nullptr
			return CastTo<TResource>(resourceIter->second);
		}
	}
}