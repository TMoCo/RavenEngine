#pragma once

#include <glm/glm.hpp>

namespace Raven
{
    class  Ray
    {
    public:
        friend class Camera;
        Ray() noexcept = default;

        Ray(const glm::vec3& origin, const glm::vec3& direction) 
        {
            Define(origin, direction);
        }

        bool operator ==(const Ray& rhs) const { return origin == rhs.origin && direction == rhs.direction; }

        bool operator !=(const Ray& rhs) const { return origin != rhs.origin || direction != rhs.direction; }

        inline void Define(const glm::vec3& origin, const glm::vec3& direction)
        {
            this->origin = origin;
            this->direction = glm::normalize(direction);
        }

        inline glm::vec3 Project(const glm::vec3& point) const
        {
            glm::vec3 offset = point - origin;
            return origin + glm::dot(offset, direction) * direction;
        }

        inline float Distance(const glm::vec3& point) const
        {
            glm::vec3 projected = Project(point);
            return glm::length(point - projected);
        }

        glm::vec3 ClosestPoint(const Ray& ray) const;

    private:
        glm::vec3 origin;
        glm::vec3 direction;
    };
}
