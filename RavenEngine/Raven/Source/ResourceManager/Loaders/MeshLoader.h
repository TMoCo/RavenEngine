#pragma once

#include "ILoader.h"

namespace Raven 
{
	class MeshLoader : public ILoader
	{
		MeshLoader();

		bool LoadAsset(const std::string& path);
	};
}
