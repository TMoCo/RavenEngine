#pragma once



#include "Utilities/Core.h"
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "glm/matrix.hpp"

#include <vector>



namespace Raven
{
	class GLVertexArray;
	class GLBuffer;
	class GLShader;
	class RenderRscShader;



	// RenderGrid:
	//    - 
	//
	class RenderGrid
	{
		// Cosntruct.
		RenderGrid();

	public:
		// Destruct.
		~RenderGrid();

		// Create a rende screen.
		static RenderGrid* Create();

		// Draw.
		void Draw();

	private:
		// Create sphere geometry.
		static void MakeGrid(std::vector<glm::vec3>& outVerts);

	private:
		// The OpenGL Vertex Array of the mesh, defines mesh vertex input.
		GLVertexArray* vxArray;

		// OpenGL Buffer for Sphere Positions.
		GLBuffer* vxBuffer;

		// Number of vertices in the grid.
		int32_t numVerts;

		// Grid Shader.
		Ptr<RenderRscShader> shader;
	};

}


