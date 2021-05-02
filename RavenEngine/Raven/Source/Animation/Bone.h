//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Utilities/Core.h"
#include "Utilities/Serialization.h"

#include <cereal/cereal.hpp>
#include <string>
#include <vector>
#include <memory>


namespace Raven
{
	class Skeleton;


	// Bone in a Skeleton.
	class Bone
	{
		// Friend.
		friend class Skeleton;

	public:
		// Construct.
		Bone();

		//
		template<class Archive>
		void load(Archive& archive)
		{
			RAVEN_ASSERT(0, "TODO: Implement Bone Archive.");
		}

		//
		template<class Archive>
		void save(Archive& archive) const 
		{
			RAVEN_ASSERT(0, "TODO: Implement Bone Archive.");
		}

		// Apply reset pose to current bone transform.
		void ResetPose();

		// Set the rest pose transform and save it for later use.
		void SetRestPose(const glm::mat4& mtx);

		// Set bone transform position.
		void SetPosition(const glm::vec3& v);

		// Return bone transform position.
		const glm::vec3& GetPosition() const { return position; }

		// Set bone transform rotation.
		void SetRotation(const glm::vec3& v);

		// Return bone transform rotation.
		const glm::vec3& GetRotation() const { return rotation; }

		// Return bone transformation as matrix.
		glm::mat4 GetLocalTransform() const;

		// Return bone world transform
		const glm::mat4& GetWorldTransform() const;

		// Set bone name.
		inline void SetName(const std::string& inName) { name = inName; }

		// Return bone name.
		inline const std::string& GetName() const { return name; }

		// Set Bone Offset Matrix.
		inline void SetOffsetMatrix(const glm::mat4& matrix) { offsetMatrix = matrix; }

		// Get Bone Offset Matrix.
		inline const glm::mat4& GetOffsetMatrix() const { return offsetMatrix; }

		// Return bone parent.
		inline Bone* GetParent() const { return parent; }

		// Return bone children.
		inline const std::vector<Bone*>& GetChildren() const { return children; }

		// Mark world matrix dirty.
		void DirtyWorldMatrix() const;

	private:
		// Bone Name.
		std::string name;

		// The Bone Offset Matrix.
		glm::mat4 offsetMatrix;

		// Parent Bone.
		Bone* parent;

		// Children Bone.
		std::vector<Bone*> children;

		// Index of the parent bone in the skeleton.
		int32_t parentIdx;

		// Id of the bone in the skeleton.
		int32_t id;

		// The Bone Local Transformation.
		glm::vec3 position;
		glm::vec3 rotation;

		// The Bone Rest Local Transformation.
		glm::vec3 restPosition;
		glm::vec3 restRotation;

		// The Bone World Transformation.
		mutable glm::mat4 worldTransform;

		// if dirty world transform need updating.
		mutable bool isTransformDirty;
	};


}
