//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

/*
* The resource manager module takes care of providing the resources consumed by the 
* game engine, making sure that only a single copy of any data is ever created. It also
* handles the loading and unloading of resources from disc to RAM and from RAM to GPU.
* The resource registry, an unordered map, stores pointers to resources that are loaded from files. 
* The resources are stored in a key / value pair where the key is the string of the resource's path.
* The loading of files is handled by Loader classes, stored in a vector. Each loader implements 
* a LoadAsset function that loads an asset of the type handled by the loader into the resource
* register.
*/

#pragma once

#include <string>		 // file path as a string
#include <unordered_map> // a register for unique resources
#include <memory>		 // pointer classes
#include <array>		 // array container
#include <vector>

#include "Utilities/Core.h"

#include "IModule.h"
#include "ResourceManager/Resources/IResource.h"
#include "ResourceManager/Loaders/ILoader.h"
#include "ResourceManager/Loaders/ImageLoader.h"
#include "ResourceManager/Loaders/MeshLoader.h"

namespace Raven
{
	class ResourceManager : public IModule
	{
		friend class ILoader; // ILoader is a friend of the resource manager, it can add to the resource register on load

	public:
		ResourceManager() = default;
		virtual ~ResourceManager() override {};
	
		// inherited from IModule, must be overridden
		virtual void Initialize() override;
		virtual void Destroy() override;

		static EModuleType GetModuleType() { return EModuleType::MT_ResourceManager; }

		//
		// Obtain resources:
		//

		// resources are obtained by passing their path as keys for the unordered map
		template<class TResource>
		std::shared_ptr<TResource> GetResource(const std::string& path);
		template<class TResource>
		void GetResource(const std::string& path, std::vector<std::shared_ptr<TResource>>& outVec);
		template<class TResource>
		std::vector<std::shared_ptr<TResource>> GetResources(const std::string& path);

		// returns true if the resource is already in the resource register
		bool HasResource(const std::string& id);

		//
		// Change the resource register:
		//

		// tell manager to load a resource of a given type at a given path, need to specify resource type to select appropriate loader
		template<class TResource>
		bool LoadResource(const std::string& path);

		void AddResource(const std::string& id, IResource* resource);

		void RemoveResource(const std::string& id);

		// loops over the register entries and deletes the values, then removes the entries
		void FlushResourceRegister();
		
		// may be needed later in threaded application?
		inline bool IsLoadingScene() const { return loadingResource; }

		inline void SetLoading(bool loading) { loadingResource = loading; }


	private:
		// add and remove loader of a certain type
		template <class TLoader>
		void AddLoader(std::unique_ptr<TLoader> loader);
		template <class TLoader>
		void RemoveLoader(const TLoader* loader);

		void PrintResources();

		// get a loader of type T
		template <class TLoader>
		TLoader* GetLoader();
				
		// one to many 
		std::unordered_multimap<std::string, std::shared_ptr<IResource>> resources;

		// an array containing the resource loaders used
		std::vector<std::unique_ptr<ILoader>> loaders;

		bool loadingResource = false;

		NOCOPYABLE(ResourceManager); // delete copy constructor and = operator
	};

	// template method should be implemented in *.h files rather than .*cpp files
	// if implemented in cpp files, the files could not complie 
	template<class TResource>
	bool ResourceManager::LoadResource(const std::string& path)
	{
		// based on resource type, select approptiate loader and call loader's LoadAsset method
		switch (TResource::Type())
		{
		case EResourceType::RT_Image:
			return GetLoader<ImageLoader>()->LoadAsset(path); // adds a resource to the register
		case EResourceType::RT_Mesh:
			return GetLoader<MeshLoader>()->LoadAsset(path);
		default:
			return false;
		}
	}

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

	template<class TResource>
	std::vector<std::shared_ptr<TResource>> ResourceManager::GetResources(const std::string& path)
	{
		std::vector<std::shared_ptr<IResource>> res;
		GetResource(path, res);
		return res;
	}

	template<class TResource>
	void ResourceManager::GetResource(const std::string& path, std::vector<std::shared_ptr<TResource>>& out)
	{
		auto pr = resources.equal_range(path);
		if (pr.first != resources.end())
		{
			for (auto iter = pr.first; iter != pr.second; ++iter)
			{
				out.emplace_back(std::static_pointer_cast<TResource>(iter->second));
			}
		}
	}

	template<class TResource>
	std::shared_ptr<TResource> ResourceManager::GetResource(const std::string& path)
	{
		auto resourceIter = resources.find(path); // a key/value pair iterator
		// check IResource pointer exists 
		if (resourceIter == resources.end())
		{
			return nullptr;
		}
		else
		{
			return std::static_pointer_cast<TResource>(resourceIter->second);
		}
	}
}
