#pragma once

#include <string> // file path as a string
#include <unordered_map> // a register for unique resources

#include "IModule.h"
#include "Loaders/ILoader.h"

namespace Raven
{
	class Resource; // some resource class

	class ResourceManager : public IModule
	{
	public:
		ResourceManager() = default;
	
		// inherited from IModule, must be over ridden
		virtual void Initialize() override;
		virtual void Destroy() override;

		// resources are obtained by passing their path as keys for the unordered map
		Resource* GetResource(const std::string& path);

	private:
		// add a loader of a certain type
		void AddLoader(std::unique_ptr<ILoader> loader);

		// for now the resource register maps an id to the resource in heap memory
		std::unordered_map<std::string, Resource*> resourceMap;

		// a vector containing the resource loaders used
		std::vector<std::unique_ptr<ILoader>> loaders;
	};
}
