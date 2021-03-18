#pragma once

#include "ResourceManager/Loaders/ILoader.h"
#include "ResourceManager/ResourceManager.h"


namespace Raven 
{
	class ModelLoader : public ILoader
	{
	public:
		ModelLoader(ResourceManager& initResourceManager);

		inline static auto Type() { return ELoaderType::LT_Model; }

		bool virtual LoadAsset(const std::string& path) override;

		bool LoadOBJ(const std::string& path);

		virtual bool LoadOnGPU() override { return true; }
	};
}
