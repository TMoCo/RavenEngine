#pragma once

#include "ResourceManager/Resources/IResource.h"
#include "ResourceManager/Resources/Shader.h"
#include "Render/RenderResource/RenderRscMaterial.h"

namespace Raven
{
	class Material : public IResource
	{
	public:
		Material() : IResource(EResourceType::RT_Material, true) {}
		// TODO: free data on destruction
		virtual ~Material() = default;

		inline static EResourceType Type() noexcept { return EResourceType::RT_Material; }

		// pointer to a shader
		Shader* shader;
	};
}