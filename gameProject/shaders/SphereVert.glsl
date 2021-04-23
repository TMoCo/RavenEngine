#version 450 core


// Input Attributes...
layout(location=0) in vec3 inPosition;



//
uniform mat4 inViewProjMatrix;


// Vertex Shader Output.
out VertexOutput
{
	vec3 position;
} outVertex;



// -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- 
// -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- 





void main()
{
	//
	gl_Position = inViewProjMatrix * vec4(inPosition, 1.0);
	outVertex.position = inPosition;
}

