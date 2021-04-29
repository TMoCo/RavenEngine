#pragma once


#include "ResourceManager/Importers/Importer.h"



namespace Raven
{

	// FBXImporter:
	//    - import fbox into a mesh or skinned mesh resource.
	class FBXImporter : public IImporter
	{
	public:
		// Construct.
		FBXImporter();

		// List all supported extensions by this importer.
		virtual void ListExtensions(std::vector<std::string>& outExt) override;

		// Import a new resrouce.
		virtual bool Import(const std::string& path, std::vector<IResource*>& resources) override;

	};
}