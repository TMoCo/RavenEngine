#version 450 core



layout(location=0) in vec3 inPosition;





// Vertex Shader Output.
out VertexOutput
{
	vec3 position;
	vec3 normal;
	vec2 texCoord;
} outVertex;



// Uniforms...
uniform vec2 inScale;
uniform vec2 inHeight;
uniform sampler2D inHeightMap;





// -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- 



vec3 ComputeNormal(vec2 texCoord)
{
	vec2 texelSize = 1.0 / textureSize(inHeightMap, 0);
	
	float h0 = texture(inHeightMap, texCoord - vec2(texelSize.x, 0.0)).r * (inHeight.y - inHeight.x) + inHeight.x;
	float h1 = texture(inHeightMap, texCoord + vec2(texelSize.x, 0.0)).r * (inHeight.y - inHeight.x) + inHeight.x;
	
	float h2 = texture(inHeightMap, texCoord - vec2(0.0, texelSize.y)).r * (inHeight.y - inHeight.x) + inHeight.x; 
	float h3 = texture(inHeightMap, texCoord + vec2(0.0, texelSize.y)).r * (inHeight.y - inHeight.x) + inHeight.x; 
	
	return normalize( vec3(h0 - h1, 2.0, h2 - h3) * 0.5 );
}





void main()
{
	vec2 uv = vec2(inPosition.x / inScale.x, inPosition.z / inScale.y);
	float height = texture(inHeightMap, uv).r * (inHeight.y - inHeight.x) + inHeight.x; 

	vec4 worldPos = inModelMatrix * vec4(inPosition.x, height , inPosition.z, 1.0);
	vec4 worldNormal = inNormalMatrix * vec4(ComputeNormal(uv), 0.0);
	
	gl_Position = inCommon.viewProjMatrix * worldPos;
	
	outVertex.position = worldPos.xyz;
	outVertex.normal = worldNormal.xyz;
	outVertex.texCoord = uv;
}





