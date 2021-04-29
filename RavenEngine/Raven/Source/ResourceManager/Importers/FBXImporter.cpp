#include "FBXImporter.h"




namespace Raven {



FBXImporter::FBXImporter()
{
	type = IMP_FBX;
}


void FBXImporter::ListExtensions(std::vector<std::string>& outExt)
{
	outExt.push_back("fbx");
}


bool FBXImporter::Import(const std::string& path, std::vector<IResource*>& resources)
{
	RAVEN_ASSERT(0, "TODO Raven Implement FBXImporter.");
	return false;
}



} // End of namespace Raven
