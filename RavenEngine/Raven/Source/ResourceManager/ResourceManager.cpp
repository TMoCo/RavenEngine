//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include "Utilities/Core.h"
#include "Utilities/StringUtils.h"

#include "ResourceManager.h"

#include "ResourceManager/Loaders/ILoader.h"


#include "ResourceManager/Resources/Texture2D.h"

// definition of the ResourceManager module

namespace Raven {

	//
	// IModule methods
	//

	void ResourceManager::Initialize()
	{
		AddLoader(std::make_unique<ImageLoader>(*this)); // create an image loader (resource manager as constructor argument
		AddLoader(std::make_unique<MeshLoader>(*this));
		AddLoader(std::make_unique<LayoutLoader>(*this));
	}

	void ResourceManager::Destroy()
	{
		FlushResourceRegister();
		loaders.clear(); // removes the loaders
	}

	//
	// Resource loading and management
	// 

	void ResourceManager::RemoveResource(const std::string& id)
	{
		resources.erase(id);
	}

	void ResourceManager::FlushResourceRegister()
	{
		// then clear the map
		resources.clear();
	}

	bool ResourceManager::HasResource(const std::string& id)
	{
		// true if a key exists
		return resources.count(id);
	}

	std::shared_ptr<IResource> ResourceManager::AddResource(const std::string& id, IResource* resource)
	{
		//resources.insert(std::make_pair(id, resource));
		std::shared_ptr<IResource> re(resource);
		resources.emplace(id, re);
		return re;
	}

	void ResourceManager::PrintResources()
	{
		for (auto& resource : resources)
		{
			// print resource path
			//LOGV(resource.first, resource.second.);
		}
	}

}