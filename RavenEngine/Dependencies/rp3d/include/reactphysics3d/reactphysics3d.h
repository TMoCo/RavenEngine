/********************************************************************************
* ReactPhysics3D physics library, http://www.reactphysics3d.com                 *
* Copyright (c) 2010-2020 Daniel Chappuis                                       *
*********************************************************************************
*                                                                               *
* This software is provided 'as-is', without any express or implied warranty.   *
* In no event will the authors be held liable for any damages arising from the  *
* use of this software.                                                         *
*                                                                               *
* Permission is granted to anyone to use this software for any purpose,         *
* including commercial applications, and to alter it and redistribute it        *
* freely, subject to the following restrictions:                                *
*                                                                               *
* 1. The origin of this software must not be misrepresented; you must not claim *
*    that you wrote the original software. If you use this software in a        *
*    product, an acknowledgment in the product documentation would be           *
*    appreciated but is not required.                                           *
*                                                                               *
* 2. Altered source versions must be plainly marked as such, and must not be    *
*    misrepresented as being the original software.                             *
*                                                                               *
* 3. This notice may not be removed or altered from any source distribution.    *
*                                                                               *
********************************************************************************/


/********************************************************************************
* ReactPhysics3D                                                                *
* Version 0.8.0                                                                 *
* http://www.reactphysics3d.com                                                 *
* Daniel Chappuis                                                               *
********************************************************************************/

#ifndef REACTPHYSICS3D_H
#define REACTPHYSICS3D_H

// Libraries
#include <rp3d/include/reactphysics3d/configuration.h>
#include <rp3d/include/reactphysics3d/mathematics/mathematics.h>
#include <rp3d/include/reactphysics3d/body/CollisionBody.h>
#include <rp3d/include/reactphysics3d/body/RigidBody.h>
#include <rp3d/include/reactphysics3d/engine/PhysicsCommon.h>
#include <rp3d/include/reactphysics3d/engine/PhysicsWorld.h>
#include <rp3d/include/reactphysics3d/engine/Material.h>
#include <rp3d/include/reactphysics3d/engine/EventListener.h>
#include <rp3d/include/reactphysics3d/collision/shapes/CollisionShape.h>
#include <rp3d/include/reactphysics3d/collision/shapes/BoxShape.h>
#include <rp3d/include/reactphysics3d/collision/shapes/SphereShape.h>
#include <rp3d/include/reactphysics3d/collision/shapes/CapsuleShape.h>
#include <rp3d/include/reactphysics3d/collision/shapes/ConvexMeshShape.h>
#include <rp3d/include/reactphysics3d/collision/shapes/ConcaveMeshShape.h>
#include <rp3d/include/reactphysics3d/collision/shapes/HeightFieldShape.h>
#include <rp3d/include/reactphysics3d/collision/PolyhedronMesh.h>
#include <rp3d/include/reactphysics3d/collision/shapes/AABB.h>
#include <rp3d/include/reactphysics3d/collision/Collider.h>
#include <rp3d/include/reactphysics3d/collision/RaycastInfo.h>
#include <rp3d/include/reactphysics3d/collision/TriangleMesh.h>
#include <rp3d/include/reactphysics3d/collision/PolyhedronMesh.h>
#include <rp3d/include/reactphysics3d/collision/TriangleVertexArray.h>
#include <rp3d/include/reactphysics3d/collision/PolygonVertexArray.h>
#include <rp3d/include/reactphysics3d/collision/CollisionCallback.h>
#include <rp3d/include/reactphysics3d/collision/OverlapCallback.h>
#include <rp3d/include/reactphysics3d/constraint/BallAndSocketJoint.h>
#include <rp3d/include/reactphysics3d/constraint/SliderJoint.h>
#include <rp3d/include/reactphysics3d/constraint/HingeJoint.h>
#include <rp3d/include/reactphysics3d/constraint/FixedJoint.h>
#include <rp3d/include/reactphysics3d/containers/List.h>

/// Alias to the ReactPhysics3D namespace
namespace rp3d = reactphysics3d;

#endif
