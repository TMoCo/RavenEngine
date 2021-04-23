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

		// BoundingBox:
		//		- Axis Aligned Bounding Box.
		//
		class BoundingBox
		{
		public:
			// Default Invalid Bounding box.
			BoundingBox();

			// Construct bounding box using min/max.
			BoundingBox(const glm::vec3& inMin, const glm::vec3& inMax);

			// Construct bounding box using center & scale.
			BoundingBox(const glm::vec3& center, float scaleX, float scaleY, float scaleZ);

			// Copy Construct
			inline BoundingBox(const BoundingBox& other)
				: min(other.min)
				, max(other.max)
				, isValid(other.isValid)
			{

			}

			// add a vertex and update bounding box min and max.
			void Add(const glm::vec3& vert);

			// add another bounding box to be contained by this bounding box.
			void Add(const BoundingBox& other);

			// Test if a point is inside/outside the bounding box.
			EIntersection DoesPointIntersect(const glm::vec3 point) const;

			// Test if two bounding box intersect with each other.
			EIntersection DoesBoxIntersect(const BoundingBox& other) const;

			// Test if a ray intersect a bounding box and return the paramter if there is an intersection.
			// @parama outAlpha: is a scalar value that can be used to compute the point using the line quation start + dir * outAlpha.
			bool RayIntersection(const glm::vec3& org, const glm::vec3& dir, float& outAlpha) const;

			// Return the center of the bounding box.
			inline glm::vec3 GetCentre() const { return (min + max) * 0.5f; }

			// Return the boudning box extent.
			inline glm::vec3 GetExtent() const { return (max - min) * 0.5f; }

			// Return the radius of the sphere that contain the bounding box.
			inline void GetSphere(glm::vec3& outCenter, float& outRadius) const
			{
				outCenter = GetCentre();
				outRadius = glm::length(GetExtent());
			}

			// Return true if the bounding box values are valid.
			inline bool IsValid() { return isValid; }

			// Reset the bounding box to invalid state.
			inline void Reset() { isValid = false; }

			// Transform the bounding box and return it.
			BoundingBox Transform(const glm::mat4& mtx) const;

		private:
			// If false then min and max values are invalid.
			bool isValid;

			// The minimum point of the bounding box.
			glm::vec3 min;

			// The maximum point of the bounding box.
			glm::vec3 max;

		};
	}
}