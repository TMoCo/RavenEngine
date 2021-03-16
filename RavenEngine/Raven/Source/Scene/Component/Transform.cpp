//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#include "Transform.h"
#include <glm/gtx/matrix_decompose.hpp>


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
		glm::vec4 perspective;
		glm::decompose(matrix, localScale, localOrientation, localPosition, skew, perspective);
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

	void Transform::SetLocalOrientation(const glm::quat& quat)
	{
		dirty = true;
		localOrientation = quat;
	}

	void Transform::UpdateLocalMatrix()
	{
		localMatrix = glm::translate(glm::mat4(1), localPosition);
		localMatrix *= glm::mat4_cast(localOrientation);
		localMatrix = glm::scale(localMatrix,localScale);
		dirty = false;
		hasUpdated = true;
	}

	void Transform::ApplyTransform()
	{
		glm::vec3 skew;
		glm::vec4 perspective;
		glm::decompose(localMatrix, localScale, localOrientation, localPosition, skew, perspective);
	}
};

