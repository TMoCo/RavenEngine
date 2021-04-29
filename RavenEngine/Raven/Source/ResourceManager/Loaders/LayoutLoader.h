//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ResourceManager/Loaders/ILoader.h"
#include "ResourceManager/ResourceManager.h"




namespace Raven
{
	// LayoutLoader:
	//    -  loading gui layouts
	//
	class LayoutLoader : public ILoader
	{
	public:
		// Construct.
		LayoutLoader();

		// Destruct.
		virtual ~LayoutLoader();

		// Loader Type.
		inline static ELoaderType Type() { return ELoaderType::LT_GuiLayout; }

		// Load Resource from archive.
		virtual IResource* LoadResource(const ResourceHeaderInfo& info, RavenInputArchive& archive) override;

		// Save Resource into archive.
		virtual void SaveResource(RavenOutputArchive& archive, IResource* Resource) override;

		// List all resources that supported by this loader.
		virtual void ListResourceTypes(std::vector<EResourceType>& outRscTypes) override;
	};
}
