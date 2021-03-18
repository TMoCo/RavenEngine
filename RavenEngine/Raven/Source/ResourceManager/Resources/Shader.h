#pragma once

#include "ResourceManager/Resources/IResource.h"
#include "Render/RenderResource/RenderRscShader.h"

namespace Raven
{
	class Shader : public IResource
	{
	public:
		Shader() : IResource(EResourceType::RT_Material, true) {}
		// TODO: free data on destruction
		virtual ~Shader() = default;

		inline static EResourceType Type() noexcept { return EResourceType::RT_Shader; }

		RenderRscShader* renderResource = nullptr;

		NOCOPYABLE(Shader);
	};
}