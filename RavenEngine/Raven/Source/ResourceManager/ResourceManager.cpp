#include <iostream>

#include "ResourceManager.h"
#include "ResourceManager/Loaders/ILoader.h"
#include "ResourceManager/Loaders/ImageLoader.h"
#include "ResourceManager/Loaders/ModelLoader.h"

#include "ResourceManager/Resources/Model.h"
#include "ResourceManager/Resources/Texture2D.h"
#include "ResourceManager/Resources/Terrain.h"


namespace Raven {

	//
	// IModule methods
	//
	std::string t("C:\\Users\\Tommy\\Pictures\\SML2_Wario_500.png");
	std::string m("C:\\Users\\Tommy\\Documents\\COMP4\\5530MGroupProject\\Assets\\sockets.obj");

	void ResourceManager::Initialize()
	{
		LOGV("Initialised the resource manager");
		AddLoader(std::make_unique<ImageLoader>(*this)); // create an image loader (resource manager as constructor argument
		AddLoader(std::make_unique<ModelLoader>(*this));

		if (LoadResource<Texture2D>(t))
			LOGV("Loaded texture");

		Texture2D* tex = GetResource<Texture2D>(t);
		if (tex)
		{
			LOGV("Got resource");
		}

		if (LoadResource<Model>(m))
			LOGV("Loaded model");

		Model* mdl = GetResource<Model>(m);
		if (mdl)
		{
			LOGV("Got resource with " + std::to_string(mdl->GetMeshes()->size()) + " meshes");
		}
	}

	void ResourceManager::Destroy()
	{
		// TODO: Clean up resource manager
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
		LOGV("Type1: " + IResource::TypeToString(TResource::Type()));
		LOGV("Type2: " + IResource::TypeToString(resourceIter->second->GetType()));
		if ((resourceIter == resources.end()) || ((TResource::Type() != resourceIter->second->GetType())) ) // check IResource pointer is of right type
		{
			return nullptr;
		}
		else
		{
			LOGV("good type, casting...");
			// we can safely static cast the pointer to the rigth resource type
			return static_cast<TResource*>(resourceIter->second);
		}
	}
}