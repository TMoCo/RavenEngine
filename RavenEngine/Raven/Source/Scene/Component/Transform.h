//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once


#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Component.h"

namespace Raven 
{
	class Mesh;

	constexpr glm::vec3 ZERO(0, 0, 0);
	constexpr glm::vec3 LEFT(-1.0f, 0.0f, 0.0f);
	constexpr glm::vec3 RIGHT(1.0f, 0.0f, 0.0f);
	constexpr glm::vec3 UP(0.0f, 1.0f, 0.0f);
	constexpr glm::vec3 DOWN(0.0f, -1.0f, 0.0f);
	constexpr glm::vec3 FORWARD(0.0f, 0.0f, 1.0f);
	constexpr glm::vec3 BACK(0.0f, 0.0f, -1.0f);
	constexpr glm::vec3 ONE(1.0f, 1.0f, 1.0f);


	class Transform final : public Component
	{
	public:
		Transform();
		Transform(const glm::mat4 & matrix);
		Transform(const glm::vec3 & position);
		~Transform();

		void SetTransform(Transform& other);
		void SetWorldMatrix(const glm::mat4& mat);
		void SetLocalTransform(const glm::mat4& localMat);
		void SetOffsetTransform(const glm::mat4& localMat);
		void SetLocalPosition(const glm::vec3& localPos);
		void SetLocalScale(const glm::vec3& localScale);
		//void SetLocalOrientation(const glm::quat& quat);
		void SetLocalOrientation(const glm::vec3& rotation);

		inline auto& GetWorldMatrix() { if (dirty) UpdateLocalMatrix();  return worldMatrix; }
		inline auto& GetLocalMatrix() { if (dirty) UpdateLocalMatrix();  return localMatrix; }

		inline auto GetWorldPosition() const { return glm::vec3{ worldMatrix[3][0], worldMatrix[3][1], worldMatrix[3][2] }; }
		inline auto GetWorldOrientation() const { return glm::quat_cast(worldMatrix); };

		inline auto& GetLocalPosition() const { return localPosition; }
		inline auto& GetLocalScale() const { return localScale; }
		inline auto& GetLocalOrientation() const { return localOrientation; }
		inline auto& GetOffsetMatrix() const { return offsetMatrix; }


		void ResetTransform();

		inline auto HasUpdated() const { return hasUpdated; }
		inline void SetHasUpdated(bool set) { hasUpdated = set; }

		inline auto IsDirty() const { return dirty; }

		void UpdateLocalMatrix();
		void ApplyTransform();

		glm::vec3 GetUpDirection() const;

		glm::vec3 GetRightDirection() const;
		
		glm::vec3 GetForwardDirection() const;
	

		static glm::vec3 GetScaleFromMatrix(const glm::mat4& mat);

		// get an empty transform
		static Transform Identity();


		template<typename Archive>
		void save(Archive& archive) const
		{
			archive(cereal::make_nvp("Position", localPosition), cereal::make_nvp("Rotation", localOrientation), cereal::make_nvp("Scale", localScale), cereal::make_nvp("Id", entity));
		}

		template<typename Archive>
		void load(Archive& archive)
		{
			archive(cereal::make_nvp("Position", localPosition), cereal::make_nvp("Rotation", localOrientation), cereal::make_nvp("Scale",  localScale), cereal::make_nvp("Id", entity));
			dirty = true;
			initLocalPosition = localPosition;
			initLocalScale = localScale;
			initLocalOrientation = localOrientation;
		}

	protected:
		glm::mat4 localMatrix = glm::mat4(1);
		glm::mat4 worldMatrix = glm::mat4(1);
		glm::mat4 offsetMatrix = glm::mat4(1);



		glm::vec3 localPosition = {};
		glm::vec3 localScale = {};
		glm::vec3 localOrientation = {};

		glm::vec3 initLocalPosition = {};
		glm::vec3 initLocalScale = {};
		glm::vec3 initLocalOrientation = {};


		bool hasUpdated = false;
		bool dirty = false;
	};



	class BoneWarpper : public Component
	{
	public:
		Mesh * mesh;
		std::vector<uint32_t> indices;
	};

};



