//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include <glm/glm.hpp>
#include "Ray.h"
#include "Scene/Component/Component.h"

namespace Raven
{
	class Camera : public Component
	{
	public:
		Camera() = default;
		Camera(float fov, float near, float far, float aspect);
		Camera(float aspectRatio, float scale);
		Camera(float pitch, float yaw, const glm::vec3 & position, float fov, float near, float far, float aspect);
		~Camera() = default;

		inline auto SetMouseSensitivity(float value){ mouseSensitivity = value;	}

		inline auto SetOrthographic(bool ortho){ orthographic = ortho;	}

		inline auto IsOrthographic() const	{ return orthographic;}

		inline auto GetAspectRatio() const { return aspectRatio;}

		inline auto SetAspectRatio(float y)	{ aspectRatio = y;	projectionDirty = true;	};

		const glm::mat4& GetProjectionMatrix();

		inline auto GetFar() const	{ return far; }
		inline auto GetNear() const	{ return near; }

		inline auto SetFar(float pFar)	{ far = pFar;	projectionDirty = true;	}

		inline auto SetNear(float pNear) {near = pNear;projectionDirty = true;}

		inline auto GetFov() const	{	return fov;	}
		inline auto GetScale() const	{	return scale;	}
		inline auto SetScale(float scale)	{this->scale = scale;projectionDirty = true;}
		inline auto SetFov(float fov)	{ this->fov = fov;projectionDirty = true;}

		Ray GetScreenRay(float x, float y, const glm::mat4 & viewMatrix, bool invertY = false) const;

		template<typename Archive>
		void save(Archive& archive) const
		{
			archive(cereal::base_class<Component>(this));

			archive(
				cereal::make_nvp("Scale", scale), 
				cereal::make_nvp("Aspect", aspectRatio), 
				cereal::make_nvp("FOV", fov), 
				cereal::make_nvp("Near", near),
				cereal::make_nvp("Far", far)
			);
		}

		template<typename Archive>
		void load(Archive& archive)
		{
			archive(cereal::base_class<Component>(this));

			archive(
				cereal::make_nvp("Scale", scale),
				cereal::make_nvp("Aspect", aspectRatio),
				cereal::make_nvp("FOV", fov),
				cereal::make_nvp("Near", near),
				cereal::make_nvp("Far", far)
			);

			projectionDirty = true;
		}


	protected:
		void UpdateProjectionMatrix();


		float aspectRatio = 0.0f;
		float scale = 1.0f;
		float zoom = 1.0f;

		glm::mat4 projMatrix = glm::mat4(1.f);
		
		bool projectionDirty = false;

		float fov = 0.0f;
		float near = 0.0f;
		float far = 0.0f;
		float mouseSensitivity = 0.1f;

		bool orthographic = false;
	};
};

