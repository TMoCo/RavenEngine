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
		public:
			BoundingBox() : min(0.0f), max(0.0f), centre(0.0f) {}

			// add a vertex and update bounding box min and max if nescessary
			void AddVertex(const glm::vec3& vert);

			EIntersection DoesPointIntersect (const glm::vec3 point) const;
			EIntersection DoesBoxIntersect (const BoundingBox& other) const;

			glm::vec3 RayIntersection(const glm::vec3 dir) const;

			void UpdateCentre(); // update the centre depending on min and max values

			inline glm::vec3 GetCentre() const { return centre; }

		private:
			glm::vec3 min; // minimum x,y,z 
			glm::vec3 max; // maximum x,y,z 

			glm::vec3 centre; // bounding box centre
		};
	}
}