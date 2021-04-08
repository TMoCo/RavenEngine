//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#include "Utilities/ToRp3d.h"

namespace Raven
{
	namespace ToRp3d
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
	}
}