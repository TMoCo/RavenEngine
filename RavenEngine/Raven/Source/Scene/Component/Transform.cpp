//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#include "Transform.h"
#include "Scene/Entity/Entity.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"


#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>



namespace Raven
{


Transform::Transform()
	: position(0.0f)
	, scale(1.0f)
	, rotation(1.0f, 0.0f, 0.0f, 0.0f)
	, isWorldMatrixDiry(true)
	, isLocalMatrixCacheDiry(true)
{

}


Transform::Transform(const glm::mat4& matrix)
	: worldMatrix(1.0f)
	, isWorldMatrixDiry(true)
	, isLocalMatrixCacheDiry(true)
{
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(matrix, scale, rotation, position, skew, perspective);
	UpdateLocalMatrix();
}


Transform::Transform(const glm::vec3& inPosition)
	: scale(1.0f)
	, rotation(1.0f, 0.0f, 0.0f, 0.0f)
	, isWorldMatrixDiry(true)
	, isLocalMatrixCacheDiry(true)

{
	position = inPosition;
	UpdateLocalMatrix();
}


Transform::~Transform()
{

}


void Transform::UpdateLocalMatrix() const
{
	localMatrix_cache = glm::translate(glm::mat4(1), position);
	localMatrix_cache *= glm::toMat4(rotation);
	localMatrix_cache = glm::scale(localMatrix_cache, scale);
	isLocalMatrixCacheDiry = false;
}


void Transform::UpdateWorldMatrix() const
{
	// Is Entity Valid?
	if (entity != entt::null)
	{
		Entity ent = GetEntity();
		Entity parentEnt = ent.GetParent();

		if (parentEnt.Valid())
		{
			Transform* parentTransform = parentEnt.TryGetComponent<Transform>();

			if (parentTransform)
			{
				worldMatrix = parentTransform->GetWorldMatrix() * GetLocalMatrix();
			}
		}
		else
		{
			worldMatrix = GetLocalMatrix();
		}

		isWorldMatrixDiry = false;
		return;
	}
	else
	{
		worldMatrix = GetLocalMatrix();
		isWorldMatrixDiry = true;
	}
}


void Transform::UpdateChildrenWorld() const
{
	// Is Entity Valid?
	if (entity == entt::null)
		return;

	Entity ent = GetEntity();
	std::vector<Entity> children = ent.GetChildren();

	for (auto& child : children)
	{
		Transform* childTransform = child.TryGetComponent<Transform>();

		// Only Iterate on transform children.
		if (!childTransform)
			continue;

		childTransform->worldMatrix = GetWorldMatrix() * childTransform->GetLocalMatrix();
		childTransform->UpdateChildrenWorld();
	}
}


bool Transform::GetParentWorldMatrix(glm::mat4& outMtx) const
{
	// Is Entity Valid?
	if (entity != entt::null)
	{
		Entity ent = GetEntity();
		Entity parentEnt = ent.GetParent();

		if (parentEnt.Valid())
		{
			Transform* parentTransform = parentEnt.TryGetComponent<Transform>();

			if (parentTransform)
			{
				outMtx = parentTransform->GetWorldMatrix();
				return true;
			}
		}
	}

	return false;
}


void Transform::ResetTransform()
{
	position = glm::vec3(0.0f);
	scale = glm::vec3(1.0f);
	rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	isWorldMatrixDiry = true;
	isLocalMatrixCacheDiry = true;
}


glm::vec3 Transform::GetForwardDirection() const
{
	return GetWorldRotation() * FORWARD;
}


glm::vec3 Transform::GetRightDirection() const
{
	return GetWorldRotation() * RIGHT;
}


glm::vec3 Transform::GetUpDirection() const
{
	return GetWorldRotation() * UP;
}


void Transform::UpdateDirty()
{
	// Not Dirty?
	if (!isWorldMatrixDiry)
		return;

	UpdateWorldMatrix();
	UpdateChildrenWorld();
}


void Transform::Dirty()
{
	isWorldMatrixDiry = true;
	isLocalMatrixCacheDiry = true;
}


void Transform::SetPosition(const glm::vec3& v)
{
	position = v;

	Dirty();
	UpdateDirty();
}


void Transform::SetScale(const glm::vec3& v)
{
	scale = v;

	Dirty();
	UpdateDirty();
}


void Transform::SetRotation(const glm::quat& v)
{
	rotation = v;

	Dirty();
	UpdateDirty();
}


void Transform::SetRotationEuler(float pitch, float yaw, float roll)
{
	// Make quat that does this order v' = Yaw * Pitch * Roll * v;
	rotation = 
		  glm::angleAxis(yaw, UP)
		* glm::angleAxis(pitch, RIGHT)
		* glm::angleAxis(roll, FORWARD);

	Dirty();
	UpdateDirty();
}


void Transform::SetWorldPosition(const glm::vec3& v)
{
	glm::mat4 mtx;
	if ( GetParentWorldMatrix(mtx) )
	{
		mtx = glm::inverse(mtx);
		position = mtx * glm::vec4(v, 1.0f);
	}
	else
	{
		position = v;
	}



	Dirty();
	UpdateDirty();
}


void Transform::SetWorldScale(const glm::vec3& v)
{
	RAVEN_ASSERT(0, "TODO Implement SetWorldScale.");
}


void Transform::SetWorldRotation(const glm::quat& v)
{
	glm::mat4 mtx;
	if (GetParentWorldMatrix(mtx))
	{
		mtx = glm::inverse(mtx);
		mtx = mtx * glm::toMat4(v);

		rotation = glm::toQuat(mtx);
		rotation = glm::normalize(rotation);
	}
	else
	{
		rotation = v;
	}


	Dirty();
	UpdateDirty();
}


void Transform::SetWorldRotation(float pitch, float yaw, float roll)
{
	// Make quat that does this order v' = Yaw * Pitch * Roll * v;
	glm::quat q =
		  glm::angleAxis(yaw, UP)
		* glm::angleAxis(pitch, RIGHT)
		* glm::angleAxis(roll, FORWARD);

	SetWorldRotation(q);
}


void Transform::SetMatrixTransform(const glm::mat4& mtx)
{
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(mtx, scale, rotation, position, skew, perspective);

	Dirty();
	UpdateDirty();
}


void Transform::SetWorldMatrixTransform(const glm::mat4& mtx)
{
	glm::mat4 invWorldMtx;
	if (GetParentWorldMatrix(invWorldMtx))
	{
		invWorldMtx = glm::inverse(mtx);
		invWorldMtx = invWorldMtx * mtx;
		SetMatrixTransform(invWorldMtx);
	}
	else
	{
		SetMatrixTransform(mtx);
	}
}


glm::vec3 Transform::ExtractScale(const glm::mat4& mtx)
{
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::vec3 mtxScale;
	glm::quat mtxRot;
	glm::vec3 mtxPosition;
	glm::decompose(mtx, mtxScale, mtxRot, mtxPosition, skew, perspective);

	return mtxScale;
}



} // End of namespace Raven

