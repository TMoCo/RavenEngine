#pragma once

#include <string> // file path as a string
#include <unordered_map> // a register for unique resources
#include <memory> // pointer classes

#include "IModule.h"
#include "ResourceManager/Resources/IResource.h"
#include "ResourceManager/Loaders/ILoader.h"

namespace Raven
{
	//class ILoader;

	class ResourceManager : public IModule
	{
	public:

		ResourceManager() = default;
		virtual ~ResourceManager() override {};
	

		// inherited from IModule, must be over-ridden
		virtual void Initialize() override;
		virtual void Destroy() override;

		static EModuleType GetModuleType() { return MT_ResourceManager; }

		// resources are obtained by passing their path as keys for the unordered map
		IResource* GetResource(const std::string& path);

		// returns true if the resource is already in the resource
		bool HasResource(const std::string& id);

	private:
		// add a loader of a certain type
		void AddLoader(std::unique_ptr<ILoader> loader);

		// for now the resource register maps an id to the resource in heap memory
		std::unordered_map<std::string, IResource*> resourceMap;

		// a vector containing the resource loaders used
		std::vector<std::unique_ptr<ILoader>> loaders;
	};
}
