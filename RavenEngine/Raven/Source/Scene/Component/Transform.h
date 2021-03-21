//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once


#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>


namespace Raven 
{

	constexpr glm::vec3 ZERO(0, 0, 0);
	constexpr glm::vec3 LEFT(-1.0f, 0.0f, 0.0f);
	constexpr glm::vec3 RIGHT(1.0f, 0.0f, 0.0f);
	constexpr glm::vec3 UP(0.0f, 1.0f, 0.0f);
	constexpr glm::vec3 DOWN(0.0f, -1.0f, 0.0f);
	constexpr glm::vec3 FORWARD(0.0f, 0.0f, 1.0f);
	constexpr glm::vec3 BACK(0.0f, 0.0f, -1.0f);
	constexpr glm::vec3 ONE(1.0f, 1.0f, 1.0f);

	class Transform final
	{
	public:
		Transform();
		Transform(const glm::mat4 & matrix);
		Transform(const glm::vec3 & position);
		~Transform();

		void SetWorldMatrix(const glm::mat4& mat);
		void SetLocalTransform(const glm::mat4& localMat);
		void SetLocalPosition(const glm::vec3& localPos);
		void SetLocalScale(const glm::vec3& localScale);
		void SetLocalOrientation(const glm::quat& quat);

		inline auto& GetWorldMatrix() { if (dirty) UpdateLocalMatrix();  return worldMatrix; }
		inline auto& GetLocalMatrix() { if (dirty) UpdateLocalMatrix();  return localMatrix; }

		inline auto GetWorldPosition() const { return glm::vec3{ worldMatrix[3][0], worldMatrix[3][1], worldMatrix[3][2] }; }
		inline auto GetWorldOrientation() const { return glm::quat_cast(worldMatrix); };

		inline auto& GetLocalPosition() const { return localPosition; }
		inline auto& GetLocalScale() const { return localScale; }
		inline auto& GetLocalOrientation() const { return localOrientation; }


		inline auto HasUpdated() const { return hasUpdated; }
		inline void SetHasUpdated(bool set) { hasUpdated = set; }

		void UpdateLocalMatrix();
		void ApplyTransform();

		glm::vec3 GetUpDirection() const;

		glm::vec3 GetRightDirection() const;
		
		glm::vec3 GetForwardDirection() const;
	

	protected:
		glm::mat4 localMatrix = glm::mat4(1);
		glm::mat4 worldMatrix = glm::mat4(1);

		glm::vec3 localPosition = {};
		glm::vec3 localScale = {};
		glm::quat localOrientation = {};

		bool hasUpdated = false;
		bool dirty = false;
	};
};