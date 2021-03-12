#pragma once

#include <string>		 // file path as a string
#include <unordered_map> // a register for unique resources
#include <memory>		 // pointer classes

#include "Utilities/Core.h"

#include "IModule.h"
#include "ResourceManager/Resources/IResource.h"
#include "ResourceManager/Loaders/ILoader.h"
#include "ResourceManager/Resources/Texture2D.h"

namespace Raven
{
	class ResourceManager : public IModule
	{
		friend class ILoader; // ILoader is a friend of the resource manager, can add to the resource register on load

	public:
		ResourceManager() = default;
		virtual ~ResourceManager() override {};
	

		// inherited from IModule, must be over-ridden
		virtual void Initialize() override;
		virtual void Destroy() override;

		static EModuleType GetModuleType() { return MT_ResourceManager; }

		// resources are obtained by passing their path as keys for the unordered map
		Texture2D* GetResource(const std::string& path);

		// returns true if the resource is already in the resource register
		bool HasResource(const std::string& id);

		bool LoadResource(const std::string& path, EResourceType type); // load a resource of a certain type

	private:
		// add a loader of a certain type
		void AddLoader(std::unique_ptr<ILoader> loader);

		void RemoveLoader(const ILoader* loader);

		// add a resource to the resourceMap
		bool AddResource(const std::string& id, Texture2D* resource);

		// for now the resource registers map an id to the resource in heap memory (may wish to convert to unique_ptr later)
		// later there will be a few registers containing resource families 
		// eg: ResourceFamily Image -> Texture2D, Texture3D, CubeMap, Material (as a texture?...)
		std::unordered_map<std::string, Texture2D*> textures; // for now we just have Texture2D, so get that working first

		// a vector containing the resource loaders used
		std::vector<std::unique_ptr<ILoader>> loaders;

		NOCOPYABLE(ResourceManager); // delete copy constructor and = operator
	};
}
