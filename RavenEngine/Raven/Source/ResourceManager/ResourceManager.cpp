#include <iostream>

#include "ResourceManager.h"
#include "ResourceManager/Loaders/ILoader.h"
#include "ResourceManager/Loaders/ImageLoader.h"
#include "ResourceManager/Loaders/ModelLoader.h"


namespace Raven {

	const std::string testTexture("C:\\Users\\Tommy\\Pictures\\SML2_Wario_500.png"); // it's-a me, Wario!
	const std::string testMesh("C:\\Users\\Tommy\\Documents\\COMP4\\5822HighPerformanceGraphics\\A1\\HPGA1VulkanTutorial\\phongShading\\assets\\models\\mallard.obj"); // duck from cw1

	//
	// IModule methods
	//

	void ResourceManager::Initialize()
	{
		std::cout << "Initialised the resource manager" << '\n';
		AddLoader(std::make_unique<ImageLoader>(*this)); // create an image loader (resource manager as constructor argument
		AddLoader(std::make_unique<ModelLoader>(*this));

		std::cout << ILoader::TypeToString(GetLoader<ImageLoader>(ELoaderType::LT_Image)->GetType()) << '\n';
		LoadResource(testTexture, EResourceType::RT_Image);
		Texture2D* tex = GetResource<Texture2D>(testTexture);
		std::cout << IResource::TypeToString(tex->GetType()) << "\nheight: " << tex->height << " width: " << tex->width << std::endl;
		LoadResource(testMesh, EResourceType::RT_Mesh);
		std::cout << resources.size() << std::endl;
		Mesh* mesh = GetResource<Mesh>(testMesh);
		if (mesh)
			std::cout << IResource::TypeToString(mesh->GetType()) << "\nvertices: " << mesh->verts.size() << " indices: " << mesh->indices.size() << std::endl;

	}

	void ResourceManager::Destroy()
	{
		FlushResourceRegister();
		// TODO: Clean up resource manager
		std::cout << "Destroyed the resource manager" << std::endl;
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
			std::cout << "Added a loader of type " << ILoader::TypeToString(loaders.rbegin()->get()->GetType()) << '\n';
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
			loaders.erase(iter);
		}
	}

	template <class TLoader>
	TLoader* ResourceManager::GetLoader(ELoaderType type)
	{
		// 
		const auto iter = std::find_if(loaders.begin(), loaders.end(), [type](const auto& ownedLoader) noexcept { return ownedLoader->GetType() == type; });
		if (iter != loaders.end())
		{
			return static_cast<TLoader*>(iter->get());
		}
	}

	//
	// Resource loading and management
	// 

	bool ResourceManager::LoadResource(const std::string& path, EResourceType type)
	{
		// based on resource type, select approptiate loader and call loader's LoadAsset method
		switch (type)
		{
		case EResourceType::RT_Image:
			return GetLoader<ImageLoader>(ELoaderType::LT_Image)->LoadAsset(path); // adds a resource to the register
		case EResourceType::RT_Mesh:
			return GetLoader<ModelLoader>(ELoaderType::LT_Model)->LoadAsset(path);
		default:
			return false;
		}
	}

	void ResourceManager::RemoveResource(const std::string& id)
	{
		// free the resource
		delete resources[id];
		// then remove entry from register
		resources.erase(id);
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

	bool ResourceManager::AddResource(const std::string& id, IResource* resource)
	{
		resources.insert(std::make_pair(id, resource));
		return true;
	}

	//
	// Resource getting
	//

	template<class TResource>
	TResource* ResourceManager::GetResource(const std::string& id)
	{
		auto resourceIter = resources.find(id); // a key/value pair iterator
		if ((resourceIter == resources.end()))
		{
			return nullptr;
		}
		else
		{
			return dynamic_cast<TResource*>(resourceIter->second);
		}
	}
}