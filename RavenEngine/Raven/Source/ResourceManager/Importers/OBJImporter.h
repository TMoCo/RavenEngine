#pragma once


#include "ResourceManager/Importers/Importer.h"



namespace Raven
{

	// OBJImporter:
	//    - import obj into a mesh resource.
	class OBJImporter : public IImporter
	{
	public:
		// Construct.
		OBJImporter();

		// Return the importer type.
		inline static EImporterType StaticGetType() noexcept { return IMP_OBJ; }

		// List all supported extensions by this importer.
		virtual void ListExtensions(std::vector<std::string>& outExt) override;

		// Import a new resrouce.
		virtual bool Import(const std::string& path, std::vector< Ptr<IResource> >& resources) override;

	private:
		// Load a mesh resource from an obj file.
		IResource* LoadOBJ(const std::string& path);
	};
}