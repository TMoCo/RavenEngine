
// Transform Unifrom Block.
layout(std140) uniform TransformBlock
{
	// The Model/Object matrix.
	mat4 inModelMatrix;
	
	// The Model/Object normals transform matrix.
	mat4 inNormalMatrix;
	
	// The View & Projection Matrix.
	mat4 inViewProjMatrix;
};



