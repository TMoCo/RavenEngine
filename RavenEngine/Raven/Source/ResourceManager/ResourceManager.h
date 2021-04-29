//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once



#include "IModule.h"

#include "Resources/IResource.h"
#include "Importers/Importer.h"
#include "Loaders/ILoader.h"



#include "Utilities/Core.h"


#include <string>
#include <unordered_map>
#include <memory>	
#include <array>
#include <vector>





namespace Raven
{
	// Hold Data for a single Resource.
	class ResourceData
	{
	public:
		// The type of the Resource.
		EResourceType type;

		// The Relative Resource Path.
		std::string path;

		// The header data that contains information about the Resource.
		ResourceHeaderInfo info;

		// The Resource.
		Ptr<IResource> rsc;
	};



	// ResourcesRegistry:
	//     - Handle reference and map all the Resources in the Resource manager.
	class ResourcesRegistry
	{
	private:
		// List of all the resroucs that exist, loaded or not.
		std::vector<ResourceData> resources;

		// Map a relative path to their Resource data.
		std::unordered_map<std::string, uint32_t> resourcePathMap;

		// Map a resource pointer to their Resource data.
		std::unordered_map<IResource*, uint32_t> resourceMap;

	public:
		// Add new loaded Resource.
		void AddResource(const std::string& path, const ResourceHeaderInfo& info, Ptr<IResource> newResource);

		// Remove loaded Resource, this will only remove the loaded Resource not its mapping.
		void RemoveResource(const std::string& path);

		// Clear all loaded Resources, this will leave the mapping.
		void Reset();

		// Find a resrouce from path.
		const ResourceData* FindResource(const std::string& path) const;
	};



	// ResourceRef:
	//    - Used to reference a resource for archiving operations.
	//
	template< class ResourceType,
		std::enable_if_t< std::is_base_of<IResource, ResourceType>::value, bool > = true >
	class ResourceRef
	{
		// Friend...
		friend class ResourceManager;

	private:
		// The Resource relative path.
		std::string path;

		// The type of the Resource.
		EResourceType type;

		// The Resource.
		Ptr<IResource> rsc;

	public:
		// Return the relative path to the Resource.
		inline const std::string& GetPath() const { return path; }

		// Return a raw pointer to the Resource.
		inline Ptr<IResource> Get() const { return rsc; }

		// Return true if the Resource is loaded and valid.
		inline bool IsValid() const { return rsc != nullptr; }

		// Find or load the Resource. 
		bool FindOrLoad();
	};





	// Resource Manager:
	//   - The resource manager module takes care of providing the resources consumed by the
	//     game engine, making sure that only a single copy of any data is ever created/loaded.
	//
	//   - It handles the importing, loading and unloading the resources.
	//
	class ResourceManager : public IModule
	{
		// Delete copy constructor and assignment operator
		NOCOPYABLE(ResourceManager);

		// ILoader is a friend of the resource manager, it can add to the resource register on load
		friend class ILoader; 

	public:
		// Construct.
		ResourceManager();

		// Destruct.
		virtual ~ResourceManager();
	
		// inherited from IModule, must be overridden
		virtual void Initialize() override;
		virtual void Destroy() override;

		// Return Module Type.
		static EModuleType GetModuleType() { return EModuleType::MT_ResourceManager; }


		// --- -- - --- -- - --- -- - --- -- - --- -- - --- 
		//                 Obtain resources
		// --- -- - --- -- - --- -- - --- -- - --- -- - ---

		// Find a Resource and 
		template<class TResource,
			std::enable_if_t< std::is_base_of<IResource, TResource>::value, bool > = true >
		Ptr<TResource> GetResource(const std::string& path);

		// Return true if the Resource exist in the Resource registry whether loaded or not.
		bool HasResource(const std::string& path);

		// Return true if the Resource exist in the registry and loaded.
		bool IsResourceLoaded(const std::string& path);


		// --- -- - --- -- - --- -- - --- -- - --- -- - --- 
		//                 Save/Import resources
		// --- -- - --- -- - --- -- - --- -- - --- -- - ---

		// Import new Resource from other file formats using our importers.
		// @param file: the file we want to import could be an image or .obj file etc...
		// @param optionalSaveDir: if not null will save the Resource to this directory.
		// @return true if successfully imported.
		bool Import(const std::string& file, std::string* optionalSaveDir = nullptr);

		// Save a new resource and add it to be the Resource registry.
		bool SaveNewResource(Ptr<IResource> newResource, const std::string& saveFile);

		// Save existing Resource.
		bool SaveResource(Ptr<IResource> rsc);


	private:
		// --- -- - --- -- - --- -- - --- -- - --- -- - --- 
		//             Importers & Loaders 
		// --- -- - --- -- - --- -- - --- -- - --- -- - --- 

		// Load a Resource at that path.
		template<class TResource>
		bool LoadResource(const std::string& path);

		// Return the loader of type TLoader
		template <class TLoader>
		TLoader* GetLoader();

		// Return the loader that can load this resource type.
		ILoader* GetLoader(EResourceType rscType);

		// Return the importer that can load a specific extension
		IImporter* GetImporter(const std::string& ext);

		// Load a resrouce using specific loader.
		bool LoadResource(ILoader* loader, const std::string& path);


	private:
		// Create & Register a new loader to the Resource manager.
		template<class TLoader,
			std::enable_if_t< std::is_base_of<ILoader, TLoader>::value, bool > = true >
		void RegisterLoader();

		// Create & Register a new importer to the Resource manager.
		template<class TImporter,
			std::enable_if_t< std::is_base_of<IImporter, TImporter>::value, bool > = true >
		void RegisterImporter();

		// List of all Resource loaders in the engine.
		std::vector< std::unique_ptr<ILoader> > loaders;

		// Map each loader to their type.
		std::unordered_map<ELoaderType, ILoader*> loadersMap;

		// Map each loader to the type of resources they can load.
		std::unordered_map<EResourceType, ILoader*> loadersRscMap;

		// List of all importers in the engine.
		std::vector< std::unique_ptr<IImporter> > importers;

		// Map each importer to their supported extensions.
		std::unordered_map<std::string, IImporter*> importersExtMap;

		// The Resource registry, use for mapping all Resources that can be loaded or already laoded.
		ResourcesRegistry registry;
	};







	// --- -- - --- -- - --- -- - --- -- - --- -- - --- -- - --- -- - ---   
	//		
	//                 Resource Manager Implementations
	//
	// --- -- - --- -- - --- -- - --- -- - --- -- - --- -- - --- -- - --- 




	template<class TResource>
	bool ResourceManager::LoadResource(const std::string& path)
	{
		ILoader* loader = GetLoader(TResource::Type());

		return LoadResource(loader, path);
	}


	template <class TLoader>
	TLoader* ResourceManager::GetLoader()
	{
		auto iter = loadersMap.find(TLoader::Type());

		// Not Found?
		if (iter == loadersMap.end())
		{
			RAVEN_ASSERT(0, "Load type not found, invalid loader type.");
			return nullptr;
		}

		return static_cast<TLoader*>(iter->second);
	}


	template<class TResource,
		std::enable_if_t< std::is_base_of<IResource, TResource>::value, bool > >
	Ptr<TResource> ResourceManager::GetResource(const std::string& path)
	{
		const ResourceData* rscData = registry.FindResource(path);

		// Doesn't Exist?
		if (!rscData)
		{
			LOGW("Error in GetResource, Resource Does not exit {0}", path.c_str());
			return nullptr;
		}

		// Type Mismatch?
		if (rscData->type != TResource::Type())
		{
			RAVEN_ASSERT(0, "Invalid Resrouce Type.");
			return nullptr;
		}

		// Not Loaded?
		if (!rscData->rsc)
		{
			if ( !LoadResource<TResource>(path) )
			{
				RAVEN_ASSERT(0, "Failed to load resource.");
				return nullptr;
			}
		}

		return std::static_pointer_cast<TResource, IResource>(rscData->rsc);
	}


	template<class TLoader,
		std::enable_if_t< std::is_base_of<ILoader, TLoader>::value, bool > >
		void ResourceManager::RegisterLoader()
	{
		ILoader* newLoader = new TLoader();
		loaders.push_back( std::unique_ptr<ILoader>(newLoader) );
		loadersMap.insert(std::make_pair(TLoader::Type(), newLoader));

		std::vector<EResourceType> rscTypes;
		newLoader->ListResourceTypes(rscTypes);

		// Map each file extenions to their importer
		for (const auto& rt : rscTypes)
		{
			loadersRscMap.insert(std::make_pair(rt, newLoader));
		}
	}


	template<class TImporter,
		std::enable_if_t< std::is_base_of<IImporter, TImporter>::value, bool > >
		void ResourceManager::RegisterImporter()
	{
		IImporter* newImporter = new TImporter();
		importers.push_back( std::unique_ptr<IImporter>(newImporter) );

		std::vector<std::string> impExt;
		newImporter->ListExtensions(impExt);

		// Map each file extenions to their importer
		for (const auto& ext : impExt)
		{
			importersExtMap.insert( std::make_pair(ext, newImporter) );
		}

	}


}
