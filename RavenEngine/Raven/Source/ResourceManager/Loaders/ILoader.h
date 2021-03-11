#pragma once

#include <string>

//#include "ResourceManager/ResourceManager.h"

// the base loader class 

namespace Raven 
{

	class ResourceManager;

	// the type of loader (for each type of resource)
	enum class ELoaderType {
		LT_Image,
		LT_Mesh,
		LT_Material,
		LT_Scene,
		LT_Audio
	};

	// base loader class interface
	class ILoader 
	{
	public:
		// constructor always needs to know which loader type the loader is
		ILoader(ResourceManager& initResourceManager, ELoaderType initType)
			: resourceManager(&initResourceManager), type(initType) {}

		// delete copy constructors
		//ILoader(const ILoader&) = delete;
		//ILoader& operator=(const ILoader&) = delete;
		
		//ILoader(const ILoader&&) = delete;
		//ILoader& operator=(const ILoader&&) = delete;


		virtual ~ILoader() = default;

		// the type of loader
		inline auto GetType() const noexcept { return type; }

		virtual bool LoadAsset(const std::string& path);

	protected:
		const ELoaderType type;
		const ResourceManager* resourceManager;
	};
}
