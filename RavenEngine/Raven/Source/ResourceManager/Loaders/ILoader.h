#pragma once

#include <string>

// the base loader class 

namespace Raven 
{
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
		ILoader(ELoaderType initType) 
			: type(initType) {}

		// delete copy constructors
		ILoader(const ILoader&) = delete;

		virtual ~ILoader() = default;

		// the type of loader
		auto GetType() const noexcept { return type; }

		virtual bool LoadAsset(const std::string& path);

	protected:
		ELoaderType type;
	};
}
