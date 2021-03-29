//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include <map>

#include <glm/glm.hpp>

#include "ResourceManager/Resources/IResource.h"
#include "ResourceManager/Resources/Shader.h"
#include "ResourceManager/Resources/Texture2D.h"
#include "Render/RenderResource/RenderRscMaterial.h"

//
// A class for storing material properties and the shader the material applies to
//

namespace Raven
{
	class Material : public IResource
	{
	public:
		Material() : IResource(EResourceType::RT_Material, true) {}
		// TODO: free data on destruction
		virtual ~Material() = default;

		inline static EResourceType Type() noexcept { return EResourceType::RT_Material; }

		// add to the material resource
		inline void AddSampler(const std::string& name, Texture2D* sampler) { samplers.insert(std::make_pair(name, sampler)); }
		inline void AddScalar(const std::string& name, const float& scalar) { scalars.insert(std::make_pair(name, scalar)); }
		inline void AddColour(const std::string& name, const glm::vec4& colour) { colours.insert(std::make_pair(name, colour)); }

		std::map<std::string, Texture2D*> samplers;
		std::map<std::string, float>	  scalars;
		std::map<std::string, glm::vec4>  colours;

		// pointer to a shader, materials may share same shaders
		// Shader* shader; 
		// renderRscMaterial already has pointer to render shader resource
		RenderRscMaterial* renderResource = nullptr;

	};
}