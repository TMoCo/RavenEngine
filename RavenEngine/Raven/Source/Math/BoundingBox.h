//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include <utility>

#include "glm/glm.hpp"

#include "Math/MathUtils.h"


namespace Raven
{
	namespace MathUtils
	{
		class BoundingBox
		{
			BoundingBox() : min(0.0f), max(0.0f), centre(0.0f) {}

			// add a vertex and update bounding box min and max if nescessary
			void AddVertex(const glm::vec3& vert);

			EIntersection PointIntersection (const glm::vec3 point) const;
			EIntersection RayIntersection(const glm::vec3 dir) const;
			EIntersection BoxIntersection (const BoundingBox& other) const;

			void UpdateCentre(); // update the centre depending on min and max values

		private:
			glm::vec3 min; // minimum x,y,z 
			glm::vec3 max; // maximum x,y,z 

			glm::vec3 centre; // bounding box centre
		};
	}
}