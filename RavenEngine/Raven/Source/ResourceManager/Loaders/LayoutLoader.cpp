#include <sstream>
#include <fstream>

#include <cereal/types/vector.hpp>
#include "cereal/archives/json.hpp"

#include "GUI/GUILayout.h"

#include "ResourceManager/Loaders/LayoutLoader.h"

#include "Utilities/Serialization.h"


namespace Raven
{
	bool LayoutLoader::LoadAsset(const std::string& path)
	{
		// Layout loading code
		std::ifstream in(path);
		if (!in.good())
		{
			LOGE("No saved scene file found {0}", path);
			in.close();
			return false;
		}
		std::string data;
		in.seekg(0, std::ios::end);
		auto len = in.tellg();
		in.seekg(0, std::ios::beg);
		data.resize(len);
		in.read(data.data(), len);
		in.close();

		std::istringstream istr;
		istr.str(data);
		cereal::JSONInputArchive input(istr);

		// create the new layout
		GUILayout* layout = new GUILayout();

		// set layout from loaded file
		input(*layout);

		layout->InitLayout();

		resourceManager->AddResource(path, layout);

		return true;
	}
}