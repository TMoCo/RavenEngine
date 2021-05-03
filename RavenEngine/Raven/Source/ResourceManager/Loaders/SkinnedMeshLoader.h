
#pragma once



#include "ILoader.h"




namespace Raven
{
	// SkinnedMeshLoader:
	//    - loader for skinned mesh & skeleton.
	//
	class SkinnedMeshLoader : public ILoader
	{
	public:
		// Construct.
		SkinnedMeshLoader();

		// Destrcut.
		virtual ~SkinnedMeshLoader();

		// Loader Type.
		inline static ELoaderType Type() { return ELoaderType::LT_SkinnedMesh; }

		// Load Resource from archive.
		virtual IResource* LoadResource(const ResourceHeaderInfo& info, RavenInputArchive& archive) override;

		// Save Resource into archive.
		virtual void SaveResource(RavenOutputArchive& archive, IResource* Resource) override;

		// List all resources that supported by this loader.
		virtual void ListResourceTypes(std::vector<EResourceType>& outRscTypes) override;

	};
}