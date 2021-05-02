#pragma once


#include "ResourceManager/Importers/Importer.h"



namespace Raven
{

	// ImageImporter:
	//    - import different types of images into texture resrouces.
	class ImageImporter : public IImporter
	{
	public:
		// Construct.
		ImageImporter();

		// Return the importer type.
		inline static EImporterType StaticGetType() noexcept { return IMP_Image; }

		// List all supported extensions by this importer.
		virtual void ListExtensions(std::vector<std::string>& outExt) override;

		// Import a new resrouce.
		virtual bool Import(const std::string& path, std::vector< Ptr<IResource> >& resources) override;

	private:
		// Import a 2D Image int a resrouce.
		IResource* ImportImage2D(const std::string& path);
		
	};
}