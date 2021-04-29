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

		// List all supported extensions by this importer.
		virtual void ListExtensions(std::vector<std::string>& outExt) override;

		// Import a new resrouce.
		virtual bool Import(const std::string& path, std::vector<IResource*>& resources) override;

	private:
		// Load a mesh resource from an obj file.
		IResource* LoadOBJ(const std::string& path);
	};
}