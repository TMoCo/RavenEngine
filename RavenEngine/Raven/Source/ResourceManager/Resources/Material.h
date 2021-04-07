//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once


#include "ResourceManager/Resources/IResource.h"

#include <glm/glm.hpp>
#include <map>
#include <vector>



namespace Raven
{
	class MaterialShader;
	class Texture2D;
	class RenderRscMaterial;


	// Dirty flag used to update the materail.
	enum class EMaterialDirtyFlag
	{
		// Not Dirty.
		None,

		// The material need a remapping.
		Remap,

		// The material data changed but no remapping needed.
		Update
	};




	// Material:
	//		- Manage material properties and how they are related to a material shader.
	//
	class Material : public IResource
	{
		NOCOPYABLE(Material);

	public:
		// Construct.
		Material(Ptr<MaterialShader> inShader);

		// Destruct.
		virtual ~Material();

		// Return the type.
		inline static EResourceType Type() noexcept { return EResourceType::RT_Material; }

		// Return the materail render resource.
		inline RenderRscMaterial* GetRenderRsc() { return renderRsc; }

		// Set material paramters by name, Return the index of the paramter.
		int32_t SetTexture(const std::string& name, Texture2D* texture);
		int32_t SetScalar(const std::string& name, const float& scalar);
		int32_t SetColor(const std::string& name, const glm::vec4& color);

		// Get material paramters by name, return false if name doesn't exist.
		bool GetTexture(const std::string& name, Texture2D*& outTexture) const;
		bool GetScalar(const std::string& name, float& outScalar) const;
		bool GetColor(const std::string& name, glm::vec4& outColor) const;

		// Set the value of an existing material paramters using its assigned index.
		void SetTexture(int32_t idx, Texture2D* texture);
		void SetScalar(int32_t idx, const float& scalar);
		void SetColor(int32_t idx, const glm::vec4& color);

		// Return the index of an existing material paramters using its name.
		int32_t GetTextureIndex(const std::string& name) const;
		int32_t GetScalarIndex(const std::string& name) const;
		int32_t GetColorIndex(const std::string& name) const;

		// Return all paramters.
		inline const std::vector< std::pair<std::string, Texture2D*> >& GetTextures() const { return textures; }
		inline const std::vector< std::pair<std::string, float> >& GetScalars() const { return scalars; }
		inline const std::vector< std::pair<std::string, glm::vec4> >& GetColors() const { return colors; }

		// Load the materail on GPU.
		void LoadOnGpu();

		// Return true if the materail is dirty and need update.
		inline bool IsDirty() { return dirtyFlag != EMaterialDirtyFlag::None; }

		// Update The Render Materail with the new paramters.
		void Update();

	private:
		// Mark Dirty for update.
		void DirtyUpdate();

		// Mark Dirty for remap.
		void DirtyRemap();

	private:
		// The materail shader.
		Ptr<MaterialShader> shader;

		// The resrouce material.
		RenderRscMaterial* renderRsc;

		// Map paramters name to their value.
		std::map<std::string, int32_t> texturesMap;
		std::map<std::string, int32_t> scalarsMap;
		std::map<std::string, int32_t> colorsMap;

		// Texture material paramters
		std::vector< std::pair<std::string, Texture2D*> > textures;

		// Scalar material paramters
		std::vector< std::pair<std::string, float> > scalars;

		// Colors material paramters
		std::vector< std::pair<std::string, glm::vec4> > colors;

		// Flags used to update material with the render.
		EMaterialDirtyFlag dirtyFlag;

	};
}