#version 450 core



// Vertex Shader Output.
in VertexOutput
{
	vec2 texCoord;
	
} inFrag;



// Input G-Buffer...
uniform sampler2D inAlbedoSpecular;
uniform sampler2D inNormal;
uniform sampler2D inBRDF;
uniform sampler2D inDepth;



layout(location=0) out vec4 outFinalColor;




// --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- 




void main()
{
	vec3 normal = texture(inNormal, inFrag.texCoord).xzy;
	
	if (normal.x != 0.0 || normal.y != 0.0 || normal.z != 0.0)
		outFinalColor.rgb = normal * 0.5 + 0.5;
		
	outFinalColor.rgb = texture(inAlbedoSpecular, inFrag.texCoord).rgb;
	
	//outFinalColor = vec4(1.0 - DepthToLinaer(texture(inDepth, inFrag.texCoord).r) * 0.001);
	//outFinalColor = vec4(sin(inCommon.time), 1.0 - sin(inCommon.time), 0.0, 1.0);
}
