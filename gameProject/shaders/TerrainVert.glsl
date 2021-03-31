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
	
} vxOut;



// Uniforms...
uniform vec2 inScale;
uniform vec2 inHeight;
uniform sampler2D inHeightMap;




// -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- 
// -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- 



vec3 ComputeNormal(float h0, vec2 texCoord)
{
	vec2 texSize = textureSize(inHeightMap, 0);
	vec2 texelSize = vec2(1.0 / texSize.x, 1.0 / texSize.y);
	
	float h1 = texture(inHeightMap, texCoord + vec2(texelSize.x, 0.0)).r;
	float h2 = texture(inHeightMap, texCoord + vec2(0.0, texelSize.y)).r; 
	
	vec3 diffX = vec3(texelSize.x, 0.0, h1 - h0);
	vec3 diffY = vec3(0.0, texelSize.y, h2 - h0);
	diffX = normalize(diffX);
	diffY = normalize(diffY);
	vec3 normal = cross(diffX, diffY);

	float len = length(normal);
	normal = len > 0.00001 ? normal / len : vec3(0.0, 0.0, 1.0);
	
	return normal;
}





void main()
{
	vec2 uv = vec2(inPosition.x / inScale.x, inPosition.z / inScale.y);
	float height = texture(inHeightMap, uv).r; 

	vxOut.position = vec3(inPosition.x, height * (inHeight.y - inHeight.x) + inHeight.x, inPosition.z);
	gl_Position = inProjectionMatrix * inViewMatrix * inModelMatrix * vec4(vxOut.position, 1.0);
	
	vxOut.position = (inModelMatrix * vec4(vxOut.position, 1.0)).xyz;
	vxOut.normal = ComputeNormal(height, uv);
	vxOut.texCoord = uv;
}





