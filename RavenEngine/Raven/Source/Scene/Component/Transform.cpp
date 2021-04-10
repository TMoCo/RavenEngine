//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#include "Transform.h"
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Raven
{
	glm::vec3 Transform::GetForwardDirection() const
	{
		return GetWorldOrientation() * FORWARD;
	}

	glm::vec3 Transform::GetRightDirection() const
	{
		return GetWorldOrientation() * RIGHT;
	}

	glm::vec3 Transform::GetUpDirection() const
	{
		return GetWorldOrientation() * UP;	
	}


	Transform::Transform()
	{
		localPosition = { 0.0f, 0.0f, 0.0f };
		localOrientation = {};
		localScale = { 1.0f, 1.0f, 1.0f };
		localMatrix = glm::mat4(1.f);
		worldMatrix = glm::mat4(1.f);
	}

	Transform::Transform(const glm::mat4& matrix)
	{
		glm::vec3 skew;
		glm::quat rotation;
		glm::vec4 perspective;
		glm::decompose(matrix, localScale, rotation, localPosition, skew, perspective);
		localOrientation = glm::eulerAngles(rotation);
		localMatrix = matrix;
		worldMatrix = matrix;
	}

	Transform::Transform(const glm::vec3& position)
	{
		localPosition = position;
		localOrientation = {};
		localScale = { 1.0f, 1.0f, 1.0f };
		localMatrix = glm::mat4(1.f);
		worldMatrix = glm::mat4(1.f);
		SetLocalPosition(position);
	}

	Transform::~Transform() = default;

	void Transform::SetWorldMatrix(const glm::mat4 & mat)
	{
		if (dirty)
			UpdateLocalMatrix();
		worldMatrix = mat * localMatrix;
	}

	void Transform::SetLocalTransform(const glm::mat4& localMat)
	{
		localMatrix = localMat;
		hasUpdated = true;
		ApplyTransform();//decompose 
	}

	void Transform::SetLocalPosition(const glm::vec3& localPos)
	{
		dirty = true;
		localPosition = localPos;
	}

	void Transform::SetLocalScale(const glm::vec3& scale)
	{
		dirty = true;
		localScale = scale;
	}

	//void Transform::SetLocalOrientation(const glm::quat& quat)
	void Transform::SetLocalOrientation(const glm::vec3& rotation)
	{
		dirty = true;
		localOrientation = rotation;
	}

	void Transform::UpdateLocalMatrix()
	{
		localMatrix = glm::translate(glm::mat4(1), localPosition);
		localMatrix *= glm::toMat4(glm::quat(localOrientation));
		localMatrix = glm::scale(localMatrix,localScale);
		dirty = false;
		hasUpdated = true;
	}

	void Transform::ApplyTransform()
	{
		glm::vec3 skew;
		glm::vec4 perspective;
		glm::quat rotation;
		glm::decompose(localMatrix, localScale, rotation, localPosition, skew, perspective);
		localOrientation = glm::eulerAngles(rotation);
	}

	glm::vec3 Transform::GetScaleFromMatrix(const glm::mat4& mat)
	{
		glm::vec3 skew;
		glm::vec3 localScale;
		glm::quat localOrientation;
		glm::vec3 localPosition;
		glm::vec4 perspective;
		glm::decompose(mat, localScale, localOrientation, localPosition, skew, perspective);
		return localScale;
	}

	Transform Transform::Identity()
	{
		return Transform(glm::mat4(1.0f));
	}

};

