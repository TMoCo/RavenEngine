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
	
		// inherited from IModule, must be over-ridden
		virtual void Initialize() override;
		virtual void Destroy() override;

		static EModuleType GetModuleType() { return EModuleType::MT_ResourceManager; }

		//
		// Obtain resources:
		//

		// resources are obtained by passing their path as keys for the unordered map
		template<class TResource>
		TResource* GetResource(const std::string& path);

		//
		// Change the resource register
		//

		// tell manager to load a resource of a given type at a given path, need resource type to select loader
		template<class TResource>
		bool LoadResource(const std::string& path);

		// returns true if the resource is already in the resource register
		bool HasResource(const std::string& id);

		// add and remove resources
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
			if (TResource::IsATypeOf() != resource->GetType()) // check that we can cast to desired resource type
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

		// for now the resource registers map an id to the resource in heap memory (may wish to convert to unique_ptr later)
		// later there will be a few registers containing resource families 
		// eg: ResourceFamily Image -> Texture2D, Texture3D, CubeMap, Material (as a texture?...)
		std::unordered_map<std::string, IResource*> resources; // for now we just have Texture2D, so get that working first

		// an array containing the resource loaders used
		std::vector<std::unique_ptr<ILoader>> loaders;

		bool loadingResource = false;

		NOCOPYABLE(ResourceManager); // delete copy constructor and = operator
	};
}
