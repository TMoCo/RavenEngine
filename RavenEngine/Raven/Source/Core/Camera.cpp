//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////
#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
namespace Raven 
{
	Camera::Camera(float fov, float near, float far, float aspect)
		: aspectRatio(aspect)
		, projectionDirty(true)
		, fov(fov)
		, near(near)
		, far(far)
		, orthographic(false)
		, scale(1.0f)
	{
	}

	Camera::Camera(float aspectRatio, float scale)
		: aspectRatio(aspectRatio)
		, scale(scale)
		, projectionDirty(true)
		, fov(0)
		, near(1.0)
		, far(3200000.0f)
		, orthographic(true)
	{

	}

	Camera::Camera(float pitch, float yaw, const glm::vec3& position, float fov, float near, float far, float aspect)
		: aspectRatio(aspect)
		, projectionDirty(true)
		, fov(fov)
		, near(near)
		, far(far)
		, orthographic(false)
		, scale(1.0f)
	{

	}

	const glm::mat4& Camera::GetProjectionMatrix()
	{
		if (projectionDirty)
		{
			UpdateProjectionMatrix();
			projectionDirty = false;
		}
		return projMatrix;
	}

	Ray Camera::GetScreenRay(float x, float y, const glm::mat4& viewMatrix, bool invertY /*= false*/) const
	{
		Ray ret;

		auto viewProjInverse = glm::inverse(projMatrix * viewMatrix);

		// The parameters range from 0.0 to 1.0. Expand to normalized device coordinates (-1.0 to 1.0)
		x = 2.0f * x - 1.0f;
		y = 2.0f * y - 1.0f;

		if (invertY)
			y *= -1.0f;
		glm::vec3 nearPlane(x, y, 0.0f);
		glm::vec3 farPlane(x, y, 1.0f);

		ret.origin = glm::vec3(viewProjInverse * glm::vec4(nearPlane, 0.0));
		ret.direction = glm::normalize(glm::vec3(viewProjInverse * glm::vec4(farPlane, 0.0)) - ret.origin);

		return ret;
	}

	void Camera::UpdateProjectionMatrix()
	{
		if (orthographic)
			projMatrix = glm::ortho(aspectRatio * -scale, aspectRatio * scale, -scale, scale, near, far);
		else
			projMatrix = glm::perspective(glm::radians(fov), aspectRatio, near, far);
	}

};
