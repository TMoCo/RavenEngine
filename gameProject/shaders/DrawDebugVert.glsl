#version 330 core



layout(location=0) in vec3 inPosition;


// Transform Unifrom Block.
layout(std140) uniform TransformBlock
{
	mat4 inModelMatrix;
	mat4 inViewMatrix;
	mat4 inProjectionMatrix;
};




void main()
{
	gl_Position = inProjectionMatrix * inViewMatrix * inModelMatrix * vec4(inPosition, 1.0);
	
	
}

