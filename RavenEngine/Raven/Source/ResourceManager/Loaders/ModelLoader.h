#pragma once

#include "ILoader.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tinyobjloader/tiny_obj_loader.h>


namespace Raven 
{
	class ModelLoader : public ILoader
	{
		ModelLoader(ResourceManager& initResourceManager);

		bool LoadAsset(const std::string& path) override;
	};
}
