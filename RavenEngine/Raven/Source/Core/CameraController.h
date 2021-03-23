//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include <glm/glm.hpp>
#include "Scene/Component/Transform.h"

namespace Raven
{
    class Camera;

	class CameraController
	{
	public:
        CameraController() = default;
		virtual ~CameraController() = default;

		virtual void HandleMouse(Transform& transform, float dt, float xpos, float ypos) {};
		virtual void HandleKeyboard(Transform& transform, float dt){};
		virtual void UpdateScroll(Transform& transform, float offset, float dt){};

		virtual void OnImGui() {};

		inline void SetMouseSensitivity(float value) { mouseSensitivity = value; }
		inline auto & GetVelocity() const { return velocity; }

	protected:
    
		glm::vec3 velocity;
		glm::vec2 rotateVelocity;
		glm::vec3 focalPoint;

		float zoomVelocity = 0.0f;
		float cameraSpeed = 0.0f;
		float distance = 0.0f;
		float zoom = 1.0f;

		glm::vec2 projectionOffset = glm::vec2(0.0f, 0.0f);
		glm::vec2 previousCurserPos = glm::vec2(0.0f, 0.0f);

		float mouseSensitivity = 0.1f;
		float zoomDampeningFactor = 0.00001f;
		float dampeningFactor = 0.00001f;
		float rotateDampeningFactor = 0.001f;
	};

}
