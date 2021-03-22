
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
		, near(-10.0)
		, far(10.0f)
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

	void Camera::UpdateProjectionMatrix()
	{
		if (orthographic)
			projMatrix = glm::ortho(aspectRatio * -scale, aspectRatio * scale, -scale, scale, near, far);
		else
			projMatrix = glm::perspective(glm::radians(fov), aspectRatio, near, far);
	}

};
