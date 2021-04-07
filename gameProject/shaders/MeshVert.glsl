#version 450 core


// Input Attributes...
layout(location=0) in vec3 inPosition;
layout(location=1) in vec3 inNormal;
layout(location=2) in vec2 inTexCoord;






// Vertex Shader Output.
out VertexOutput
{
	vec3 position;
	vec3 normal;
	vec2 texCoord;
	
} outVertex;




// -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- 
// -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- 





void main()
{
	// Transform to world space.
	vec4 worldPos = inModelMatrix * vec4(inPosition, 1.0);
	vec4 wolrdNormal = inNormalMatrix * vec4(inNormal, 0.0);
	
	//
	gl_Position = inViewProjMatrix * worldPos;
	
	// Set Vertex-Output...
	outVertex.position = worldPos.xyz;
	outVertex.normal = wolrdNormal.xyz;
	outVertex.texCoord = inTexCoord;
}
