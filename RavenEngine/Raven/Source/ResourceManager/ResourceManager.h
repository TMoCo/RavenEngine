//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

/**
* The resource manager module takes care of providing the resources consumed by the 
* game engine, making sure that only a single copy of any data is ever created. It also
* handles the loading and unloading of resources from disc to RAM and from RAM to GPU.
* A resource registry, an unordered map stores pointers to resources that are loaded from files. 
* Loading of files is handled by Loader classes, stored in a vector. Each loader implements 
* a LoadAsset function that loads an asset of the type handled by the loader into the resource
* register.
*/

#pragma once

#include <string>		 // file path as a string
#include <unordered_map> // a register for unique resources
#include <memory>		 // pointer classes
#include <array>		 // array container

#include "Utilities/Core.h"

#include "IModule.h"
#include "ResourceManager/Resources/IResource.h"
#include "ResourceManager/Loaders/ILoader.h"


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
		TResource* GetResource(const std::string& path);

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
		// cast the resource pointer to the right resource type (basically same as dynamic cast...)
		template<class TResource>
		inline TResource* CastTo(IResource* resource) 
		{
			if (TResource::Type() != resource->GetType()) // check that we can cast to desired resource type
			{
				//throw std::runtime_error("Bad resource cast!");
				return nullptr;
			}
			return static_cast<TResource*>(resource);
		}

		// add and remove loader of a certain type
		template <class TLoader>
		void AddLoader(std::unique_ptr<TLoader> loader);

		template <class TLoader>
		void RemoveLoader(const TLoader* loader);

		// get a loader of type T
		template <class TLoader>
		TLoader* GetLoader();

		// for now the resource registers map an id to the resource in heap memory
		std::unordered_map<std::string, IResource*> resources;

		// an array containing the resource loaders used
		std::vector<std::unique_ptr<ILoader>> loaders;

		bool loadingResource = false;

		NOCOPYABLE(ResourceManager); // delete copy constructor and = operator
	};
}
