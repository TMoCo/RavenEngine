//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include <string>
#include "Utilities/Core.h"

#include <entt/entity/fwd.hpp>
#include <cereal/cereal.hpp>

namespace Raven
{
	class SceneGraph final
	{
	public:
		SceneGraph() = default;
		~SceneGraph() = default;

		void Init(entt::registry & registry);
		void DisconnectOnConstruct(bool disable, entt::registry & registry);
		void Update(entt::registry & registry);
		void UpdateTransform(entt::entity entity, entt::registry & registry);
	};

};