//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "ResourceManager/MeshFactory.h"
#include <string>
#include <vector>
#include <memory>
#include "Scene/Component/Transform.h"

#include <cereal/cereal.hpp>
#include "Utilities/Serialization.h"


namespace Raven
{
	struct Bone
	{
		enum { MAX_COUNT = 196 };
		explicit Bone()
			: parentIdx(-1)
		{
		}

		template<class Archive>
		void load(Archive& archive)
		{
			archive(name, offsetMatrix, parentIdx, id);
		}

		template<class Archive>
		void save(Archive& archive) const 
		{
			archive(name, offsetMatrix, parentIdx, id);
		}

	
			
		std::string name;
		Transform * localTransform = nullptr;
		glm::mat4 offsetMatrix;
		int32_t parentIdx;
		int32_t id;
	};

};