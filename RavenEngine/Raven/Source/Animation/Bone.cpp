#include "Bone.h"



#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>




namespace Raven {



Bone::Bone()
	: parent(nullptr)
	, id(-1)
	, parentIdx(-1)
	, isTransformDirty(false)
{

}


void Bone::ResetPose()
{
	position = restPosition;
	rotation = restRotation;
}


void Bone::SetRestPose(const glm::mat4& mtx)
{
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::vec3 scale;
	glm::quat qrot;
	glm::decompose(mtx, scale, qrot, restPosition, skew, perspective);

	restRotation = glm::eulerAngles(qrot);
	ResetPose();
}


void Bone::SetPosition(const glm::vec3& v)
{
	position = v;
}


void Bone::SetRotation(const glm::vec3& v)
{
	rotation = v;
}


glm::mat4 Bone::GetLocalTransform() const
{
	glm::mat4 tr = glm::translate(glm::mat4(1), position);
	tr *= glm::toMat4(glm::quat(rotation));
	return tr;
}


const glm::mat4& Bone::GetWorldTransform() const
{
	// World Matrix is Dirty?
	if (isTransformDirty)
	{
		// Has Parent?
		if (parent)
		{
			worldTransform = parent->GetWorldTransform() * GetLocalTransform();
		}
		else
		{
			worldTransform = GetLocalTransform();
		}

		isTransformDirty = false;
	}

	return worldTransform;
}


void Bone::DirtyWorldMatrix() const
{
	isTransformDirty = true;
}


} // End of namespace Raven.


