//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include <glm/glm.hpp>
#include "Component.h"
namespace Raven 
{
	enum class LightType
	{
		DirectionalLight = 0,
		SpotLight = 1,
		PointLight = 2
	};


	struct Light : public Component
	{
		// Construct.
		Light(const glm::vec3& direction = glm::vec3(0.0f), const glm::vec4& color = glm::vec4(1.0f), float intensity = 1.0f, const LightType& type = LightType::DirectionalLight, const glm::vec3& position = glm::vec3(), float radius = 1.0f, float angle = 0.0f);

		void OnImGui();

		// The light type matching the LightType enum.
		int32_t type;

		// The Light Color.
		glm::vec4 color;

		// The Light Power.
		float intensity;

		// TODO: Remove & Use Transform instead.
		glm::vec3 position;
		glm::vec3 direction;

		// The radius of influance, ignored for directional lights.
		float radius;

		// The inner angle of a spot light.
		float innerAngle;

		// The outer angle of a spot light.
		float outerAngle;

		// Distance from the view used to clip the light.
		float clipDistance;

		// serialize save.
		template<typename Archive>
		void save(Archive& archive) const
		{
			archive(cereal::base_class<Component>(this));

			archive(
				type,
				color,
				intensity,
				position,
				direction,
				radius,
				innerAngle,
				outerAngle,
				clipDistance
			);
		}

		// serialize load.
		template<typename Archive>
		void load(Archive& archive)
		{
			archive(cereal::base_class<Component>(this));

			archive(
				type,
				color,
				intensity,
				position,
				direction,
				radius,
				innerAngle,
				outerAngle,
				clipDistance
			);
		}

	};

};

