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
// -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- 



vec3 ComputeNormal(vec2 texCoord)
{
	vec2 texelSize = textureSize(inHeightMap, 0);
	
	float h0 = texture(inHeightMap, texCoord - vec2(texelSize.x, 0.0)).r;
	float h1 = texture(inHeightMap, texCoord + vec2(texelSize.x, 0.0)).r;
	
	float h2 = texture(inHeightMap, texCoord - vec2(0.0, texelSize.y)).r; 
	float h3 = texture(inHeightMap, texCoord + vec2(0.0, texelSize.y)).r; 
	
	return normalize( vec3(h1 - h0, 2.0, h3 - h2) * 0.5 );
}





void main()
{
	vec2 uv = vec2(inPosition.x / inScale.x, inPosition.z / inScale.y);
	float height = texture(inHeightMap, uv).r * (inHeight.y - inHeight.x) + inHeight.x; 

	vec4 worldPos = inModelMatrix * vec4(inPosition.x, height , inPosition.z, 1.0);
	vec4 worldNormal = inNormalMatrix * vec4(ComputeNormal(uv), 1.0);
	
	gl_Position = inViewProjMatrix * worldPos;
	
	outVertex.position = worldPos.xyz;
	outVertex.normal = worldNormal.xyz;
	outVertex.texCoord = uv;
}





