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
			return rp3d::Transform(ToRp3dVector3(t.GetWorldPosition()), ToRp3dQuaternion(t.GetWorldOrientation()));
		}

		rp3d::Vector3 ToRp3dVector3(const glm::vec3& v)
		{
			return rp3d::Vector3(v.x, v.y, v.z);
		}

		rp3d::Quaternion ToRp3dQuaternion(const glm::quat& q)
		{
			// !rp3d has scalar after vector! 
			return rp3d::Quaternion(q.x, q.y, q.z, q.w);
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
	}
}