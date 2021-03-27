//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#include "Math/BoundingBox.h"
#include "Math/MathUtils.h"

namespace Raven 
{
	namespace MathUtils
	{
		// add a vertex and update bounding box min and max if nescessary
		void BoundingBox::AddVertex(const glm::vec3& vert)
		{
			if (min.x > vert.x + EPS)
				min.x = vert.x;
			if (max.x < vert.x - EPS)
				max.x = vert.x;
			if (min.y > vert.y + EPS)
				min.y = vert.y;
			if (max.y < vert.y - EPS)
				max.y = vert.y;
			if (min.z > vert.z + EPS)
				min.z = vert.z;
			if (max.z < vert.z - EPS)
				max.z = vert.z;
		}

		EIntersection BoundingBox::DoesPointIntersect(const glm::vec3 point) const
		{
			// point is outside if either of its xyz is bigger or smaller than the corresponding max or min respectively
			if (point.x > min.x || point.x < max.x ||
				point.y > min.y || point.y < max.y ||
				point.z > min.z || point.z < max.z)
			{
				return EIntersection::Outside;
			}
			else
			{
				return EIntersection::Inside;
			}
		}

		EIntersection BoundingBox::DoesBoxIntersect(const BoundingBox& other) const
		{
			// other box is outside if either of its max or min is smaller or bigger
			// than this box's corresponding min or max respectively
			if (other.max.x < min.x || max.x > other.min.x ||
				other.max.y < min.y || max.y > other.min.y ||
				other.max.z < min.z || max.z > other.min.z)
			{
				return EIntersection::Outside;
			}
			else 
			// other box is intersecting if either of its max or min is or smaller 
			// than this box's corresponding max or min respectively
			if (other.max.x > max.x || min.x < other.min.x ||
				other.max.y > max.y || min.y < other.min.y ||
				other.max.z > max.z || min.z < other.min.z)
			{
				return EIntersection::Intesects;
			}
			// remaining case must be Inside
			else
			{
				return EIntersection::Inside;
			}
		}


		glm::vec3 BoundingBox::RayIntersection(const glm::vec3 dir) const
		{
			// get intersection point with box, how to process no intersection?
		}

		void BoundingBox::UpdateCentre()
		{
			// update the centre depending on min and max values
			centre = glm::vec3((min.x + max.x) / 2.0f, (min.y + max.y) / 2.0f, (min.z + max.z) / 2.0f);

		}
	};
};
