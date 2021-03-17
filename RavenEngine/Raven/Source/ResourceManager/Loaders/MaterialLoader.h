#pragma once

#include "ILoader.h"

namespace Raven
{
	class MaterialLoader : ILoader
	{
	public:
		MaterialLoader(ResourceManager& initResourceManager);

		inline static auto Type() { return ELoaderType::LT_Material; }

		bool virtual LoadAsset(const std::string& path);

		virtual bool LoadOnGPU() override { return true; }
	};
}