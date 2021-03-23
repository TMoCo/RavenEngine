#version 330 core



layout(location=0) in vec3 inPosition;
layout(location=1) in vec3 inNormal;
layout(location=2) in vec2 inTexCoord;



// Transform Unifrom Block.
layout(std140) uniform TransformBlock
{
	mat4 inModelMatrix;
	mat4 inViewMatrix;
	mat4 inProjectionMatrix;
};


// Vertex Shader Output.
out VertexOutput
{
	vec3 position;
	vec3 normal;
	vec2 texCoord;
	
} vxOut;




// -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- 
// -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- 





void main()
{
	gl_Position = inProjectionMatrix * inViewMatrix * inModelMatrix * vec4(inPosition, 1.0);
	vxOut.position = (inModelMatrix * vec4(inPosition, 1.0)).xyz;
	vxOut.normal 	= (inModelMatrix * vec4(inNormal,   0.0)).xyz;
	vxOut.texCoord = inTexCoord;
}

