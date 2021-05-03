//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#include "Utilities/ToRp3d.h"

#include <glm/gtc/type_ptr.hpp>

namespace Raven
{
	namespace Rp3dConvert
	{
		rp3d::Transform ToRp3dTransform(const Transform& t)
		{
			return rp3d::Transform(ToRp3dVector3(t.GetWorldPosition()), ToRp3dQuaternion(t.GetWorldRotation()));
		}

		rp3d::Vector3 ToRp3dVector3(const glm::vec3& v)
		{
			return rp3d::Vector3(v.x, v.y, v.z);
		}

		rp3d::Quaternion ToRp3dQuaternion(const glm::quat& q)
		{
			// make sure to normalize the quaternion
			glm::quat quat = glm::normalize(q);
			// !rp3d has scalar after vector! 
			return rp3d::Quaternion(quat.x, quat.y, quat.z, quat.w);
		}

		Transform ToTransform(const rp3d::Transform& other)
		{
			// construct from a glm mat4
			glm::mat4 t;
			// column major
			other.getOpenGLMatrix(glm::value_ptr(t));
			// a new Raven::Transform with the rp3d transform data
			return Transform(t);
		}

		glm::quat ToGLMQuat(const rp3d::Quaternion other)
		{
			glm::quat out;
			out.x = other.x;
			out.y = other.y;
			out.z = other.z;
			out.w = other.w;
			return out;
		}
	}
}