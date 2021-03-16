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
		Debug
	};



	//
	//
	class RenderRscShader : public IRenderResource
	{
	public:
		// Construct.
		RenderRscShader();

		// Destruct.
		~RenderRscShader();

		//
		void Load(ERenderShaderType type, std::string name);

		//
		inline GLShader* GetShader() { return shader.get(); }

	private:
		// 
		ERenderShaderType type;

		// The Shader.
		std::shared_ptr<GLShader> shader;

		// Input uniform buffer, this is specific to this shader resource.
		std::shared_ptr<GLBuffer> input;
	};

}


