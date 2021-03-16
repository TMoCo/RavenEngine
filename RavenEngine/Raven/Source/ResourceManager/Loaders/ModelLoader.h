#pragma once

#include "ResourceManager/Loaders/ILoader.h"
#include "ResourceManager/ResourceManager.h"


namespace Raven 
{
	class ModelLoader : public ILoader
	{
	public:
		ModelLoader(ResourceManager& initResourceManager);

		bool LoadAsset(const std::string& path) override;
	};
}
