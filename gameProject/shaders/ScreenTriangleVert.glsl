#version 450 core


// Input Attributes...
layout(location=0) in vec2 inPosition;
layout(location=1) in vec2 inTexCoord;






// Vertex Shader Output.
out VertexOutput
{
	vec2 texCoord;
} outVertex;


// The Actual Size of the target, used to scale the uv to correctly sample targets.
uniform vec2 inRTSize;



// -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- 
// -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- 





void main()
{
	//
	gl_Position = vec4(inPosition.x, inPosition.y, 0.5, 1.0);
	
	// Set Vertex-Output...
	outVertex.texCoord = (inCommon.viewport.zw / inRTSize) * inTexCoord.xy;
}

