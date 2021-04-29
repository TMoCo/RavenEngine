//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ResourceManager/Resources/IResource.h"
#include "Render/RenderResource/Shader/RenderRscShader.h"



namespace Raven
{
	class RenderRscShader;
	class UniformBuffer;



	// Shader:
	//		- manage a render shader resource and provide a way to override its material functions.
	//
	class MaterialShader : public IResource
	{
		NOCOPYABLE(MaterialShader)

	public:
		// Construct.
		MaterialShader();

		// Destruct.
		virtual ~MaterialShader();

		// Return Resrouce Type.
		inline static EResourceType Type() noexcept { return EResourceType::RT_MaterialShader; }

		// Return the materail render resource.
		inline RenderRscShader* GetRenderRsc() { return renderRsc; }

		// Set the shader name.
		void SetName(const std::string& name);

		// Set the material function.
		void SetMaterialFunction(const std::string& func, bool hasComputeVertex = false);

		// Return the block description, which the expected input of our material shader.
		inline RSInputBlockDescription& GetBlockInput() { return blockInput; }
		inline const RSInputBlockDescription& GetBlockInput() const { return blockInput; }

		// Add sampler description as expected input for this shader.
		void AddSampler(const std::string& name);

		// Set Shader Domain.
		void SetDomain(ERenderShaderDomain val);

		// Return the shader domain.
		inline ERenderShaderDomain GetDomain() const { return domain; }

		// Set Shader Type.
		void SetType(ERenderShaderType val);

		// Return the shader type.
		inline ERenderShaderType GetType() const { return type; }

		// Return the shader unifrom buffer.
		inline UniformBuffer* GetUnifromBuffer() const { return materialUBO.get(); }

		// Load the shader on GPU.
		virtual void LoadRenderResource() override;


	private:
		// The Shader Render Resrouce.
		RenderRscShader* renderRsc;

		// The Shade Name.
		std::string name;

		// Path to the material function to load.
		std::string materialFunction;

		// if true then the we have material function for computing vertex.
		bool isComputeMaterialVertex;

		// The block description, which the expected input of our material shader.
		RSInputBlockDescription blockInput;

		// Texture input.
		std::vector<RSInputDescription> samplers;

		// The Shader Domain.
		ERenderShaderDomain domain;

		// The Shader Type.
		ERenderShaderType type;

		// The uniform buffer for the material data.
		Ptr<UniformBuffer> materialUBO;
	};
}