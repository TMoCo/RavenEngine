#pragma once

#include "ResourceManager/Resources/IResource.h"
#include "ResourceManager/Resources/Mesh.h"

namespace Raven
{
	class Model : public IResource
	{
	public:
		Model() : IResource(EResourceType::RT_Image) {}

		std::vector<Mesh*> meshes;

		NOCOPYABLE(Model);
	};
}

