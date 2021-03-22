#version 330 core



layout(location=0) in vec3 inPosition;





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
	
} output;



// Uniforms...
uniform vec2 inScale;
uniform float inHeightFactor;
uniform sampler2D inHeightMap;




// -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- 
// -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- 




void main()
{
	vec2 uv = vec2(inPosition.x / inScale.x, inPosition.z / inScale.y);
	float height = texture(inHeightMap, uv).r; 

	output.position = vec3(inPosition.x, height * inHeightFactor, inPosition.z);
	gl_Position = inProjectionMatrix * inViewMatrix * inModelMatrix * vec4(output.position, 1.0);
	
	output.position = (inModelMatrix * vec4(output.position, 1.0)).xyz;
	output.normal = vec3(0.0, 1.0, 0.0);
	output.texCoord = uv;
}





