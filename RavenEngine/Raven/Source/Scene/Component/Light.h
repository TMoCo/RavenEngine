//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include <glm/glm.hpp>
#include "Component.h"
namespace Raven 
{
	enum class  LightType
	{
		DirectionalLight = 0,
		SpotLight = 1,
		PointLight = 2
	};

	class Light : public Component
	{
	public:
		Light(const glm::vec3& direction = glm::vec3(0.0f), const glm::vec4& color = glm::vec4(1.0f), float intensity = 1.0f, const LightType& type = LightType::DirectionalLight, const glm::vec3& position = glm::vec3(), float radius = 1.0f, float angle = 0.0f);

		void OnImGui();

		glm::vec4 color;
		glm::vec3 position;
		glm::vec3 direction;
		float intensity;
		float radius;
		int32_t type;
		float angle;

		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(position, color, type, angle, direction,intensity, radius,entity);
		}
	};
};

