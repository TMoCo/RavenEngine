//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include <reactphysics3d/reactphysics3d.h>
#include <reactphysics3d/mathematics/Transform.h>
#include <reactphysics3d/mathematics/Vector3.h>
#include <reactphysics3d/mathematics/Quaternion.h>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Scene/Component/Transform.h"

//
// Convert our maths data types into the ones used by the react 3d library
//

namespace Raven
{
	namespace Rp3dConvert
	{
		// from engine to physics
		rp3d::Transform ToRp3dTransform(const Transform& t);
		rp3d::Vector3 ToRp3dVector3(const glm::vec3& v);
		rp3d::Quaternion ToRp3dQuaternion(const glm::quat& q);

		// from physics to engine
		Transform ToTransform(const rp3d::Transform& other);
		glm::quat ToGLMQuat(const rp3d::Quaternion other);
	}
}