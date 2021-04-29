


#include "LayoutLoader.h"
#include "GUI/GUILayout.h"
#include "Utilities/Serialization.h"

#include <cereal/types/vector.hpp>
#include "cereal/archives/json.hpp"

#include <sstream>
#include <fstream>



namespace Raven
{


LayoutLoader::LayoutLoader()
{

}


LayoutLoader::~LayoutLoader()
{

}


IResource* LayoutLoader::LoadResource(const ResourceHeaderInfo& info, RavenInputArchive& archive)
{
	// create the new layout
	GUILayout* layout = new GUILayout();

	// set layout from loaded file
	archive.ArchiveLoad(*layout);

	layout->InitLayout();

	return layout;
}


void LayoutLoader::SaveResource(RavenOutputArchive& archive, IResource* Resource)
{
	RAVEN_ASSERT(0, "Not Supported.");
}


void LayoutLoader::ListResourceTypes(std::vector<EResourceType>& outRscTypes)
{
	outRscTypes.push_back(RT_GuiLayout);
}


}
