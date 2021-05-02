//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once



#include "ILoader.h"




namespace Raven
{
	// AnimationLoader:
	//    - 
	//
	class AnimationLoader : public ILoader
	{
	public:
		// Construct.
		AnimationLoader();

		// Loader Type.
		inline static ELoaderType Type() { return ELoaderType::LT_Animation; }

		// Load Resource from archive.
		virtual IResource* LoadResource(const ResourceHeaderInfo& info, RavenInputArchive& archive) override;

		// Save Resource into archive.
		virtual void SaveResource(RavenOutputArchive& archive, IResource* Resource) override;

		// List all resources that supported by this loader.
		virtual void ListResourceTypes(std::vector<EResourceType>& outRscTypes) override;
	};
}



