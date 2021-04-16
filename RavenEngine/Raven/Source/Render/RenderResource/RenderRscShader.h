#pragma once





#include "IRenderResource.h"
#include "Render/OpenGL/GLTypes.h"


#include <memory>
#include <string>
#include <vector>
#include <map>




namespace Raven
{
	class GLShader;
	class GLBuffer;
	class GLTexture;



	// Different Types of shaders types each with its own shader source base.
	enum class ERenderShaderType
	{
		//
		Terrain,

		//
		MaterialOpaque,
		
		//
		MaterialMasked,

		//
		MaterialTranslucent,

		//
		PostProcessing,

		// Debug Drawing Shader.
		Debug,

		SkinnedMesh

	};



	// ShaderInput
	//		- describe an input that the shader expect.
	class ShaderInput
	{
	public:
		//
		enum EInputType
		{
			None = 0,
			Uniform,
			Sampler,
			UniformBlock
		};

		// The Input Type.
		EInputType type;

		// The name of the input.
		std::string name;

		// Binding.
		uint32_t binding;

		// GLOBAL INPUT BINDINGS...
		static uint32_t TRANSFORM_BINDING;
		static uint32_t LGHTING_BINDING;
	};




	// RenderRscShader:
	//		-  
	class RenderRscShader : public IRenderResource
	{
	public:
		// Construct.
		RenderRscShader();

		// Destruct.
		~RenderRscShader();

		// Load a Shader of type.
		void Load(ERenderShaderType type, std::string name);

		//
		inline GLShader* GetShader() { return shader.get(); }

	private:
		//
		void AddInput(ShaderInput::EInputType type, const std::string& name, uint32_t binding = ((uint32_t)-1));

	private:
		// The Render Shader Type.
		ERenderShaderType type;

		// The Shader.
		std::shared_ptr<GLShader> shader;

		// Input description.
		std::vector<ShaderInput> inputs;
	};

}


