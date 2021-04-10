#pragma once




#include "glm/vec2.hpp"





namespace Raven
{
	class GLVertexArray;
	class GLBuffer;
	class GLShader;


	// RenderScreen:
	//    - Render a triangle covering the entire screen.
	//
	class RenderScreen
	{
		// Cosntruct.
		RenderScreen();

	public:
		// Destruct.
		~RenderScreen();

		// Create a rende screen.
		static RenderScreen* Create();

		// Draw.
		void Draw(GLShader* shader);

		// Set the render target actual size.
		void SetRTSize(const glm::vec2& size) { rtSize = size; }

	private:
		// The OpenGL Vertex Array of the mesh, defines mesh vertex input.
		GLVertexArray* vxarray;

		// OpeGL Buffer for Mesh Positions.
		GLBuffer* positionBuffer;

		// The render target actual size, used to scale the uv to correctly sample targets.
		glm::vec2 rtSize;
	};

}


