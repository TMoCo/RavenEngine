#pragma once



#include "Utilities/Core.h"
#include "ResourceManager/Resources/IResource.h"





namespace Raven
{
	// Types of all the importers in the engine.
	enum EImporterType
	{
		// Invalid Importer Type.
		IMP_None = -1,

		// Import images into textures.
		IMP_Image,

		// Import obj files into meshes.
		IMP_OBJ,

		// Import from FBX file into meshes, skeleton, animations, etc..
		IMP_FBX,
	};




	// IImporter:
	//    - The base class for importers managed by the resrouce manager.
	//
	class IImporter
	{
	public:
		// Default Construct.
		IImporter()
		{

		}

		// Destruct.
		virtual ~IImporter()
		{

		}

		// Return the resrouce type.
		inline EImporterType GetType() const noexcept { return type; }

		// List all extensions supported by this importer.
		virtual void ListExtensions(std::vector<std::string>& outExt) = 0;

		// Import a new resrouce.
		virtual bool Import(const std::string& path, std::vector<IResource*>& resources) = 0;

	protected:
		// The type of importer.
		EImporterType type;

	};



}