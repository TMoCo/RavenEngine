//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include <string>
#include <glm/glm.hpp>

namespace Raven
{
	namespace MathUtils 
	{
		constexpr float EPS = 0.000001f;


		bool operator >=(const glm::vec4& left, const glm::vec4& other)
		{
			return left.x >= other.x && left.y >= other.y && left.z >= other.z && left.w >= other.w;
		}

		bool operator <=(const glm::vec4& left, const glm::vec4& other)
		{
			return left.x <= other.x && left.y <= other.y && left.z <= other.z && left.w <= other.w;;
		}


		bool operator >=(const glm::vec3& left, const glm::vec3& other)
		{
			return left.x >= other.x && left.y >= other.y && left.z >= other.z;
		}

		bool operator <=(const glm::vec3& left, const glm::vec3& other)
		{
			return left.x <= other.x && left.y <= other.y && left.z <= other.z;
		}

		bool operator >=(const glm::vec2& left, const glm::vec2& other)
		{
			return left.x >= other.x && left.y >= other.y ;
		}

		bool operator <=(const glm::vec2& left, const glm::vec2& other)
		{
			return left.x <= other.x && left.y <= other.y ;
		}


		//only support for float value and vector
		template <class T>

		typename std::enable_if<
			std::is_floating_point<T>::value || 
			std::is_same<T,glm::vec2>::value || 
			std::is_same<T, glm::vec3>::value || 
			std::is_same<T, glm::vec4>::value,bool>::type Equals(const T& lhs, const T& rhs) {
			T eps(0.000001f); 
			return lhs + eps >= rhs && lhs - eps <= rhs; 
		}

	};
};
